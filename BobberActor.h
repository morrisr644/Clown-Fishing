// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Previously BallActor.h

#pragma once
#include "Actor.h"
#include "AudioComponent.h"
#include "BasicFish.h"

class BobberActor : public Actor
{
public:
	BobberActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);

	void HitGround();
	void PutInWater();
	void OutOfWater();
	void FishOn();
	void FishOff(BasicFish* currFish);
	void SetLaunchAngle(float newAngle);
	void SetMyMoveSpeed();
	void SetForwardVelocity(Vector3 newVelocity) { mForwardVelocity = newVelocity; }
	void ResetMoveAwayTimer() { moveAwayTimer = 3.0; }
	bool GetFishOnStatus() { return isFishOn; }
	bool BobberActor::GetInWaterStatus() { return isInWater; }
	void CheckFish(float deltaTime, BasicFish* currFish);
	void SetTensionSpeed(float tensionSpeed);
	float getTotalDistance() { return mTotalDistance; };
	void setBobberFishOn(bool fishOn) { isFishOn = fishOn; }
private:
	class AudioComponent* mAudioComp;
	class BobberMove* mMyMove;
	float mLifeSpan;
	double mLaunchAngle;
	bool isInWater;
	bool isFishOn;
	float moveAwayTimer;
	Vector3 mForwardVelocity;
	bool hasAlreadySplashed;
	SoundEvent mSplash;		//Sound effects for the bobber entering water,
	SoundEvent mBubbles;	// fish getting hooked
	SoundEvent mLose;		// and player losing the fish - Rebecca Morris
	float mTotalDistance;
};
