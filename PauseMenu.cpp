// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
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

	mBackground = game->GetRenderer()->GetTexture("Assets/PauseDialogBG.png");
	mBGPos = Vector2(-45.0f, 145.0f);
	/*AddButton("InventoryButton", [this]() {
		new InventoryMenu(mGame);
		});*/
	AddButton("MenuButton", [this]() {
		//new InventoryMenu(mGame);
		});
	AddButton("RestartButton", [this]() {
		new DialogBox(mGame, "RestartText",
		[this]() {
				//mGame->Shutdown();
				//mGame->Initialize();
				//mGame->SetState(Game::EQuit);
				//mGame->RunLoop();
				bool success = mGame->Restart();
				/*if (success)
				{
					mGame->RunLoop();
				}*/
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
