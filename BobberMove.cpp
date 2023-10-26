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
#include "WaterPlaneActor.h"

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

	// If the bobber hits something
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{

		BobberActor* bobber = mOwner->GetGame()->GetBobber();

		// Make the bobber stop when hitting Ground
		static_cast<BobberActor*>(mOwner)->HitGround();

		BasicFish* fish = dynamic_cast<BasicFish*>(info.mActor);
		YellowFish* yellowFish = dynamic_cast<YellowFish*>(info.mActor);

		// If the bobber hits the fish
		if (fish)
		{
			fish->GetOnLine();
			bobber->FishOn();
		}
		if (yellowFish)
		{
			yellowFish->GetOnLine();
			bobber->FishOn();
		}
		

		WaterPlaneActor* water = dynamic_cast<WaterPlaneActor*>(info.mActor);

		if (water)
		{
			bobber->PutInWater();
		}

	}
		



	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
