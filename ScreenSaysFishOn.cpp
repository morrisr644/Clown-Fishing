// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// UI Screen which is just temporary text that says a fish has gotten on the line - Rebecca Morris

#include "ScreenSaysFishOn.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include <SDL/SDL.h>

ScreenSaysFishOn::ScreenSaysFishOn(Game* game)
	:UIScreen(game)
{
	TimePassed = 0;
	SetTitle("FishOnTitle");
}

ScreenSaysFishOn::~ScreenSaysFishOn()
{
	SetRelativeMouseMode(true);
}

void ScreenSaysFishOn::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	TimePassed += deltaTime; // After a certain amount of time, remove the text - Rebecca Morris

	if (TimePassed >= 0.75f)
		Close();

}
