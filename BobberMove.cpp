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
#include "BobberActor.h"
#include "BasicFish.h"
#include "FPSActor.h"
#include "RodActor.h"

BobberMove::BobberMove(Actor* owner)
	:MoveComponent(owner)
{
}

void BobberMove::Update(float deltaTime)
{
	// Construct segment in direction of travel
	// This class is here to handle the bobbers position and how that position affects the game
	// If the the bobber is in the water, we call the function PutInWater, if it hits the walls we bounce it off the walls
	// If the bobber hits the grass we stop it.  Adam Caligiuri
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;
	constexpr float LEFTWALL = 1450.0;
	constexpr float RIGHTWALL = -1450.0;
	constexpr float FORWARDWALL = 1475.0;
	constexpr float BACKWALL = -1475.0;
	constexpr float PONDWALL = 250.0;
	constexpr float WATERLEVEL = -100.0;

	// Create line segment
	LineSegment l(start, end);

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	BobberActor* bobber = mOwner->GetGame()->GetBobber();
	Vector3 bobberPosition = bobber->GetPosition();

	// If the bobber hits something
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{

		
		FPSActor* player = mOwner->GetGame()->GetPlayer();
		BasicFish* yFish = mOwner->GetGame()->GetYellowFish();
		BasicFish* rFish = mOwner->GetGame()->GetRedFish();

		//Changes to bobber - Rebecca Morris

		RodActor* rod = dynamic_cast<RodActor*>(info.mActor);

		//If it is in the water
		if (bobberPosition.z <= WATERLEVEL && ((bobberPosition.x >= RIGHTWALL
			&& bobberPosition.x < LEFTWALL) && (bobberPosition.y < FORWARDWALL
			&& bobberPosition.y >= PONDWALL)))
		{
			bobber->PutInWater(); // put the bobber in the water, and spawn the hook
			bobber->HitGround(); // this function stops the bobber from moving, we use it when it hits the ground and the surface of the water
		}
		//If it hits the fence
		// 
		// 25.0f is an adjustment made to the y axis because the walls were not perfectly lined up - Rebecca Morris
		else if (bobberPosition.z > WATERLEVEL && (bobberPosition.y > FORWARDWALL - 25.0f || bobberPosition.y < BACKWALL + 25.0f
			|| bobberPosition.x > LEFTWALL || bobberPosition.x < RIGHTWALL))
		{
			dir = Vector3::Reflect(dir, info.mNormal);
			bobber->RotateToNewForward(dir); // this is us reflecting the bobber
		}

	}
	//If it is in the water
	if (bobberPosition.z <= WATERLEVEL && ((bobberPosition.x >= RIGHTWALL
		&& bobberPosition.x < LEFTWALL) && (bobberPosition.y < FORWARDWALL
			&& bobberPosition.y >= PONDWALL)))
	{
		bobber->PutInWater(); // this is the same as above
		bobber->HitGround();
	}
	//If it hits the ground
	else if (bobberPosition.z <= WATERLEVEL && bobberPosition.y <= PONDWALL)
	{
		bobber->HitGround();
	}
	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
