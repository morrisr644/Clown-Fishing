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
#include "RedFish.h"

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
	moveAwayTimer = 1.0;
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
		Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();

		//for (auto singleFish : GetGame()->GetBasicFishes())
		//{
		//	//BasicFish* singleFish = GetGame()->GetBasicFish();
		//	float basicFishTimer = singleFish->GetFishTimer();

		//	Vector3 fishCurrentPosition = singleFish->GetPosition();
		//	if (isFishOn == false && ((abs(currentPosition.x - fishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - fishCurrentPosition.y) < 100.0)))
		//	{
		//		basicFishTimer -= deltaTime;
		//		singleFish->SetFishTimer(basicFishTimer);
		//		if (basicFishTimer <= 0)
		//		{
		//			Vector3 fishFacingBobber;
		//			fishFacingBobber.x = currentPosition.x - fishCurrentPosition.x;
		//			fishFacingBobber.y = currentPosition.y - fishCurrentPosition.y;
		//			fishFacingBobber.z = currentPosition.z - fishCurrentPosition.z;
		//			fishFacingBobber.Normalize();
		//			GetGame()->GetBasicFish()->RotateToNewForward(fishFacingBobber);
		//			GetGame()->GetBasicFish()->SetAngularSpeed(0);
		//		}
		//	}
		//	else if (((abs(currentPosition.x - fishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - fishCurrentPosition.y)) < 100.0) && isFishOn == true)
		//	{
		//		Vector3 turnFishAround;
		//		turnFishAround.x = -fishCurrentPosition.x;
		//		turnFishAround.y = -fishCurrentPosition.y;
		//		turnFishAround.z = -fishCurrentPosition.z;
		//		turnFishAround.Normalize();
		//		GetGame()->GetBasicFish()->RotateToNewForward(turnFishAround);
		//		GetGame()->GetBasicFish()->SetAngularSpeed(0);
		//	}
		//}


		YellowFish* yellowFish = GetGame()->GetYellowFish();
		if (yellowFish->GetState() == Actor::EActive)
		{
			float yelloFishTimer = yellowFish->GetFishTimer();
			Vector3 yellowFishCurrentPosition = yellowFish->GetPosition();
			if (isFishOn == false && ((abs(currentPosition.x - yellowFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - yellowFishCurrentPosition.y) < 100.0)))
			{
				yelloFishTimer -= deltaTime;
				yellowFish->SetFishTimer(yelloFishTimer);
				if (yelloFishTimer <= 0)
				{
					Vector3 fishFacingBobber;
					fishFacingBobber.x = currentPosition.x - yellowFishCurrentPosition.x;
					fishFacingBobber.y = currentPosition.y - yellowFishCurrentPosition.y;
					fishFacingBobber.z = currentPosition.z - yellowFishCurrentPosition.z;
					fishFacingBobber.Normalize();
					GetGame()->GetYellowFish()->RotateToNewForward(fishFacingBobber);
					GetGame()->GetYellowFish()->SetAngularSpeed(0);
				}
			}
			else if (isFishOn == true && ((abs(currentPosition.x - yellowFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - yellowFishCurrentPosition.y) < 100.0)))
			{
				Vector3 turnFishAround;
				turnFishAround.x = -yellowFishCurrentPosition.x;
				turnFishAround.y = -yellowFishCurrentPosition.y;
				turnFishAround.z = yellowFishCurrentPosition.z;
				turnFishAround.Normalize();
				GetGame()->GetYellowFish()->RotateToNewForward(turnFishAround);
				GetGame()->GetYellowFish()->SetAngularSpeed(0);
			}
		}

		RedFish* redFish = GetGame()->GetRedFish();
		if (redFish->GetState() == Actor::EActive)
		{
			float redFishTimer = redFish->GetFishTimer();
			Vector3 redFishCurrentPosition = redFish->GetPosition();
			if (isFishOn == false && ((abs(currentPosition.x - redFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - redFishCurrentPosition.y) < 100.0)))
			{
				redFishTimer -= deltaTime;
				redFish->SetFishTimer(redFishTimer);
				if (redFishTimer <= 0)
				{
					Vector3 fishFacingBobber;
					fishFacingBobber.x = currentPosition.x - redFishCurrentPosition.x;
					fishFacingBobber.y = currentPosition.y - redFishCurrentPosition.y;
					fishFacingBobber.z = currentPosition.z - redFishCurrentPosition.z;
					fishFacingBobber.Normalize();
					GetGame()->GetRedFish()->RotateToNewForward(fishFacingBobber);
					GetGame()->GetRedFish()->SetAngularSpeed(0);
				}
			}
			else if (isFishOn == true && ((abs(currentPosition.x - redFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - redFishCurrentPosition.y) < 100.0)))
			{
				Vector3 turnFishAround;
				turnFishAround.x = -redFishCurrentPosition.x;
				turnFishAround.y = -redFishCurrentPosition.y;
				turnFishAround.z = redFishCurrentPosition.z;
				turnFishAround.Normalize();
				GetGame()->GetRedFish()->RotateToNewForward(turnFishAround);
				GetGame()->GetRedFish()->SetAngularSpeed(0);
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

void BobberActor::FishOn()
{
	isFishOn = true;
}

void BobberActor::FishOff()
{
	isFishOn = false;
}

void BobberActor::SetLaunchAngle(float newAngle)
{
	mLaunchAngle = newAngle;
}

void BobberActor::SetMyMoveSpeed()
{
	mMyMove->SetForwardSpeed(1000.0f);
}

