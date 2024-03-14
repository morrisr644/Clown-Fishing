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

	if (game->GetAllCaughtFish(2)) // 2 is Orange Fish
	{
		AddButton("OrangeFishItem", [this]() {
			new InventoryBox(mGame, "OrangeFishText", 2);
			});
	}

	if (game->GetAllCaughtFish(1)) // 1 is Yellow Fish
	{
		AddButton("YellowFishItem", [this]() {
			new InventoryBox(mGame, "YellowFishText", 1);
			});
	}

	if (game->GetAllCaughtFish(3)) // 3 is Green Fish
	{
		AddButton("GreenFishItem", [this]() {
			new InventoryBox(mGame, "GreenFishText", 3);
			});
	}

	if (game->GetAllCaughtFish(4)) // 4 is Blue Fish
	{
		AddButton("BlueFishItem", [this]() {
			new InventoryBox(mGame, "BlueFishText", 4);
			});
	}

	if (game->GetAllCaughtFish(5)) // 5 is Purple Fish
	{
		AddButton("PurpleFishItem", [this]() {
			new InventoryBox(mGame, "PurpleFishText", 5);
			});
	}

	if (game->GetAllCaughtFish(6)) // 6 is Pink Fish
	{
		AddButton("PinkFishItem", [this]() {
			new InventoryBox(mGame, "PinkFishText", 6);
			});
	}

	if (game->GetAllCaughtFish(7)) // 7 is Polka-Dot/Spotted Fish
	{
		AddButton("SpottedFishItem", [this]() {
			new InventoryBox(mGame, "SpottedFishText", 7);
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
