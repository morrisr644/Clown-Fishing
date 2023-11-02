// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "CatchScreen.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

CatchScreen::CatchScreen(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);

	int caughtFishType = mGame->GetCaughtFishType();

	switch (caughtFishType)
	{
		case 1:
			SetTitle("RedFishTitle");
			break;
		case 2:
			SetTitle("YellowFishTitle");
			break;
		default:
			SetTitle("CatchTitle");
	}

	//SetTitle("CatchTitle");

	AddButton("ResumeButton", [this]() {
		Close();
	});
	AddButton("QuitButton", [this]() { 
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::EQuit);
		});
	});
}

CatchScreen::~CatchScreen()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void CatchScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
