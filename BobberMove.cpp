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
#include "WaterPlaneActor.h"
#include "FencePlaneActor.h"
#include "InvisiblePlaneActor.h"
#include "FPSActor.h"
#include "RodActor.h"

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
	constexpr float LEFTWALL = 1450.0;
	constexpr float RIGHTWALL = -1450.0;
	constexpr float FORWARDWALL = 1500.0;
	constexpr float BACKWALL = -1500.0;
	constexpr float PONDWALL = 250.0;
	constexpr float WATERLEVEL = -100.0;

	// Create line segment
	LineSegment l(start, end);

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	BobberActor* bobber = mOwner->GetGame()->GetBobber();
	Vector3 bobberPosition = bobber->GetPosition();

	/*if (bobberPosition.z <= WATERLEVEL && bobberPosition.y < RIGHTWALL
		&& bobberPosition.y > LEFTWALL && bobberPosition.x < RIGHTWALL && bobberPosition.x > LEFTWALL)
	{
		bobber->PutInWater();
		bobber->HitGround();
	}*/

	// If the bobber hits something
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{

		
		FPSActor* player = mOwner->GetGame()->GetPlayer();
		BasicFish* yFish = mOwner->GetGame()->GetYellowFish();
		BasicFish* rFish = mOwner->GetGame()->GetRedFish();

		//InvisiblePlaneActor* invisWall = dynamic_cast<InvisiblePlaneActor*>(info.mActor);

		//if (!invisWall)
		//{
		//	if (rFish || yFish)
		//	{
		//		//do nothing
		//	}
		//	else
		//	{
		//		static_cast<BobberActor*>(mOwner)->HitGround();
		//	}
		//	
		//}
		//WaterPlaneActor* water = dynamic_cast<WaterPlaneActor*>(info.mActor);

		RodActor* rod = dynamic_cast<RodActor*>(info.mActor);

		if (bobberPosition.z <= WATERLEVEL + 25.0f && ((bobberPosition.x >= RIGHTWALL
			&& bobberPosition.x < LEFTWALL) && (bobberPosition.y < FORWARDWALL
			&& bobberPosition.y >= PONDWALL)))
		{
			bobber->PutInWater();
			bobber->HitGround();
		}
		else if (bobberPosition.z > WATERLEVEL && (bobberPosition.y > FORWARDWALL - 25.0f || bobberPosition.y < BACKWALL + 25.0f
			|| bobberPosition.x > LEFTWALL || bobberPosition.x < RIGHTWALL))
		{
			dir = Vector3::Reflect(dir, info.mNormal);
			bobber->RotateToNewForward(dir);
		}
		else if (!rod)
		{
			//bobber->OutOfWater();
			bobber->HitGround();
		}

		/*if (water)
		{
			bobber->PutInWater();
			bobber->HitGround();
		}*/
		

		//FencePlaneActor* fence = dynamic_cast<FencePlaneActor*>(info.mActor);

		//if (fence)
		//{
		//	dir = Vector3::Reflect(dir, info.mNormal);
		//	bobber->RotateToNewForward(dir);
		//}
		//else if (!invisWall && !rod)
		//{
		//	//bobber->OutOfWater();
		//	bobber->HitGround();
		//}

	}
	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
