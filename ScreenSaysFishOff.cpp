// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// A UI Screen which is just text which says that the fish has gotten off the line - Rebecca Morris

#include "ScreenSaysFishOff.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include <SDL/SDL.h>

ScreenSaysFishOff::ScreenSaysFishOff(Game* game)
	:UIScreen(game)
{
	TimePassed = 0;
	SetTitle("FishOffTitle");
}

ScreenSaysFishOff::~ScreenSaysFishOff()
{
	SetRelativeMouseMode(true);
}

void ScreenSaysFishOff::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	TimePassed += deltaTime;

	if (TimePassed >= 1.25f)
		Close();

}
