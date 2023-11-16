// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Previously BallMove.cpp
// Edits by Rebecca Morris

#include "BobberMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "BobberActor.h"
#include "BasicFish.h"
#include "YellowFish.h"
#include "RedFish.h"
#include "WaterPlaneActor.h"
#include "InvisiblePlaneActor.h"
#include "FPSActor.h"

BobberMove::BobberMove(Actor* owner)
	:MoveComponent(owner)
{
}

void BobberMove::Update(float deltaTime)
{
	// Construct segment in direction of travel
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// Create line segment
	LineSegment l(start, end);


	// Test segment vs world

	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	BobberActor* bobber = mOwner->GetGame()->GetBobber();
	Vector3 bobberPosition = bobber->GetPosition();

	if (bobberPosition.z <= -100.0 && bobberPosition.y < 1450.0 && bobberPosition.y > -1450.0 && bobberPosition.x < 1450.0 && bobberPosition.x > -1450.0) 
	{
		bobber->PutInWater();
		bobber->HitGround();
	}

	// If the bobber hits something
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{

		
		FPSActor* player = mOwner->GetGame()->GetPlayer();
		YellowFish* yFish = mOwner->GetGame()->GetYellowFish();
		RedFish* rFish = mOwner->GetGame()->GetRedFish();

		InvisiblePlaneActor* invisWall = dynamic_cast<InvisiblePlaneActor*>(info.mActor);

		if (!invisWall)
		{
			// Make the bobber stop when hitting Ground
			static_cast<BobberActor*>(mOwner)->HitGround();
		}

		BasicFish* fish = dynamic_cast<BasicFish*>(info.mActor);
		YellowFish* yellowFish = dynamic_cast<YellowFish*>(info.mActor);
		RedFish* redFish = dynamic_cast<RedFish*>(info.mActor);
		WaterPlaneActor* water = dynamic_cast<WaterPlaneActor*>(info.mActor);

		


		if (bobberPosition.z <= -100.0 || water)
		{
			bobber->PutInWater();
			bobber->HitGround();
		}
		else
		{
			bobber->OutOfWater();
		}


		// If the bobber hits the fish
		/*if (fish)
		{
			fish->GetOnLine();
			bobber->FishOn();
		}*/

		if (yellowFish && !bobber->GetFishOnStatus())
		{
			yellowFish->GetOnLine();
			bobber->FishOn();
			Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
			bobber->SetPosition(newBobberPosition);
		}
		
		if (yFish->GetLineStatus() && yFish->GetState() == Actor::EActive) // this handles the fishes tension
		{
			// make the fish face bobber instead of the player.
			// get fish to reflect off of wall.
			Vector3 playerPos = player->GetPosition();
			Vector3 bobberPos = bobber->GetPosition();
			Vector3 fishPos = yFish->GetPosition();
			Vector3 bobberOppPlayer = playerPos - bobberPos;
			Vector3 fishOppPlayer = playerPos - fishPos;
			bobberOppPlayer.Normalize();
			fishOppPlayer.Normalize();
			bobberOppPlayer.x = -bobberOppPlayer.x;
			bobberOppPlayer.y = -bobberOppPlayer.y;
			bobberOppPlayer.z = -bobberOppPlayer.z;
			fishOppPlayer.x = fishOppPlayer.x;
			fishOppPlayer.y = fishOppPlayer.y;
			fishOppPlayer.z = fishOppPlayer.z;
			bobber->RotateToNewForward(bobberOppPlayer);
			yFish->RotateToNewForward(fishOppPlayer);
			bobber->SetTensionSpeed(30.0);
			yFish->SetMovementSpeed(-30.0);
		}

		if (redFish && !bobber->GetFishOnStatus())
		{
			redFish->GetOnLine();
			bobber->FishOn();
			Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
			bobber->SetPosition(newBobberPosition);
		}

		if (rFish->GetLineStatus() && rFish->GetState() == Actor::EActive) // this handles the fishes tension
		{
			Vector3 playerPos = player->GetPosition();
			Vector3 bobberPos = bobber->GetPosition();
			Vector3 fishPos = rFish->GetPosition();
			Vector3 bobberOppPlayer = playerPos - bobberPos;
			Vector3 fishOppPlayer = playerPos - fishPos;
			bobberOppPlayer.Normalize();
			fishOppPlayer.Normalize();
			bobberOppPlayer.x = -bobberOppPlayer.x;
			bobberOppPlayer.y = -bobberOppPlayer.y;
			bobberOppPlayer.z = -bobberOppPlayer.z;
			fishOppPlayer.x = fishOppPlayer.x;
			fishOppPlayer.y = fishOppPlayer.y;
			fishOppPlayer.z = fishOppPlayer.z;
			bobber->RotateToNewForward(bobberOppPlayer);
			rFish->RotateToNewForward(fishOppPlayer);
			bobber->SetTensionSpeed(20.0);
			rFish->SetMovementSpeed(-20.0);
		}
	}
		



	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
