#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

namespace GameHook
{
	extern Present oPresent;
	extern HWND window;
	extern WNDPROC oWndProc;
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	extern ID3D11RenderTargetView* mainRenderTargetView;
	extern bool hkPresentInit;

	void InitImGui();

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	void CreateCustomHooks();
	void DestroyCustomHooks();
	void ToggleHooks();

	// Recoil
	static uintptr_t applyRecoilOffset = 0x139DFE0;
	void NoRecoil();
	void ApplyRecoil_h(DWORD* playerController, Unity::Vector2 recoilAmount, bool isFirstShot, DWORD* method);

	//HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
}
