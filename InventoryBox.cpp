// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InventoryBox.h"
#include "Game.h"
#include "Renderer.h"
#include <SDL/SDL.h>

InventoryBox::InventoryBox(Game* game, const std::string& text, int fish)
	:UIScreen(game)
{
	// Adjust positions for dialog box
	mBGPos = Vector2(0.0f, 0.0f);
	mTitlePos = Vector2(0.0f, 100.0f);
	mNextButtonPos = Vector2(0.0f, -100.0f);

	switch (fish)
	{
		case 0: //Red Fish
			mBackground = mGame->GetRenderer()->GetTexture("Assets/RedFishDialogBG.png");
			break;
		case 1:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/YellowFishDialogBG.png");
			break;
		default:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/DialogBG.png");
			break;
	}


	SetTitle(text, Vector3::Zero, 30);
	AddButton("OKButton", [this]() {
		Close();
	});
}

InventoryBox::~InventoryBox()
{
}
