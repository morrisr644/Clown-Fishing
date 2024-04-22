// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Handles the elements that appear on the screen without pausing gameplay - Rebecca Morris

#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FPSActor.h"
#include <algorithm>
#include "TargetComponent.h"
#include "BasicFish.h"
#include "BobberActor.h"
#include <string.h>

HUD::HUD(Game* game)
	:UIScreen(game)
	,mRadarRange(2000.0f)
	,mRadarRadius(92.0f)
	,mTargetEnemy(false)
{
	float height = mGame->GetWindowHeight();
	mRadarRadius = (height * 23) / 220;

	Renderer* r = mGame->GetRenderer();
	mBlankTensionBar = r->GetTexture("Assets/BlankBar.png"); // All of the different levels for the tension bar - Rebecca Morris
	mTensionBar1 = r->GetTexture("Assets/HealthBar.png");
	mTensionBar2 = r->GetTexture("Assets/TensionLevel2.png");
	mTensionBar3 = r->GetTexture("Assets/TensionLevel3.png");
	mTensionBar4 = r->GetTexture("Assets/TensionLevel4.png");
	mTensionBar5 = r->GetTexture("Assets/TensionLevel5.png");
	mTensionBar6 = r->GetTexture("Assets/TensionLevel6.png");
	mTensionBar7 = r->GetTexture("Assets/TensionLevel7.png");
	mTensionBar8 = r->GetTexture("Assets/TensionLevel8.png");

	mRadar = r->GetTexture("Assets/Radar.png");
	mCrosshair = r->GetTexture("Assets/Crosshair.png");
	mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	mBlipTex = r->GetTexture("Assets/Blip.png");
	mBobberBlipTex = r->GetTexture("Assets/BobberBlip.png");
	mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);
	
	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
	UpdateTensionBar(deltaTime);
}

void HUD::Draw(Shader* shader)
{
	// Crosshair
	Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
	DrawTexture(shader, cross, Vector2::Zero, 2.0f);
	
	// Radar
	//Changed after screen was turned fullscreen - Rebecca Morris
	// 
	// The numbers that are not constants were found by taking my original
	// screen size as well as the original sizes of the elements on my screen
	// and taking the ratio of the size to screen to hopefully get it working
	// for all screen sizes.

	float width = mGame->GetWindowWidth();
	float height = mGame->GetWindowHeight();

	const Vector2 cRadarPos(-((width * 3) / 8), ((height * 5) / 16));
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);
	// Blips
	for (Vector2& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}
	// Bobber Blip
	for (Vector2& blip : mBobberBlips)
	{
		DrawTexture(shader, mBobberBlipTex, cRadarPos + blip, 1.0f);
	}
	// Radar arrow
	DrawTexture(shader, mRadarArrow, cRadarPos);

	// Tension bars
	for (Vector2& fullHealth : mTensionBarBlank)
	{
		DrawTexture(shader, mBlankTensionBar, Vector2(150, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel1)
	{
		DrawTexture(shader, mTensionBar1, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel2)
	{
		DrawTexture(shader, mTensionBar2, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel3)
	{
		DrawTexture(shader, mTensionBar3, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel4)
	{
		DrawTexture(shader, mTensionBar4, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel5)
	{
		DrawTexture(shader, mTensionBar5, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel6)
	{
		DrawTexture(shader, mTensionBar6, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel7)
	{
		DrawTexture(shader, mTensionBar7, Vector2(-50, ((height * 5) / 11)));
	}
	for (Vector2& fullHealth : mTensionBarLevel8)
	{
		DrawTexture(shader, mTensionBar8, Vector2(-50, ((height * 5) / 11)));
	}
	
}

void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(),
		tc);
	mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(float deltaTime)
{
	// Reset to regular cursor
	mTargetEnemy = false;
	// Make a line segment
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	// Segment cast
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// Is this a target?
		for (auto tc : mTargetComps)
		{
			if (tc->GetOwner() == info.mActor)
			{
				mTargetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateTensionBar(float deltaTime)
{
	mTensionBarBlank.clear(); // these need to be here or we just get a ton of tension bars - Rebecca Morris
	mTensionBarLevel1.clear();
	mTensionBarLevel2.clear();
	mTensionBarLevel3.clear();
	mTensionBarLevel4.clear();
	mTensionBarLevel5.clear();
	mTensionBarLevel6.clear();
	mTensionBarLevel7.clear();
	mTensionBarLevel8.clear();

	Renderer* r = mGame->GetRenderer();

	//Adjusting for screen size
	float width = mGame->GetWindowWidth();
	float height = mGame->GetWindowHeight();

	if (mGame->isReelingIn)
	{

		// BasicFish
		int TensionLevel = 1; // This is set to 1 as a default

		BasicFish* currentFish;

		char fishType = mGame->GetCurrentFishType();

		currentFish = mGame->GetCurrentFish();



		BobberActor* bobber = mGame->GetBobber();
		float totalDistance = mGame->GetFishHookDistance(); //Gets smaller as space is pressed and larger as space is not pressed - Rebecca Morris 
		float fishDistance = currentFish->GetFishDistance(); //Gets larger as spaced is pressed (should it get smaller?) - Rebecca Morris

		//The more space is pressed = the smaller the percent becomes - Rebecca Morris
		//The longer space is not pressed = the larger the value gets
		float percent = (totalDistance/600.0) * 10;  



		if (percent >= 8.5f)
		{
			TensionLevel = 8;
		}
		else if (8.5f > percent && percent >= 7.3f)
		{
			TensionLevel = 7;
		}
		else if (7.3f > percent && percent >= 6.1f)
		{
			TensionLevel = 6;
		}
		else if (6.1f > percent && percent >= 4.9f)
		{
			TensionLevel = 5;
		}
		else if (4.9f > percent && percent >= 3.7f)
		{
			TensionLevel = 4;
		}
		else if (3.7f > percent && percent >= 2.5f)
		{
			TensionLevel = 3;
		}
		else if (2.5f > percent && percent >= 1.3f)
		{
			TensionLevel = 2;
		}
		else if (1.3f > percent && percent >= 0.1f)
		{
			TensionLevel = 1;
		}
		
		switch (TensionLevel)
		{
		case 1:
			mTensionBarLevel1.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 2:
			mTensionBarLevel2.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 3:
			mTensionBarLevel3.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 4:
			mTensionBarLevel4.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 5:
			mTensionBarLevel5.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 6:
			mTensionBarLevel6.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 7:
			mTensionBarLevel7.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		case 8:
			mTensionBarLevel8.emplace_back(Vector2(0, ((height * 35) / 88)));
			break;
		}
	}
	else
		mTensionBarBlank.emplace_back(Vector2(0, ((height * 35) / 88)));

}

void HUD::UpdateRadar(float deltaTime)
{
	// Clear blip positions from last frame
	mBlips.clear();
	mBobberBlips.clear();
	
	// Convert player position to radar coordinates (x forward, z up)
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);
	// Ditto for player forward
	Vector3 playerForward = mGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);
	
	// Use atan2 to get rotation of radar
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// Make a 2D rotation matrix
	Matrix3 rotMat = Matrix3::CreateRotation(angle);
	
	// Get positions of blips
	for (auto tc : mTargetComps)
	{

		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);
		
		// Calculate vector between player and target
		Vector2 playerToTarget = actorPos2D - playerPos2D;
		
		// See if within range
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			// Convert playerToTarget into an offset from
			// the center of the on-screen radar
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius/mRadarRange;
			
			// Rotate blipPos
			blipPos = Vector2::Transform(blipPos, rotMat);

			if (tc->GetOwner() == mGame->GetBobber())
				mBobberBlips.emplace_back(blipPos);
			else
				mBlips.emplace_back(blipPos);
		}
	}
}
