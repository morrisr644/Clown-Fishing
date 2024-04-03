// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

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

	mBGPos = Vector2(0.0f, 0.0f);
	mTitlePos = Vector2(0.0f, 155.0f);
	mNextButtonPos = Vector2(0.0f, -175.0f);

	SetTitle("ControlText", Color::Black, 40);
	AddButton("OKButton", [this]()
		{
			Close();
		});

	mBackground = game->GetRenderer()->GetTexture("Assets/ControlsBG.png");
	//mBGPos = Vector2(-45.0f, 145.0f);
}

ControlsMenu::~ControlsMenu()
{
}
