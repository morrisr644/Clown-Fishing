// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Previously BallActor.cpp
// Edits made by Rebecca Morris

#include "BobberActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BobberMove.h"
#include "AudioComponent.h"
#include "Math.h"
#include "BasicFish.h"
#include "YellowFish.h"

BobberActor::BobberActor(Game* game)
	:Actor(game)
	,mLifeSpan(2.0f)
	,mLaunchAngle(0.0)
	, mForwardVelocity()
{
	//SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"); // Sphere has been recolored to look more like a bobber (RCM)
	mc->SetMesh(mesh);
	mMyMove = new BobberMove(this);
	mMyMove->SetForwardSpeed(1000.0f);
	mAudioComp = new AudioComponent(this);
	isInWater = false;
}

void BobberActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	float gravity = 98.1;

	//mLifeSpan -= deltaTime;

	
	if (mMyMove->GetForwardSpeed() != 0)
	{

		Vector3 pos = GetPosition();
		Vector3 velocity = mForwardVelocity;
		Vector3 acceleration(0, 0, -gravity);

		pos = 0.5 * acceleration * deltaTime * deltaTime + velocity * deltaTime + pos;
		velocity = velocity + acceleration * deltaTime;

		SetForwardVelocity(velocity);
		SetPosition(pos);
	}

	//Currently I am checking if the movement speed is 0, and if it is then its considered in the water.
	//Note for Adam: refactoring in the future by putting these in their own functions is probably a good idea to clean up code.
	if (mMyMove->GetForwardSpeed() == 0 && isInWater)
	{
		Vector3 stopVelocity(0.0, 0.0, 0.0);
		SetForwardVelocity(stopVelocity);
		BasicFish* singleFish = GetGame()->GetBasicFish();
		float basicFishTimer = singleFish->GetBasicFishTimer();
		basicFishTimer -= deltaTime;
		singleFish->SetBasicFishTimer(basicFishTimer);
		if (basicFishTimer <= 0)
		{
			Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();

			Vector3 fishCurrentPosition = singleFish->GetPosition();
			if ((abs(currentPosition.x - fishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - fishCurrentPosition.y) < 100.0))
			{
				Vector3 fishFacingBobber;
				fishFacingBobber.x = currentPosition.x - fishCurrentPosition.x;
				fishFacingBobber.y = currentPosition.y - fishCurrentPosition.y;
				fishFacingBobber.z = 0;
				fishFacingBobber.Normalize();
				GetGame()->GetBasicFish()->RotateToNewForward(fishFacingBobber);
				GetGame()->GetBasicFish()->SetAngularSpeed(0);

			}
		}

		YellowFish* yellowFish = GetGame()->GetYellowFish();
		float yelloFishTimer = yellowFish->GetYellowFishTimer();
		yelloFishTimer -= deltaTime;
		yellowFish->SetYellowFishTimer(yelloFishTimer);
		if (yelloFishTimer <= 0)
		{
			Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();

			Vector3 fishCurrentPosition = yellowFish->GetPosition();
			if ((abs(currentPosition.x - fishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - fishCurrentPosition.y) < 100.0))
			{
				Vector3 fishFacingBobber;
				fishFacingBobber.x = currentPosition.x - fishCurrentPosition.x;
				fishFacingBobber.y = currentPosition.y - fishCurrentPosition.y;
				fishFacingBobber.z = 0;
				fishFacingBobber.Normalize();
				GetGame()->GetYellowFish()->RotateToNewForward(fishFacingBobber);
				GetGame()->GetYellowFish()->SetAngularSpeed(0);

			}
		}
		
	}
}

void BobberActor::SetPlayer(Actor* player)
{
	mMyMove->SetPlayer(player);
	
}

void BobberActor::HitGround()
{
	mMyMove->SetForwardSpeed(0.0f);
}

void BobberActor::PutInWater()
{
	isInWater = true;
}

void BobberActor::OutOfWater()
{
	isInWater = false;
}

void BobberActor::SetLaunchAngle(float newAngle)
{
	mLaunchAngle = newAngle;
}

void BobberActor::SetMyMoveSpeed()
{
	mMyMove->SetForwardSpeed(1000.0f);
}

