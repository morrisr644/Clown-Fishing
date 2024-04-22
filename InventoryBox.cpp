// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// UI Screen which displays the dialogue boxes for the different fish - Rebecca Morris

#include "InventoryBox.h"
#include "Game.h"
#include "Renderer.h"
#include <SDL/SDL.h>

InventoryBox::InventoryBox(Game* game, const std::string& text, int fish)
	:UIScreen(game)
{
	//Adjust for screen size
	float height = mGame->GetWindowHeight();

	// Adjust positions for dialog box
	mBGPos = Vector2(0.0f, 0.0f);
	mTitlePos = Vector2(0.0f, ((height * 5) / 44));
	mNextButtonPos = Vector2(0.0f, -((height * 5) / 44));

	switch (fish)
	{
		case 0: //Red Fish
			mBackground = mGame->GetRenderer()->GetTexture("Assets/RedFishDialogBG.png");
			break;
		case 1:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/YellowFishDialogBG.png");
			break;
		case 2:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/OrangeFishDialogBG.png");
			break;
		case 3:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/GreenFishDialogBG.png");
			break;
		case 4:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/BlueFishDialogBG.png");
			break;
		case 5:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/PurpleFishDialogBG.png");
			break;
		case 6:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/PinkFishDialogBG.png");
			break;
		case 7:
			mBackground = mGame->GetRenderer()->GetTexture("Assets/SpottedFishDialogBG.png");
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
