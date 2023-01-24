#include "pch-il2cpp.h"
#include "user.h"

#include "../framework/gamehook.h"

namespace User
{
	// Runs every frame before ImGui Render - Do all your drawing here
	void Tick()
	{
		Store::players = Unity::GameObject::FindWithTag("Player");
		Store::PlayerListUpdate();

		Watermark();
		DrawESP();
		DrawAim();

		GameHook::ToggleHooks();
	}

	void Menu()
	{
		if (Store::m_show)
		{
			ImGui::SetNextWindowSize(ImVec2(300.000f, 400.000f), ImGuiCond_Once);
			ImGui::Begin("NoEyes", NULL, 7);

			ImGui::Checkbox("No Recoil", &Store::m_noRecoil);

			ImGui::Checkbox("ESP Lines", &Store::m_espLines);
			ImGui::Checkbox("ESP Names", &Store::m_espNames);
			ImGui::Checkbox("ESP Boxes", &Store::m_espBoxes);
			ImGui::Checkbox("ESP Distance", &Store::m_espDistance);
			ImGui::Checkbox("ESP Dead Players", &Store::m_espDeadPlayers);

			ImGui::Checkbox("Enable Aim", &Store::m_aim);
			ImGui::Checkbox("Display FOV", &Store::m_aimFovDisplay);
			ImGui::SliderInt("Aim FOV", &Store::m_aimFov, 0, 1000);

			ImGui::End();
		}
	}

	void Watermark()
	{
		uintptr_t playerCount = Store::players->m_uMaxLength;
		std::string debugMessage = "[WorldBoss Champion Loaded] Players: " + std::to_string(Store::players->m_uMaxLength);
		ImGui::GetBackgroundDrawList()->AddText({ 0, 0 }, ImColor(1.f, 1.f, 1.f), debugMessage.c_str());
	}

	void DrawAim()
	{
		if (Store::localPlayer.initialized && Store::m_aim)
		{
			if (Store::m_aimFovDisplay)
			{
				ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Store::ScreenW / 2, Store::ScreenH / 2), Store::m_aimFov * 1.f, ImColor(1.f, 0.f, 0.f), 74);

				double nearestDistance = 99999.f;
				Unity::Vector3 nearestHeadScreenPos = { -1.f, -1.f, -1.f };

				Unity::Vector3 screenCenter;
				screenCenter.x = Store::ScreenW / 2;
				screenCenter.y = Store::ScreenH / 2;

				Unity::Vector3 localPos = Store::localPlayer.position();

				for (Player player : Store::playerList)
				{
					if (!player.initialized || player.local || player.isDead())
						continue;

					Unity::CComponent* head = player.getBone("head");
					Unity::Vector3 headPos = head->GetTransform()->GetPosition();

					Unity::Vector3 screenPos;
					if (Util::worldToScreen(headPos, &screenPos)) {

						double distance = player.distance(localPos);
						if (distance < nearestDistance)
						{
							float screenDistance = Util::screenDistance(screenPos, screenCenter);

							if (screenDistance <= Store::m_aimFov)
							{
								nearestDistance = distance;
								nearestHeadScreenPos = screenPos;
							}
						}
					}
				}

				if (nearestHeadScreenPos.x > 0)
				{
					// We have a target cuh
					if (GetAsyncKeyState(VK_XBUTTON2))
					{
						Unity::Vector2 moveToPoint = Util::aimAtPoint(nearestHeadScreenPos.x, nearestHeadScreenPos.y);
						mouse_event(MOUSEEVENTF_MOVE, (DWORD)moveToPoint.x, (DWORD)-(moveToPoint.y), NULL, NULL);
					}
				}
			}
		}
	}

	void DrawESP()
	{
		// Do nothing until we have a local player...
		if (Store::localPlayer.initialized)
		{
			Unity::Vector3 localPosition = Store::localPlayer.position();

			for (Player player : Store::playerList)
			{
				// Do nothing for the local player
				if (!player.initialized || player.local)
					continue;

				Unity::Vector3 playerPos = player.position();

				Unity::Vector3 screenPos;
				if (Util::worldToScreen(playerPos, &screenPos)) {
					// Get distance from me
					double distance = player.distance(Store::localPlayer.position());

					bool* dead = player.isDead();

					float yModifier = 0.f;

					if (Store::m_espDistance)
					{
						std::string distanceString = "[" + std::to_string((int)distance) + "m]";
						ImVec2 distanceStringSize = ImGui::CalcTextSize(distanceString.c_str());

						yModifier += distanceStringSize.y;

						if (Store::m_espDeadPlayers)
						{
							if (dead)
							{
								ImGui::GetBackgroundDrawList()->AddText({ screenPos.x - (distanceStringSize.x / 2), Store::ScreenH - screenPos.y }, ImColor(1.f, 0.f, 0.f), distanceString.c_str());
							}
							else {
								ImGui::GetBackgroundDrawList()->AddText({ screenPos.x - (distanceStringSize.x / 2), Store::ScreenH - screenPos.y }, ImColor(1.f, 1.f, 1.f), distanceString.c_str());
							}

							if (!dead)
								ImGui::GetBackgroundDrawList()->AddText({ screenPos.x - (distanceStringSize.x / 2), Store::ScreenH - screenPos.y }, ImColor(1.f, 1.f, 1.f), distanceString.c_str());
						}
						else {
							if (!dead)
								ImGui::GetBackgroundDrawList()->AddText({ screenPos.x - (distanceStringSize.x / 2), Store::ScreenH - screenPos.y }, ImColor(1.f, 1.f, 1.f), distanceString.c_str());
						}
					}

					if (Store::m_espNames)
					{
						std::string nameString = player.name();
						ImVec2 nameStringSize = ImGui::CalcTextSize(nameString.c_str());

						ImGui::GetBackgroundDrawList()->AddText({ screenPos.x - (nameStringSize.x / 2), Store::ScreenH - screenPos.y + yModifier }, ImColor(1.f, 1.f, 1.f), nameString.c_str());

						yModifier += nameStringSize.y;
					}

					if (Store::m_espBoxes)
					{
						BoxCoords bounds = player.calcBounds();
						ImGui::GetBackgroundDrawList()->AddRect(ImVec2(bounds.topLeft.x, bounds.topLeft.y + (20 / distance)), ImVec2(bounds.bottomRight.x, bounds.bottomRight.y), ImColor(1.f, 1.f, 1.f));
					}

					if (!dead)
					{
						if (Store::m_espLines)
						{
							ImGui::GetBackgroundDrawList()->AddLine({ (float)Store::ScreenW / 2, (float)Store::ScreenH }, { screenPos.x, Store::ScreenH - screenPos.y + yModifier }, ImColor(1.f, 1.f, 1.f), 1.f);
						}
					}
				}
			}
		}
	}
}
