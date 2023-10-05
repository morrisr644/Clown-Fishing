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

BobberActor::BobberActor(Game* game)
	:Actor(game)
	,mLifeSpan(2.0f)
	,mLaunchAngle(0.0)
{
	//SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"); // Sphere has been recolored to look more like a bobber (RCM)
	mc->SetMesh(mesh);
	mMyMove = new BobberMove(this);
	mMyMove->SetForwardSpeed(1500.0f);
	mAudioComp = new AudioComponent(this);
}

void BobberActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	float gravity = 9.81;

	mLifeSpan -= deltaTime;

	//Forward Speed = Forward/Backward = X Axis
	//Angular Speed = Up/Down = Z Axis
	//Strafe Speed = Left/Right = Y Axis

	//newSpeed = 1/2(gravity)(deltaTime)(deltaTime) + (Velocity)(deltaTime) + currentSpeed

	// dx = endPosition.x - startPosition.x
	// dy = endPosition.y - startPosition.y
	// dz = endPosition.z - startPosition.z

	// d = sqrt(dx*dx + dy*dy + dz*dz)

	// This is the velocity vector
	// vx = dx/d * currentSpeed
	// vy = dy/d * currentSpeed
	// vz = dz/d * currentSpeed
	/*Vector3 currPosition = GetGame()->GetBobber()->GetPosition();
	currPosition.Normalize();
	float launchAngleInRadians = asin(currPosition.z);
	SetLaunchAngle(launchAngleInRadians);

	Vector3 currentPosition = this->GetPosition(); // this doesnt work
	//Vector3 currPosition = GetGame()->GetBobber()->GetPosition(); 
	Vector3 appliedMath = currentPosition;
	appliedMath.x = currentPosition.x + 10 * cos(mLaunchAngle) * deltaTime;
	appliedMath.y = currentPosition.y + 10 * sin(mLaunchAngle) * deltaTime;
	appliedMath.z = currentPosition.z + 10 * sin(mLaunchAngle) * deltaTime - (0.5 * 9.81 * deltaTime * deltaTime);
	this->SetPosition(appliedMath);*/
	
	if (mMyMove->GetForwardSpeed() != 0)
	{
		//Vector3 currPosition = GetGame()->GetBobber()->GetPosition();
		//currPosition.z = currPosition.z * .981;
		//GetGame()->GetBobber()->SetPosition(currPosition);
		Vector3 currPosition = GetGame()->GetBobber()->GetPosition();
		currPosition.Normalize();
		float launchAngleInRadians = sin(currPosition.z);
		SetLaunchAngle(launchAngleInRadians);

		Vector3 currentPosition = GetGame()->GetBobber()->GetPosition();
		//Vector3 currPosition = GetGame()->GetBobber()->GetPosition(); 
		Vector3 appliedMath = currentPosition;
		appliedMath.x = currentPosition.x;//+1000 * cos(mLaunchAngle) * deltaTime;
		appliedMath.y = currentPosition.y;// +1000 * sin(mLaunchAngle) * deltaTime;
		appliedMath.z = currentPosition.z + 1000 * sin(mLaunchAngle) * deltaTime - (0.5 * 9.81 * deltaTime * deltaTime);
		GetGame()->GetBobber()->SetPosition(appliedMath);
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

void BobberActor::SetLaunchAngle(float newAngle)
{
	mLaunchAngle = newAngle;
}

void BobberActor::SetMyMoveSpeed()
{
	mMyMove->SetForwardSpeed(1500.0f);
}

