#pragma once
//#include "Store.h"
#include <Windows.h>
#include <iostream>
#include "../dependencies/resolver/Resolver.hpp"

namespace Util {

	double distance(Unity::Vector3 vec1, Unity::Vector3 vec2);

	float screenDistance(Unity::Vector3 p1, Unity::Vector3 p2);

	float calcTargetPos(float pos, int screenDirSize);

	Unity::Vector2 aimAtPoint(float x, float y);

	bool debouncedRun(clock_t* lastUpdate, float interval);

	void yeetAnti();

	bool worldToScreen(Unity::Vector3 position, Unity::Vector3* screenPos);

	Unity::CComponent* findBone(Unity::il2cppArray<Unity::CComponent*>* bones, std::string search);
}