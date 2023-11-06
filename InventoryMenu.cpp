// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InventoryMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryBox.h"
#include <SDL/SDL.h>

InventoryMenu::InventoryMenu(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	SetTitle("InventoryTitle");

	//const int FISHTYPES = 2; // This will change as fish are added

	if (game->GetAllCaughtFish(0)) // 0 is Red Fish
	{
		AddButton("RedFishItem", [this]() {
			new InventoryBox(mGame, "RedFishText", 0);
			});
	}

	if (game->GetAllCaughtFish(1)) // 1 is Yellow Fish
	{
		AddButton("YellowFishItem", [this]() {
			new InventoryBox(mGame, "YellowFishText", 1);
			});
	}

	AddButton("ResumeButton", [this]() {
		Close();
		});
}

InventoryMenu::~InventoryMenu()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void InventoryMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE || key == SDLK_e)
	{
		Close();
	}
}
