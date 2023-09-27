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

	//Attempt at making a curve
	/*Vector3 angle = start;
	angle.z = start.z * 0.9;
	mOwner->RotateToNewForward(angle);*/

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	// (Don't collide vs player)
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{
		// If we collided, reflect the ball about the normal
		//dir = Vector3::Reflect(dir, info.mNormal);
		//mOwner->RotateToNewForward(dir);

		//// If we collided, stop.
		//dir = Vector3(0, 0, 0);


		static_cast<BobberActor*>(mOwner)->HitGround();

	}



	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
