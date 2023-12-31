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
	void Splash();
	void FishOff();
	void SetLaunchAngle(float newAngle);
	void SetMyMoveSpeed();
	void SetForwardVelocity(Vector3 newVelocity) { mForwardVelocity = newVelocity; }
	void ResetMoveAwayTimer() { moveAwayTimer = 3.0; }
	bool GetFishOnStatus() { return isFishOn; }
	bool BobberActor::GetInWaterStatus() { return isInWater; }
	void CheckYellowFish(float deltaTime);
	void CheckRedFish(float deltaTime);
	void SetTensionSpeed(float tensionSpeed);
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
};
