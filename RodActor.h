// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class RodActor : public Actor
{
public:
	RodActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	//void ActorInput(const uint8_t* keys) override;

	/*void Shoot();

	void SetFootstepSurface(float value);*/

	void SetVisible(bool visible);

	void FixCollisions();
private:
	class MoveComponent* mMoveComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Actor* mRodModel;
};
