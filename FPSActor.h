// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "MobileActor.h"
#include "SoundEvent.h"

class MeshComponent;

class FPSActor : public MobileActor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void Shoot();

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

	void FixFPSCollisions();

	class FPSCamera* GetCamera() { return mCameraComp; }
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FPSCamera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Actor* mFPSModel;
	SoundEvent mFootstep;
	float mLastFootstep;
	std::vector<MeshComponent*> meshComponents_;
};
