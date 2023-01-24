#pragma once

#include <ctime>
#include "..\dependencies\resolver\Resolver.hpp"

#include "player.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Store
{
	extern bool ejected;

	extern int ScreenW;
	extern int ScreenH;

	extern Unity::CCamera* worldCamera;
	extern Player localPlayer;

	extern Player playerList[16];
	extern clock_t lastPlayerListUpdate;

	// Menu Features
	extern bool m_show;

	extern bool m_espLines;
	extern bool m_espNames;
	extern bool m_espBoxes;
	extern bool m_espDistance;
	extern bool m_espDeadPlayers;

	extern bool m_aim;
	extern bool m_aimFovDisplay;
	extern int m_aimFov;

	extern bool m_noRecoil;
	extern bool m_noRecoil_hooked;

	extern Unity::il2cppArray<Unity::CGameObject*>* players;

	void PlayerListUpdate();
}