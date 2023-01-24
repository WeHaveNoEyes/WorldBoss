#pragma once

#include <iostream>
#include "../framework/store.h"
#include "../framework/player.h"
#include "../dependencies/resolver/Resolver.hpp"
#include "../dependencies/imgui/imgui.h"
#include "../framework/util.h"

namespace User
{
	void Tick();
	void Menu();

	void Watermark();
	void DrawESP();
	void DrawAim();
}


