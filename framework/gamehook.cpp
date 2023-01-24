#include "pch-il2cpp.h"

#include <iostream>

#include "store.h"
#include "gamehook.h"
#include "../dependencies/kiero/kiero.h"
#include "../dependencies/kiero/minhook/include/MinHook.h"
#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "../dependencies/imgui/imgui_impl_dx11.h"
#include "../user/user.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

bool init = false;
uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t gameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

namespace GameHook
{
	Present oPresent;
	HWND window = NULL;
	WNDPROC oWndProc;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	ID3D11RenderTargetView* mainRenderTargetView;

	void InitImGui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
	}

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!init)
		{
			il2cpp_thread_attach(il2cpp_domain_get());
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
			{
				pDevice->GetImmediateContext(&pContext);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				window = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
				pBackBuffer->Release();
				oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				InitImGui();
				init = true;
			}

			else
				return oPresent(pSwapChain, SyncInterval, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		User::Menu();
		User::Tick();

		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (Store::ejected) {
			std::cout << "Cleaning Up Kiero and ImGui..." << std::endl;
			kiero::shutdown();
			pDevice->Release();
			pContext->Release();
			pSwapChain->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(oWndProc));
			oPresent(pSwapChain, SyncInterval, Flags);
			std::cout << "Done cleaning..." << std::endl;
			return 0;
		}

		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	// void BasePlayerController_object___ApplyRecoil (BasePlayerController_T__o* __this, UnityEngine_Vector2_o recoilAmount, bool isFirstShot, const MethodInfo_19A05B0* method);
	void(__fastcall* ApplyRecoil_o)(DWORD*, Unity::Vector2, bool, DWORD*);
	void __stdcall ApplyRecoil_h(DWORD* playerController, Unity::Vector2 recoilAmount, bool isFirstShot, DWORD* method)
	{
		recoilAmount.x = 0.f;
		recoilAmount.y = 0.f;
	}

	void NoRecoil() {
		if (Store::m_noRecoil && !Store::m_noRecoil_hooked)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + applyRecoilOffset));
			Store::m_noRecoil_hooked = true;
		}

		if (!Store::m_noRecoil && Store::m_noRecoil_hooked)
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(gameAssembly + applyRecoilOffset));
			Store::m_noRecoil_hooked = false;
		}
	}

	void CreateCustomHooks()
	{
		MH_Initialize();

		// Init Everything
		MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + applyRecoilOffset), &ApplyRecoil_h, (LPVOID*)&ApplyRecoil_o);
	}

	void DestroyCustomHooks()
	{
		MH_RemoveHook(reinterpret_cast<LPVOID*>(gameAssembly + applyRecoilOffset));
	}

	void ToggleHooks()
	{
		NoRecoil();
	}
}
