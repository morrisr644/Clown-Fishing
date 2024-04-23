// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// UI screen which appears when the game is paused
// Gives the player the option to resume, restart, or quit the game - Rebecca Morris

#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include "MainMenu.h"
#include "Renderer.h"
#include <SDL/SDL.h>

PauseMenu::PauseMenu(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	SetTitle("PauseTitle");
	AddButton("ResumeButton", [this]() {
		Close();
	});

	float width = mGame->GetWindowWidth(); // Get the width of the player's screen - Rebecca Morris
	float height = mGame->GetWindowHeight(); // Get the height of the player's screen

	mBackground = game->GetRenderer()->GetTexture("Assets/PauseDialogBG.png");
	mBGPos = Vector2(-((width * 9) / 320), ((height * 29) / 176));
	AddButton("MenuButton", [this]() {
		mGame->PopUI(this);
		new MainMenu(mGame);
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

PauseMenu::~PauseMenu()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
