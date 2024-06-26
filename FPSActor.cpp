// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
// This is a comment for the test merge - Adam Caligiuri

#include "FPSActor.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "BobberActor.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
#include "PhysWorld.h"
#include "Texture.h"
#include "AssimpLoad.h"
#include "Mesh.h"
#include "RodActor.h"

FPSActor::FPSActor(Game* game)
	:MobileActor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep"); //The sound effect for when the player takes a step - Rebecca Morris
	mFootstep.SetPaused(true);

	mCameraComp = new FPSCamera(this);

	//Added Rod Actor to make the addition of the new model simpler - Rebecca
	mFPSModel = new RodActor(game);
	mFPSModel->SetScale(0.05f);

	// Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
		Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions();
	FixFPSCollisions();

	// Play the footstep if we're moving and haven't recently
	mLastFootstep -= deltaTime;
	if ((!Math::NearZero(mMoveComp->GetForwardSpeed()) ||
		!Math::NearZero(mMoveComp->GetStrafeSpeed())) &&
		mLastFootstep <= 0.0f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}
	
	// Update position of FPS model relative to actor position
	const Vector3 modelOffset(Vector3(15.0f, -15.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);
	// Initialize rotation to actor rotation
	Quaternion q = GetRotation();
	// Rotate by pitch from camera
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), mCameraComp->GetPitch()));
	//Adjusted the pitch to make the rod face the right way - Rebecca
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), -180.0));
	//Quaternion r = mFPSModel->GetRotation();
	
	mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// Mouse movement
	// Get relative movement from SDL
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	// Assume mouse movement is usually between -500 and +500
	const int maxMouseSpeed = 500;
	// Rotation/sec at maximum speed
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		// Convert to ~[-1.0, 1.0]
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// Multiply by rotation/sec
		angularSpeed *= maxAngularSpeed;
	}
	mMoveComp->SetAngularSpeed(angularSpeed);

	// Compute pitch
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		// Convert to ~[-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::Shoot()
{
	// here i should set the bobber to not have a fish on the line.
	// also make it not possible to shoot if fish is on the line.
	
	if (!(GetGame()->GetReelState()))
	{
		// Get direction vector
		Vector3 start, dir;
		GetGame()->GetRenderer()->GetScreenDirection(start, dir);
		// Spawn a ball
		BobberActor* ball = GetGame()->GetBobber(); // This line
		//this->GetGame()->AddBobber(ball); // Adds bobber to list of bobbers - Rebecca Morris
		//ball->FishOff(); // this resets the isFishOn variable to false.
		ball->setBobberFishOn(false);
		ball->SetPlayer(this);
		ball->SetPosition(start + dir * 20.0f);
		// Rotate the ball to face new direction
		ball->RotateToNewForward(dir);
		// Play shooting sound
		ball->SetMyMoveSpeed();
		Vector3 stopVelocity(0.0, 0.0, 0.0); // sets the beginning 
		ball->SetForwardVelocity(stopVelocity);
		ball->OutOfWater();
		mAudioComp->PlayEvent("event:/CastLine");
		//this->GetGame()->StopReeling();
	}
}

void FPSActor::SetFootstepSurface(float value)
{
	// Pause here because the way I setup the parameter in FMOD - Rebecca Morris
	// changing it will play a footstep
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	//mMeshComp->SetVisible(visible);
}

void FPSActor::FixFPSCollisions() //Adam this is where we collide with the pond - Rebecca Morris
{
	Vector3 pos = GetPosition();
	if (pos.y >= 150.0f)
	{
		pos.y = 149.9f;
		SetPosition(pos);
	}
}

