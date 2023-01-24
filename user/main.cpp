#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include "il2cpp-appdata.h"
#include "helpers.h"

#include "../dependencies/kiero/kiero.h"
#include "../dependencies/resolver/Resolver.hpp"

#include "../framework/util.h"
#include "../framework/store.h"
#include "../framework/gamehook.h"

using namespace app;

extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

// Custom injected code entry point
DWORD WINAPI Run(HMODULE hModule)
{
    Il2CppThread* thread = il2cpp_thread_attach(il2cpp_domain_get());

    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Starting IL2CPP Resolver" << std::endl;
    IL2CPP::Initialize(true);

    std::cout << "Loading custom hooks..." << std::endl;
    GameHook::CreateCustomHooks();

    Util::yeetAnti();

    // Hook visuals
    bool init_hook = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&GameHook::oPresent, GameHook::hkPresent);
            init_hook = true;

            std::cout << "Kiero Hooked" << std::endl;
        }
    } while (!init_hook);

    std::cout << "Ready!" << std::endl;

	while (!Store::ejected)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			std::cout << "Ejecting..." << std::endl;
            
            GameHook::DestroyCustomHooks();
			Store::ejected = true;

			Sleep(1000);
			
            std::cout << "Goodbye!" << std::endl;
            fclose(f);
            FreeConsole();
			FreeLibraryAndExitThread(hModule, 0);
			break;
		}

		if (GetAsyncKeyState(VK_F5) & 1)
		{
			Store::m_show = !Store::m_show;
		}

		Sleep(5);
	}

    return 1;
}