// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MainMenu.h"
#include "PauseMenu.h"
#include "ControlsMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include "Renderer.h"
#include <SDL/SDL.h>

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	mState = EActive();
	this->mNextButtonPos.Set(300.0f, 150.0f);
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	//SetTitle("PauseTitle");

	mGame->PauseMusic();
	mBackground = game->GetRenderer()->GetTexture("Assets/SmallMainMenu.jpg");
	mBGPos = Vector2(0.0f, 0.0f);
	AddButton("StartOrResumeButton", [this]() {
		Close();
		});
	AddButton("RestartButton", [this]() {
		new DialogBox(mGame, "RestartText",
		[this]() {
				bool success = mGame->Restart();
			});
		});
	AddButton("ControlText", [this]() {
		new ControlsMenu(mGame);
		});
	AddButton("QuitButton", [this]() { 
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::EQuit);
		});
	});
}

MainMenu::~MainMenu()
{
	mGame->PauseMusic();
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void MainMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
