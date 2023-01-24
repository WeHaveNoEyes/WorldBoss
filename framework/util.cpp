#include "pch-il2cpp.h"
#include "util.h"
#include "store.h"

namespace Util
{
	double distance(Unity::Vector3 vec1, Unity::Vector3 vec2)
	{
		return std::sqrt(std::pow((vec1.x - vec2.x), 2) + std::pow((vec1.y - vec2.y), 2) + std::pow((vec1.z - vec2.z), 2));
	}

	float screenDistance(Unity::Vector3 p1, Unity::Vector3 p2)
	{
		float diffY = p1.y - p2.y;
		float diffX = p1.x - p2.x;
		return sqrt((diffY * diffY) + (diffX * diffX));
	}


	float calcTargetPos(float pos, int screenDirSize)
	{
		float aimSpeed = 5.f;
		float center = screenDirSize / 2;
		float targetPos = 0.f;

		// Addition - Move Right
		if (pos > center)
		{
			targetPos = (pos - center) / aimSpeed;
		}

		// Negative - Move Left
		if (pos < center)
		{
			targetPos = -((center - pos) / aimSpeed);
		}

		return targetPos;
	}

	Unity::Vector2 aimAtPoint(float x, float y)
	{
		float targetX = calcTargetPos(x, Store::ScreenW);
		float targetY = calcTargetPos(y, Store::ScreenH);

		return { targetX, targetY };
	}

	bool debouncedRun(clock_t* lastUpdate, float interval) {
		if ((clock() - *lastUpdate) > (interval * 1000)) {
			*lastUpdate = clock();
			return true;
		}

		return false;
	}

	void yeetAnti()
	{
		std::cout << "Cleaning up anticheat garbage..." << std::endl;

		Unity::CGameObject* anti = Unity::GameObject::Find("Anti-Cheat Toolkit");
		if (anti)
		{
			std::cout << "Found..." << std::endl;
			anti->Destroy();
			std::cout << "Anticheat Destroyed!" << std::endl;
		}
	}

	bool worldToScreen(Unity::Vector3 position, Unity::Vector3* screenPos)
	{
		Unity::Vector3 iScreenPos;
		Store::worldCamera->WorldToScreen(position, iScreenPos, 2);

		if (iScreenPos.z < 0)
			return false;

		if (iScreenPos.x < 0)
			return false;

		if (iScreenPos.x > Store::ScreenW)
			return false;

		if (iScreenPos.y < 0)
			return false;

		if (iScreenPos.y > Store::ScreenH)
			return false;

		screenPos->x = iScreenPos.x;
		screenPos->y = iScreenPos.y;
		screenPos->z = iScreenPos.z;

		return true;
	}

	Unity::CComponent* findBone(Unity::il2cppArray<Unity::CComponent*>* bones, std::string search)
	{
		Unity::CComponent* foundBone{};

		for (int i = 0; i < bones->m_uMaxLength; i++)
		{
			Unity::CComponent* bone = bones->At(i);
			if (bone)
			{
				Unity::System_String* boneName = bone->GetName();

				if (boneName->ToString().find("Head_") != std::string::npos)
				{
					foundBone = bone;
					break;
				}
			}
		}

		return foundBone;
	}
}
