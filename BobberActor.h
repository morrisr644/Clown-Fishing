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
	//void Splash();
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
	//void CheckFish(float deltaTime, BasicFish* currFish);
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
	SoundEvent mSplash;
	SoundEvent mBubbles;
	SoundEvent mLose; // For when you lose the fish
	float mTotalDistance;
};
