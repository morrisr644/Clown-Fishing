// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FishOffScreen.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include <SDL/SDL.h>

FishOffScreen::FishOffScreen(Game* game)
	:UIScreen(game)
{
	//mGame->SetState(Game::EPaused);
	//SetRelativeMouseMode(false);
	//startTime = game->GetTime();
	TimePassed = 0;
	SetTitle("FishOffTitle");
}

FishOffScreen::~FishOffScreen()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void FishOffScreen::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	TimePassed += deltaTime;

	if (TimePassed >= 1.25f)
		Close();

}
