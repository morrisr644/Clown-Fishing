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

	/*BasicFish* yellowFish = GetGame()->GetYellowFish();
	BasicFish* redFish = GetGame()->GetRedFish();
	const int NUMOFFISH = 2;
	BasicFish* fishArray[NUMOFFISH] = { yellowFish, redFish };*/
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
		{ // if there is no fish on the line, and the yellow fish is within range of the hook on the x and y axis, move toward the hook.
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
		} //&& ((abs(currentPosition.x - redFishCurrentPosition.x) < 100.0) || (abs(currentPosition.y - redFishCurrentPosition.y) < 100.0))
		else if (currentBobber->GetFishOnStatus() == true && currFish->GetLineStatus() == false) // add an and this fish is not on the line here
		{
			if (currFish->GetFleeingStatus() == false)
			{
				Vector3 turnFishAround = currFish->GetForward();
				Vector3 zeroedVector(0.0, 0.0, 0.0);
				turnFishAround = zeroedVector - turnFishAround; //  Adam Example of the operator - in use to get an inverse.
				turnFishAround.Normalize();
				currFish->RotateToNewForward(turnFishAround);
				currFish->SetAngularSpeed(0);
				currFish->SetFleeingStatus(true);

			}

		}
		else if (currentBobber->GetFishOnStatus() == true && currFish->GetLineStatus() == true) // if the current fish on is the yellow fish, then do this
		{
			Vector3 startPos = this->GetPosition();
			//startPos = GetGame()->GetHook()->GetPosition();
			Vector3 currPos = currFish->GetPosition();
			Vector3 difference;
			difference.x = abs(currPos.x - startPos.x);
			difference.y = abs(currPos.y - startPos.y);
			difference.z = abs(currPos.z - startPos.z);
			mTotalDistance = difference.x + difference.y + difference.z;
			GetGame()->SetFishHookDistance(mTotalDistance);
			if (mTotalDistance > 800.0 && mTotalDistance != 0) // The case of mTotalDistance being zero must be added so the fish can be caught
			{ 

				Vector3 turnFishAround = currFish->GetForward();
				Vector3 zeroedVector(0.0, 0.0, 0.0);
				turnFishAround = zeroedVector - turnFishAround;
				turnFishAround.Normalize();
				currFish->RotateToNewForward(turnFishAround);
				currFish->SetFleeingStatus(true);
 				currFish->SetLineStatus(false); //Is this where the game glitches?
				Vector3 bobberSpawnPoint(20000, 20000, 0);
				currentBobber->SetPosition(bobberSpawnPoint);
				currFish->SetFishTimer(1.0);
				currFish->SetMovementSpeed(200);
				currFish->SetAngularSpeed(0.2);
				GetGame()->isReelingIn = false;

				this->isFishOn = false;

				mLose.SetPaused(false);
				mLose.Restart();

				new ScreenSaysFishOff(this->GetGame());
				this->GetGame()->TurnScreenSaysFishOffOn(); 

				mTotalDistance = 0;

			}

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


/*void BobberActor::CheckYellowFish(float deltaTime)
{
	Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();
	BasicFish* yellowFish = GetGame()->GetYellowFish();
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
		else if (currentBobber->GetFishOnStatus() == true && yellowFish->GetLineStatus() == false && GetGame()->GetYellowFish()->GetState() != Actor::EDead) // add an and this fish is not on the line here
		{
			if (yellowFish->GetFleeingStatus() == false && GetGame()->GetYellowFish()->GetState() != Actor::EDead)
			{
				Vector3 turnFishAround = yellowFish->GetForward();
				turnFishAround.x = -turnFishAround.x;
				turnFishAround.y = -turnFishAround.y;
				turnFishAround.z = -turnFishAround.z;
				turnFishAround.Normalize();
				GetGame()->GetYellowFish()->RotateToNewForward(turnFishAround);
				//GetGame()->GetYellowFish()->SetAngularSpeed(0);
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
			if (mTotalDistance > 900.0 && mTotalDistance != 0) // The case of mTotalDistance being zero must be added so the fish can be caught
			{ // yellowFish->GetFishDistance()
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

				mLose.SetPaused(false);
				mLose.Restart();

				new ScreenSaysFishOff(this->GetGame());
				this->GetGame()->TurnScreenSaysFishOffOn();

				mTotalDistance = 0;

			}

		}
		// if the fish is on the line, grab the position of the fish when it first gets on the line.
				// take that initial position and the current position, get the absolute value of current minus old, then take that number and compare to the 
				// fishDistance float.  When that number is larger than the fishDistance, put the bobber outside the map to show the bobber going away, set the fish to fleeing,
				// and should be it?
		else if ((GetGame()->GetYellowFish()->GetState() != Actor::EDead && abs(currentPosition.x - yellowFishCurrentPosition.x) > 200.0) || (abs(currentPosition.y - yellowFishCurrentPosition.y) > 200.0) || (abs(currentPosition.z - yellowFishCurrentPosition.z) > 200.0))
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
}*/


