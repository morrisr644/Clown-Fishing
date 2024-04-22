// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// UI Screen that pops up for showing controls - Rebecca Morris

#include "ControlsMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include "InventoryMenu.h"
#include "MainMenu.h"
#include "Renderer.h"
#include <SDL/SDL.h>

ControlsMenu::ControlsMenu(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);

	//Adjust for screen size
	float height = mGame->GetWindowHeight();

	mBGPos = Vector2(0.0f, 0.0f);
	mTitlePos = Vector2(0.0f, ((height * 2) / 11));
	mNextButtonPos = Vector2(0.0f, -((height * 165) / 880));

	SetTitle("ControlText", Color::Black, 40);
	AddButton("OKButton", [this]()
		{
			Close();
		});

	mBackground = game->GetRenderer()->GetTexture("Assets/ControlsBG.png");
}

ControlsMenu::~ControlsMenu()
{
}
