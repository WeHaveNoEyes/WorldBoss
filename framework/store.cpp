#include "pch-il2cpp.h"
#include "store.h"
#include "util.h"

namespace Store {
	bool ejected = false;

	int ScreenH = 1;
	int ScreenW = 1;

	Unity::CCamera* worldCamera;
	Player localPlayer;

	Player playerList[16];
	clock_t lastPlayerListUpdate;

	// Menu Features
	bool m_show = false;

	bool m_espLines = true;
	bool m_espNames = true;
	bool m_espBoxes = true;
	bool m_espDistance = true;
	bool m_espDeadPlayers = true;

	bool m_aim = true;
	bool m_aimFovDisplay = true;
	int m_aimFov = 25;

	bool m_noRecoil = false;
	bool m_noRecoil_hooked = false;

	Unity::il2cppArray<Unity::CGameObject*>* players;

	void PlayerListUpdate()
	{
		// Don't run every frame, time in seconds
		if (Util::debouncedRun(&Store::lastPlayerListUpdate, 1)) {
			Unity::il2cppArray<Unity::CGameObject*>* players = Unity::GameObject::FindWithTag("Player");

			// Loop over our players, creating our internal classes
			for (int i = 0; i < players->m_uMaxLength; i++)
			{
				Unity::CGameObject* player = players->At(i);

				// If the player has changed, update
				if (playerList[i].gameObject != player)
				{
					playerList[i] = Player(player);
				}

				if (playerList[i].local) {
					localPlayer = playerList[i];
					worldCamera = playerList[i].getWorldCamera();
					ScreenW = worldCamera->GetMemberValue<int>("pixelWidth");
					ScreenH = worldCamera->GetMemberValue<int>("pixelHeight");
				}
			}
		}
	}
}
