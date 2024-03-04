// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "ScreenSaysFishOff.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include <SDL/SDL.h>

ScreenSaysFishOff::ScreenSaysFishOff(Game* game)
	:UIScreen(game)
{
	//mGame->SetState(Game::EPaused);
	//SetRelativeMouseMode(false);
	//startTime = game->GetTime();
	TimePassed = 0;
	SetTitle("FishOffTitle");
}

ScreenSaysFishOff::~ScreenSaysFishOff()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void ScreenSaysFishOff::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	TimePassed += deltaTime;

	if (TimePassed >= 1.25f)
		Close();

}
