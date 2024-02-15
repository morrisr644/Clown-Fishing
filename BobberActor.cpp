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
#include "FishOffScreen.h"
#include "Hook.h"

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

	mSplash = mAudioComp->PlayEvent("event:/Splash");
	mSplash.SetPaused(true);
	mBubbles = mAudioComp->PlayEvent("event:/Bubbles");
	mBubbles.SetPaused(true);
	isInWater = false;
	isFishOn = false;
	hasAlreadySplashed = false;
	moveAwayTimer = 1.0;
	mTotalDistance = 0.0f;
}

void BobberActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	float gravity = 98.1;

	//mLifeSpan -= deltaTime;

	
	if (mMyMove->GetForwardSpeed() != 0 && !isFishOn)
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
	if (mMyMove->GetForwardSpeed() == 0 && !isFishOn)
	{
		Vector3 stopVelocity(0.0, 0.0, 0.0);
		SetForwardVelocity(stopVelocity);
		if (GetGame()->GetBobber()->GetInWaterStatus())
		{
			CheckYellowFish(deltaTime);
			CheckRedFish(deltaTime);
		}
	}
	CheckYellowFish(deltaTime);
	CheckRedFish(deltaTime);
}

void BobberActor::CheckYellowFish(float deltaTime)
{
	Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();
	YellowFish* yellowFish = GetGame()->GetYellowFish();
	BobberActor* currentBobber = GetGame()->GetBobber();
	Hook* currentHook = GetGame()->GetHook();
	Vector3 currentHookPosition = GetGame()->GetHook()->GetPosition();
	if (yellowFish->GetState() == Actor::EActive)
	{
		float yellowFishTimer = yellowFish->GetFishTimer();
		Vector3 yellowFishCurrentPosition = yellowFish->GetPosition();
		if ((currentBobber->GetFishOnStatus() == false) && ((abs(currentHookPosition.x - yellowFishCurrentPosition.x) < 100.0) || (abs(currentHookPosition.y - yellowFishCurrentPosition.y) < 100.0)))
		{ // if there is no fish on the line, and the yellow fish is within range of the hook on the x and y axis, move toward the hook.
			yellowFishTimer -= deltaTime;
			yellowFish->SetFishTimer(yellowFishTimer);
			if (yellowFishTimer <= 0)
			{
				Vector3 fishFacingBobber;
				fishFacingBobber.x = currentHookPosition.x - yellowFishCurrentPosition.x;
				fishFacingBobber.y = currentHookPosition.y - yellowFishCurrentPosition.y;
				fishFacingBobber.z = currentHookPosition.z - yellowFishCurrentPosition.z;
				fishFacingBobber.Normalize();
				GetGame()->GetYellowFish()->RotateToNewForward(fishFacingBobber);
				GetGame()->GetYellowFish()->SetAngularSpeed(0);
			}
		} //&& ((abs(currentPosition.x - redFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - redFishCurrentPosition.y) < 100.0))
		else if (currentBobber->GetFishOnStatus() == true && yellowFish->GetLineStatus() == false) // add an and this fish is not on the line here
		{
			if (yellowFish->GetFleeingStatus() == false)
			{
				Vector3 turnFishAround = yellowFish->GetForward();
				turnFishAround.x = -turnFishAround.x;
				turnFishAround.y = -turnFishAround.y;
				turnFishAround.z = -turnFishAround.z;
				turnFishAround.Normalize();
				GetGame()->GetYellowFish()->RotateToNewForward(turnFishAround);
				GetGame()->GetYellowFish()->SetAngularSpeed(0);
				yellowFish->SetFleeingStatus(true);

			}

		}
		else if (currentBobber->GetFishOnStatus() == true && yellowFish->GetLineStatus() == true) // if the current fish on is the yellow fish, then do this
		{
			Vector3 startPos = this->GetPosition();
			Vector3 currPos = yellowFish->GetPosition();
			Vector3 difference;
			difference.x = abs(currPos.x - startPos.x);
			difference.y = abs(currPos.y - startPos.y);
			difference.z = abs(currPos.z - startPos.z);
			mTotalDistance = difference.x + difference.y + difference.z;
			GetGame()->SetFishHookDistance(mTotalDistance); 
			if (mTotalDistance > yellowFish->GetFishDistance() && mTotalDistance != 0) // The case of mTotalDistance being zero must be added so the fish can be caught
			{
				Vector3 turnFishAround = yellowFish->GetForward();
				turnFishAround.x = -turnFishAround.x;
				turnFishAround.y = -turnFishAround.y;
				turnFishAround.z = -turnFishAround.z;
				turnFishAround.Normalize();
				GetGame()->GetYellowFish()->RotateToNewForward(turnFishAround);
				GetGame()->GetYellowFish()->SetAngularSpeed(0);
				yellowFish->SetFleeingStatus(true);
				yellowFish->SetLineStatus(false);
				Vector3 bobberSpawnPoint(20000, 20000, 0);
				currentBobber->SetPosition(bobberSpawnPoint);
				yellowFish->SetFishTimer(1.0);
				yellowFish->SetMovementSpeed(200);
				yellowFish->SetAngularSpeed(0.2);
				GetGame()->isReelingIn = false;

				new FishOffScreen(this->GetGame());
				this->GetGame()->TurnFishOffScreenOn();

			}

		}
		// if the fish is on the line, grab the position of the fish when it first gets on the line.
				// take that initial position and the current position, get the absolute value of current minus old, then take that number and compare to the 
				// fishDistance float.  When that number is larger than the fishDistance, put the bobber outside the map to show the bobber going away, set the fish to fleeing,
				// and should be it?
		else if (((abs(currentPosition.x - yellowFishCurrentPosition.x) > 200.0) || (abs(currentPosition.y - yellowFishCurrentPosition.y) > 200.0) || (abs(currentPosition.z - yellowFishCurrentPosition.z) > 200.0)))
		{
			yellowFish->SetFishTimer(1.0);
			yellowFish->SetMovementSpeed(200);
			yellowFish->SetAngularSpeed(0.2);
			yellowFish->SetFleeingStatus(false);
		}

		else if (yellowFish->GetFleeingStatus() == true)
		{
			yellowFish->SetFishTimer(1.0);
			yellowFish->SetMovementSpeed(200);
			yellowFish->SetAngularSpeed(0.2);
			yellowFish->SetFleeingStatus(false);
		}

	}
}

// Clean this function up, break it up a bit
void BobberActor::CheckRedFish(float deltaTime)
{
	Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();
	RedFish* redFish = GetGame()->GetRedFish();
	BobberActor* currentBobber = GetGame()->GetBobber();
	Hook* currentHook = GetGame()->GetHook();
	Vector3 currentHookPosition = GetGame()->GetHook()->GetPosition();
	if (redFish->GetState() == Actor::EActive)
	{
		float redFishTimer = redFish->GetFishTimer();
		Vector3 redFishCurrentPosition = redFish->GetPosition();
		// if the fish is not currently on the line, and its in range, head towards the bobber
		if ((currentBobber->GetFishOnStatus() == false) && ((abs(currentHookPosition.x - redFishCurrentPosition.x) < 100.0) || (abs(currentHookPosition.y - redFishCurrentPosition.y) < 100.0)))
		{
			redFishTimer -= deltaTime;
			redFish->SetFishTimer(redFishTimer);
			if (redFishTimer <= 0)
			{
				Vector3 fishFacingBobber;
				fishFacingBobber.x = currentHookPosition.x - redFishCurrentPosition.x;
				fishFacingBobber.y = currentHookPosition.y - redFishCurrentPosition.y;
				fishFacingBobber.z = currentHookPosition.z - redFishCurrentPosition.z;
				fishFacingBobber.Normalize();
				GetGame()->GetRedFish()->RotateToNewForward(fishFacingBobber);
				GetGame()->GetRedFish()->SetAngularSpeed(0);
			}
		} //&& ((abs(currentPosition.x - redFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - redFishCurrentPosition.y) < 100.0))
		//if a fish that is not the red fish is on the line do this.
		// seems like this is currently not working?
		else if (currentBobber->GetFishOnStatus() == true && redFish->GetLineStatus() == false) // add an and this fish is not on the line here
		{
			if (redFish->GetFleeingStatus() == false)
			{
				Vector3 turnFishAround = redFish->GetForward();
				turnFishAround.x = -turnFishAround.x;
				turnFishAround.y = -turnFishAround.y;
				turnFishAround.z = -turnFishAround.z;
				turnFishAround.Normalize();
				GetGame()->GetRedFish()->RotateToNewForward(turnFishAround);
				GetGame()->GetRedFish()->SetAngularSpeed(0);
				redFish->SetFleeingStatus(true);
				
			}

		}
		// if the current fish on the line is the red fish
		else if (currentBobber->GetFishOnStatus() == true && redFish->GetLineStatus() == true)
		{
			Vector3 startPos = this->GetPosition();
			Vector3 currPos = redFish->GetPosition();
			Vector3 difference;
			difference.x = abs(currPos.x - startPos.x);
			difference.y = abs(currPos.y - startPos.y);
			difference.z = abs(currPos.z - startPos.z);
			float totalDistance = difference.x + difference.y + difference.z;
			GetGame()->SetFishHookDistance(totalDistance);
			if (totalDistance > redFish->GetFishDistance() && totalDistance != 0) // Rebecca, this chunk of code handles if the RedFish gets too far from the bobber
			{
				Vector3 turnFishAround = redFish->GetForward();
				turnFishAround.x = -turnFishAround.x;
				turnFishAround.y = -turnFishAround.y;
				turnFishAround.z = -turnFishAround.z;
				turnFishAround.Normalize();
				GetGame()->GetRedFish()->RotateToNewForward(turnFishAround);
				GetGame()->GetRedFish()->SetAngularSpeed(0);
				redFish->SetFleeingStatus(true);
				redFish->SetLineStatus(false);
				Vector3 bobberSpawnPoint(20000, 20000, 0);
				currentBobber->SetPosition(bobberSpawnPoint);
				redFish->SetFishTimer(1.0);
				redFish->SetMovementSpeed(200);
				redFish->SetAngularSpeed(0.2);
				GetGame()->isReelingIn = false;

				new FishOffScreen(this->GetGame());
				this->GetGame()->TurnFishOffScreenOn();
				
			}
			
		}
		// if the fish is on the line, grab the position of the fish when it first gets on the line.
				// take that initial position and the current position, get the absolute value of current minus old, then take that number and compare to the 
				// fishDistance float.  When that number is larger than the fishDistance, put the bobber outside the map to show the bobber going away, set the fish to fleeing,
				// and should be it?
		else if (((abs(currentPosition.x - redFishCurrentPosition.x) > 200.0) || (abs(currentPosition.y - redFishCurrentPosition.y) > 200.0) || (abs(currentPosition.z - redFishCurrentPosition.z) > 200.0)))
		{
			redFish->SetFishTimer(1.0);
			redFish->SetMovementSpeed(200);
			redFish->SetAngularSpeed(0.2);
			redFish->SetFleeingStatus(false);
		}

		else if (redFish->GetFleeingStatus() == true)
		{
			redFish->SetFishTimer(1.0);
			redFish->SetMovementSpeed(200);
			redFish->SetAngularSpeed(0.2);
			redFish->SetFleeingStatus(false);
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
	if (!hasAlreadySplashed)
	{
		mSplash.SetPaused(false);
		mSplash.Restart();
		hasAlreadySplashed = true;
	}
}
void BobberActor::Splash()
{
	/*isInWater = true;
	if (!hasAlreadySplashed)
	{
		mSplash.SetPaused(false);
		mSplash.Restart();
		hasAlreadySplashed = true;
	}*/
}

void BobberActor::OutOfWater()
{
	isInWater = false;
	mSplash.SetPaused(true);
	hasAlreadySplashed = false;
}

void BobberActor::FishOn()
{
	isFishOn = true;
	mBubbles.SetPaused(false);
	mBubbles.Restart();
}

void BobberActor::FishOff()
{
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



