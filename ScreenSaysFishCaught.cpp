// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// A UI screen which is just text that pops up when a fish is caught
// The title changes depending om which fish was caught and whether
// or not all of the other fish have been caught - Rebecca Morris

#include "ScreenSaysFishCaught.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

ScreenSaysFishCaught::ScreenSaysFishCaught(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);

	char caughtFishType = mGame->GetCaughtFishType();

	switch (caughtFishType)
	{
		case 'r':
			SetTitle("RedFishTitle");
			break;
		case 'o':
			SetTitle("OrangeFishTitle");
			break;
		case 'y':
			SetTitle("YellowFishTitle");
			break;
		case 'g':
			SetTitle("GreenFishTitle");
			break;
		case 'b':
			SetTitle("BlueFishTitle");
			break;
		case 'p':
			SetTitle("PurpleFishTitle");
			break;
		case 'k':
			SetTitle("PinkFishTitle");
			break;
		case 's':
			SetTitle("SpottedFishTitle");
			break;
		default:
			SetTitle("CatchTitle");
	}

	if (mGame->CheckIfAllFishCaught())
	{
		SetTitle("AllFishCaught");
		AddButton("ResumeButton", [this]() {
			Close();
			});
		AddButton("RestartButton", [this]() {
			new DialogBox(mGame, "RestartText",
			[this]() {
					bool success = mGame->Restart();
				});
			});
		AddButton("QuitButton", [this]() {
			new DialogBox(mGame, "QuitText",
			[this]() {
					mGame->SetState(Game::EQuit);
				});
			});
	}

	else
	{
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
	
}

ScreenSaysFishCaught::~ScreenSaysFishCaught()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void ScreenSaysFishCaught::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
