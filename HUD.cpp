// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

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
#include "RedFish.h"
#include "YellowFish.h"
#include "BobberActor.h"

HUD::HUD(Game* game)
	:UIScreen(game)
	,mRadarRange(2000.0f)
	,mRadarRadius(92.0f)
	,mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mBlankTensionBar = r->GetTexture("Assets/BlankBar.png");
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
	const Vector2 cRadarPos(-390.0f, 275.0f);
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);
	// Blips
	for (Vector2& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}
	// Radar arrow
	DrawTexture(shader, mRadarArrow, cRadarPos);

	// Tension bars
	for (Vector2& fullHealth : mTensionBarBlank)
	{
		DrawTexture(shader, mBlankTensionBar, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel1)
	{
		DrawTexture(shader, mTensionBar1, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel2)
	{
		DrawTexture(shader, mTensionBar2, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel3)
	{
		DrawTexture(shader, mTensionBar3, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel4)
	{
		DrawTexture(shader, mTensionBar4, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel5)
	{
		DrawTexture(shader, mTensionBar5, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel6)
	{
		DrawTexture(shader, mTensionBar6, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel7)
	{
		DrawTexture(shader, mTensionBar7, Vector2(0, 350.0f));
	}
	for (Vector2& fullHealth : mTensionBarLevel8)
	{
		DrawTexture(shader, mTensionBar8, Vector2(0, 350.0f));
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
	mTensionBarBlank.clear(); // these need tobe here or we just get a ton of health bars
	mTensionBarLevel1.clear();
	mTensionBarLevel2.clear();
	mTensionBarLevel3.clear();
	mTensionBarLevel4.clear();
	mTensionBarLevel5.clear();
	mTensionBarLevel6.clear();
	mTensionBarLevel7.clear();
	mTensionBarLevel8.clear();

	Renderer* r = mGame->GetRenderer();

	if (mGame->isReelingIn)
	{
		// Tension bar
		//DrawTexture(shader, mTensionBar, Vector2(-350.0f, -350.0f));
		//mTensionBar = r->GetTexture("Assets/HealthBar.png");


		// BasicFish
		int TensionLevel = 1;

		auto* currentFish = mGame->GetBasicFish();

		int fishType = mGame->GetCurrentFishType();

		switch (fishType)
		{
			case 0:
				currentFish = mGame->GetBasicFish();
				break;
			case 1:
				currentFish = mGame->GetRedFish();
				break;
			case 2:
				currentFish = mGame->GetYellowFish();
				break;
		}

		BobberActor* bobber = mGame->GetBobber();

		float totalDistance = bobber->getTotalDistance();
		float fishDistance = currentFish->GetFishDistance();
		//float fishDistance = mGame->GetCurrentFishDistance(); //This would only update when space is pressed

		float percent = (fishDistance/totalDistance) * 10;  //Why isnt this working
		//float percent = totalDistance - fishDistance;

		if (percent >= 8.5f)
		{
			TensionLevel = 1;
		}
		else if (8.5f > percent && percent >= 7.3f)
		{
			TensionLevel = 2;
		}
		else if (7.3f > percent && percent >= 6.1f)
		{
			TensionLevel = 3;
		}
		else if (6.1f > percent && percent >= 4.9f)
		{
			TensionLevel = 4;
		}
		else if (4.9f > percent && percent >= 3.7f)
		{
			TensionLevel = 5;
		}
		else if (3.7f > percent && percent >= 2.5f)
		{
			TensionLevel = 6;
		}
		else if (2.5f > percent && percent >= 1.3f)
		{
			TensionLevel = 7;
		}
		else if (1.3f > percent && percent >= 0.1f)
		{
			TensionLevel = 8;
		}

		//if (percent >= 15.0f)
		//{
		//	TensionLevel = 1;
		//}
		//else if (14.9f >= percent && percent >= 12.8f)
		//{
		//	TensionLevel = 2;
		//}
		//else if (12.7f >= percent && percent >= 10.6f)
		//{
		//	TensionLevel = 3;
		//}
		//else if (10.5f >= percent && percent >= 8.4f)
		//{
		//	TensionLevel = 4;
		//}
		//else if (8.3f >= percent && percent >= 6.2f)
		//{
		//	TensionLevel = 5;
		//}
		//else if (6.1f >= percent && percent >= 4.1f)
		//{
		//	TensionLevel = 6;
		//}
		//else if (4.0f >= percent && percent >= 2.0f)
		//{
		//	TensionLevel = 7;
		//}
		//else //if (1.2f >= percent && percent >= 0.1)
		//{
		//	TensionLevel = 8;
		//}


		switch (TensionLevel)
		{
		case 1:
			mTensionBarLevel1.emplace_back(Vector2(0, 350.0f));
			break;
		case 2:
			mTensionBarLevel2.emplace_back(Vector2(0, 350.0f));
			break;
		case 3:
			mTensionBarLevel3.emplace_back(Vector2(0, 350.0f));
			break;
		case 4:
			mTensionBarLevel4.emplace_back(Vector2(0, 350.0f));
			break;
		case 5:
			mTensionBarLevel5.emplace_back(Vector2(0, 350.0f));
			break;
		case 6:
			mTensionBarLevel6.emplace_back(Vector2(0, 350.0f));
			break;
		case 7:
			mTensionBarLevel7.emplace_back(Vector2(0, 350.0f));
			break;
		case 8:
			mTensionBarLevel8.emplace_back(Vector2(0, 350.0f));
			break;
		}
	}
	else
		mTensionBarBlank.emplace_back(Vector2(0, 350.0f));

}

void HUD::UpdateRadar(float deltaTime)
{
	// Clear blip positions from last frame
	mBlips.clear();
	
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
			mBlips.emplace_back(blipPos);
		}
	}
}
