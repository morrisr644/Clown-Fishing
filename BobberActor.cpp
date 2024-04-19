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
#include "ScreenSaysFishOff.h"
#include "Hook.h"
#include "TargetComponent.h"

BobberActor::BobberActor(Game* game)
	:Actor(game)
	,mLifeSpan(2.0f)
	,mLaunchAngle(0.0)
	, mForwardVelocity()
{

	new TargetComponent(this);

	//SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"); // Sphere has been recolored to look more like a bobber (RCM)
	mc->SetMesh(mesh);
	mMyMove = new BobberMove(this);
	mMyMove->SetForwardSpeed(1000.0f);
	mAudioComp = new AudioComponent(this);

	mSplash = mAudioComp->PlayEvent("event:/Splash");
	mSplash.SetPaused(true);
	mBubbles = mAudioComp->PlayEvent("event:/Bubbles");
	mBubbles.SetPaused(true);
	mLose = mAudioComp->PlayEvent("event:/Lose");
	mLose.SetPaused(true);
	isInWater = false;
	isFishOn = false;
	hasAlreadySplashed = false;
	moveAwayTimer = 1.0;
	mTotalDistance = 0.0f;
}

void BobberActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	float gravity = 208.1;

	//mLifeSpan -= deltaTime;

	
	if (mMyMove->GetForwardSpeed() != 0 && !isFishOn)
	{
		// Below is the math done to create gravity on the bobber after it is shot out. Adam Caligiuri
		Vector3 pos = GetPosition();
		Vector3 velocity = mForwardVelocity;
		Vector3 acceleration(0, 0, -gravity);

		pos = 0.5 * acceleration * deltaTime * deltaTime + velocity * deltaTime + pos;
		velocity = velocity + acceleration * deltaTime;

		SetForwardVelocity(velocity);
		SetPosition(pos);
	}

	std::vector<class BasicFish*> otherFishArray = GetGame()->GetBasicFishes();
	for (BasicFish* fish : otherFishArray)
	{
		if (fish->GetState() != Actor::EDead)
		{
			CheckFish(deltaTime, fish); // Note check only if alive
		}
	}
}

void BobberActor::CheckFish(float deltaTime, BasicFish* currFish)
{
	Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();
	BobberActor* currentBobber = GetGame()->GetBobber();
	Hook* currentHook = GetGame()->GetHook();
	Vector3 currentHookPosition = GetGame()->GetHook()->GetPosition();
	if (currFish->GetState() == Actor::EActive)
	{
		float fishTimer = currFish->GetFishTimer();
		Vector3 fishCurrentPosition = currFish->GetPosition();
		if ((currentBobber->GetFishOnStatus() == false) && ((abs(currentHookPosition.x - fishCurrentPosition.x) < 100.0) || (abs(currentHookPosition.y - fishCurrentPosition.y) < 100.0)))
		{ // if there is no fish on the line, and the yellow fish is within range of the hook on the x and y axis, move toward the hook. Adam Caligiuri
			fishTimer -= deltaTime;
			currFish->SetFishTimer(fishTimer);
			if (fishTimer <= 0)
			{
				Vector3 fishFacingBobber;
				fishFacingBobber = currentHookPosition - fishCurrentPosition; //  Adam Example of the operator - in use.
				fishFacingBobber.Normalize();
				currFish->RotateToNewForward(fishFacingBobber);
				currFish->SetAngularSpeed(0);
			}
		} 
		else if (currentBobber->GetFishOnStatus() == true && currFish->GetLineStatus() == false) // This handles if the fish is in range, but another fish gets on the line
		{ // before the current one, turn the current fish around as if it is scared.  Adam Caligiuri
			if (currFish->GetFleeingStatus() == false)
			{
				Vector3 turnFishAround = -currFish->GetForward(); // the use of the new -operator with Dr. Briggs help
				currFish->RotateToNewForward(turnFishAround);
				currFish->SetAngularSpeed(0);
				currFish->SetFleeingStatus(true);

			}

		}
		else if (currentBobber->GetFishOnStatus() == true && currFish->GetLineStatus() == true) 
		{ // this handles the fish on the line moving backward slowly as it is on the line. Adam Caligiuri
			Vector3 startPos = this->GetPosition();
			Vector3 currPos = currFish->GetPosition();
			Vector3 difference;
			difference.x = abs(currPos.x - startPos.x);
			difference.y = abs(currPos.y - startPos.y);
			difference.z = abs(currPos.z - startPos.z);
			mTotalDistance = difference.x + difference.y + difference.z;
			GetGame()->SetFishHookDistance(mTotalDistance);
			if (mTotalDistance > currFish->GetFishDistance() && mTotalDistance != 0) // The case of mTotalDistance being zero must be added so the fish can be caught
			{ 
				
				FishOff(currFish);

				mTotalDistance = 0;

			}
			// Tweak start speed and rotation for each fish individually Adam
			// when each fish gets off the line, check what fish it is and reset its original move speed and rotation.

		}
		// if the fish is on the line, grab the position of the fish when it first gets on the line.
				// take that initial position and the current position, get the absolute value of current minus old, then take that number and compare to the 
				// fishDistance float.  When that number is larger than the fishDistance, put the bobber outside the map to show the bobber going away, set the fish to fleeing,
				// and should be it?
		else if (((abs(currentPosition.x - fishCurrentPosition.x) > 200.0) || 
			(abs(currentPosition.y - fishCurrentPosition.y) > 200.0) || 
			(abs(currentPosition.z - fishCurrentPosition.z) > 200.0)) || currFish->GetFleeingStatus())
		{
			currFish->SetFishTimer(1.0);
			currFish->SetMovementSpeed(200);
			currFish->SetAngularSpeed(0.2);
			currFish->SetFleeingStatus(false);
		}
	}
}

void BobberActor::SetPlayer(Actor* player)
{
	mMyMove->SetPlayer(player);
	
}

void BobberActor::HitGround() // if the bobber hits the ground, it stops moving
{
	mMyMove->SetForwardSpeed(0.0f);
}

void BobberActor::PutInWater() // If the bobber is in the water, play the splash sound and let the game know the bobber is in the water
{
	isInWater = true;
	if (!hasAlreadySplashed)
	{
		mSplash.SetPaused(false);
		mSplash.Restart();
		hasAlreadySplashed = true;
	}
}

void BobberActor::OutOfWater() // used if the bobber is taken out of the water for whatever reason
{
	isInWater = false;
	mSplash.SetPaused(true);
	hasAlreadySplashed = false;
}

void BobberActor::FishOn() // handles if the fish is put on the line, lets the game know a fish is on the line.
{
	isFishOn = true;
	mBubbles.SetPaused(false);
	mBubbles.Restart();
}

void BobberActor::FishOff(BasicFish* currFish) // This handles when the fish breaks the line, the fish turns away from th ebobber and starts moving
{// we move the bobber back to its spawn point and we reset all of the variables letting the game know the fish is off Adam Caligiuri
	BobberActor* currentBobber = GetGame()->GetBobber();

	Vector3 turnFishAround = currFish->GetForward();
	Vector3 zeroedVector(0.0, 0.0, 0.0);
	turnFishAround = zeroedVector - turnFishAround;
	turnFishAround.Normalize();
	currFish->RotateToNewForward(turnFishAround);
	currFish->SetFleeingStatus(true);
	currFish->SetLineStatus(false); 
	Vector3 bobberSpawnPoint(20000, 20000, 0);
	currentBobber->SetPosition(bobberSpawnPoint);
	currFish->SetFishTimer(1.0);
	currFish->SetMovementSpeed(200);
	currFish->SetAngularSpeed(0.2);
	GetGame()->isReelingIn = false;

	mLose.SetPaused(false);
	mLose.Restart();

	new ScreenSaysFishOff(this->GetGame());
	this->GetGame()->TurnScreenSaysFishOffOn();

	isFishOn = false;
	mBubbles.SetPaused(true);
}

void BobberActor::SetLaunchAngle(float newAngle)
{
	mLaunchAngle = newAngle;
}

void BobberActor::SetMyMoveSpeed()
{
	mMyMove->SetForwardSpeed(1000.0);
}

void BobberActor::SetTensionSpeed(float tensionSpeed)
{
	mMyMove->SetForwardSpeed(tensionSpeed);
}

