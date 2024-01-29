// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "TensionMeter.h"
#include "Game.h"
#include "Renderer.h"
#include "DialogBox.h"
#include "HUD.h"
#include <SDL/SDL.h>

TensionMeter::TensionMeter(Game* game)
	:UIScreen(game)
{
	//mGame->SetState(Game::EPaused);
	//SetRelativeMouseMode(false);
	//startTime = game->GetTime();
	Renderer* r = mGame->GetRenderer();

	TensionLevel = 1;
	mTensionBar = r->GetTexture("Assets/BlankBar.png");
	//SetTitle("FishOnTitle");
}

TensionMeter::~TensionMeter()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void TensionMeter::Draw(Shader* shader)
{
	DrawTexture(shader, mTensionBar, Vector2(-350.0f, -350.0f));
}

void TensionMeter::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	//TensionLevel = GetTensionLevel();
	Renderer* r = mGame->GetRenderer();

	if (mGame->isReelingIn)
	{
		switch (TensionLevel)
		{
		case 1:
			mTensionBar = r->GetTexture("Assets/HealthBar.png");
			break;
		case 2:
			mTensionBar = r->GetTexture("Assets/TensionLevel2.png");
			break;
		case 3:
			mTensionBar = r->GetTexture("Assets/TensionLevel3.png");
			break;
		case 4:
			mTensionBar = r->GetTexture("Assets/TensionLevel4.png");
			break;
		case 5:
			mTensionBar = r->GetTexture("Assets/TensionLevel5.png");
			break;
		case 6:
			mTensionBar = r->GetTexture("Assets/TensionLevel6.png");
			break;
		case 7:
			mTensionBar = r->GetTexture("Assets/TensionLevel7.png");
			break;
		case 8:
			mTensionBar = r->GetTexture("Assets/TensionLevel8.png");
			break;
		}
	}
	else
		mTensionBar = r->GetTexture("Assets/BlankBar.png");

}

