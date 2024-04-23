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

	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"); // Sphere has been recolored to look more like a bobber - Rebecca Morris
	mc->SetMesh(mesh);
	mMyMove = new BobberMove(this);
	mMyMove->SetForwardSpeed(1000.0f);
	mAudioComp = new AudioComponent(this);

	mSplash = mAudioComp->PlayEvent("event:/Splash"); // Sound effect for bobber going in water - Rebecca Morris
	mSplash.SetPaused(true);
	mBubbles = mAudioComp->PlayEvent("event:/Bubbles"); // Sound effect for fish getting on the line - Rebecca Morris
	mBubbles.SetPaused(true);
	mLose = mAudioComp->PlayEvent("event:/Lose"); // Sound effect for player not catching the fish in time - Rebecca Morris
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
	for (BasicFish* fish : otherFishArray) // check every fish in the games array, but only check if its set to alive
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
		{ // if there is no fish on the line, and a fish is within range of the hook, we reduce its timer by delta time
			fishTimer -= deltaTime;
			currFish->SetFishTimer(fishTimer);
			if (fishTimer <= 0)// if there is no fish on the line, and a fish is within range of the hook, and its timer has hit 0
				// we turn the fish towards the hook with simple math provided by madhav, and the fish moves towards the hook.
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
			Vector3 currPos = currFish->GetPosition();// grabs the fishes position, and the bobbers position, finds that line, takes that distance and test it
			Vector3 difference;						  // against the fishes preset get loose distance. 
			difference.x = abs(currPos.x - startPos.x);
			difference.y = abs(currPos.y - startPos.y);
			difference.z = abs(currPos.z - startPos.z);
			mTotalDistance = difference.x + difference.y + difference.z;
			GetGame()->SetFishHookDistance(mTotalDistance);
			if (mTotalDistance > currFish->GetFishDistance() && mTotalDistance != 0) // The case of mTotalDistance being zero must be added so the fish can be caught
			{ 
				// if a fish has moved far enough away from the bobber we consider that them breaking free and the fish breaks loose.
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

	Vector3 turnFishAround = currFish->GetForward(); // grabs the current fishes direction
	Vector3 zeroedVector(0.0, 0.0, 0.0);
	turnFishAround = zeroedVector - turnFishAround; // just gives us the opposite of the above vector
	turnFishAround.Normalize(); // normalizes that vector, probably not needed
	currFish->RotateToNewForward(turnFishAround); // now we turn the fish around so its now swimming in the opposite direction

	currFish->SetFleeingStatus(true); // this is used to make it so that a fish that is fleeing cannot turn around and go back after the bobber, it has to get
									  // a certain distance away from the bobber before it turns back around to go after the hook.
	currFish->SetLineStatus(false); // reset the line status for the game

	Vector3 bobberSpawnPoint(20000, 20000, 0);
	currentBobber->SetPosition(bobberSpawnPoint); // this is just sending the bobber back to its spawn point, reseting it.
	currFish->SetFishTimer(1.0);// each fish has a timer that relates to how long is needs to be in range of the a hook before it is attracted to the hook
					// the timer goes down to 0, then it goes towards the hook.  If a fish gets off the hook that timer needs to reset, so we reset it back to a second.
	currFish->SetMovementSpeed(200); // reseting a fishes movement speed
	currFish->SetAngularSpeed(0.2); // just set them on a slight angle so they look less robotic.
	GetGame()->isReelingIn = false; // let the game know we are no longer reeling a fish in, which turns off the ability to press space

	mLose.SetPaused(false); // this is the womp womp womp sound effect that plays when a fish gets off the line
	mLose.Restart();

	new ScreenSaysFishOff(this->GetGame());  // let the player know a fish has got off the line.
	this->GetGame()->TurnScreenSaysFishOffOn();

	isFishOn = false;  // resets the games boolean, which then allows another fish to be attracted to a hook if its within range.
	mBubbles.SetPaused(true); // pauses the bubbles sound for the case in which it is still playing when the fish breaks off - Rebecca Morris
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

