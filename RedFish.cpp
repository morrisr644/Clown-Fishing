//Start of the basic fish for the game.
//We are starting by making the basic fish a car model for the time being.

#include "BasicFish.h"
#include "RedFish.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "PlaneActor.h"
#include "UnderPlaneActor.h"
#include "InvisiblePlaneActor.h"
#include "MoveComponent.h"
#include "BobberActor.h"
#include "PhysWorld.h"


RedFish::RedFish(Game* game)
	:BasicFish(game)
	, angularMovement(0.2)
	, forwardMovement(200)
	, fishTimer(1.0)
{
	SetScale(0.5f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh");
	mc->SetMesh(mesh);

	//Adding a collision box for the fish
	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(forwardMovement);
	mMoveComp->SetAngularSpeed(angularMovement);

	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
		Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	isOnLine = false;
	isCaught = false;
	isFleeing = false;

	GetGame()->AddBasicFish(this);
}

void RedFish::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();

	PhysWorld::CollisionInfo info;
	
	Vector3 redCurrPosition = this->GetPosition(); //This works but it stalls at the beginning
	if (redCurrPosition.z >= -100.0 )
	{
		//turn the fish around here
		Vector3 turnFishAround = this->GetForward();
		turnFishAround.x = -turnFishAround.x;
		turnFishAround.y = -turnFishAround.y;
		turnFishAround.z = -turnFishAround.z;
		turnFishAround.Normalize();
		Vector3 dir = this->GetForward();
		dir = Vector3::Reflect(dir, info.mNormal);
		this->RotateToNewForward(turnFishAround);
		//this->RotateToNewForward(dir);

	}
	if(redCurrPosition.z <= -600.0)
	{
		//turn around here as well
		Vector3 turnFishAround = this->GetForward();
		turnFishAround.x = -turnFishAround.x;
		turnFishAround.y = -turnFishAround.y;
		turnFishAround.z = -turnFishAround.z;
		turnFishAround.Normalize();
		Vector3 dir = this->GetForward();
		dir = Vector3::Reflect(dir, info.mNormal);
		this->RotateToNewForward(turnFishAround);
		//this->RotateToNewForward(dir);
	}
	if (redCurrPosition.x >= 1250.0f)
	{
		//turn around here as well
		Vector3 turnFishAround = this->GetForward();
		turnFishAround.x = -turnFishAround.x;
		turnFishAround.y = -turnFishAround.y;
		turnFishAround.z = -turnFishAround.z;
		turnFishAround.Normalize();
		Vector3 dir = this->GetForward();
		dir = Vector3::Reflect(dir, info.mNormal);
		this->RotateToNewForward(turnFishAround);
		//this->RotateToNewForward(dir);
	}
	if (redCurrPosition.x <= -1250.0f)
	{
		//turn around here as well
		Vector3 turnFishAround = this->GetForward();
		turnFishAround.x = -turnFishAround.x;
		turnFishAround.y = -turnFishAround.y;
		turnFishAround.z = -turnFishAround.z;
		turnFishAround.Normalize();
		Vector3 dir = this->GetForward();
		dir = Vector3::Reflect(dir, info.mNormal);
		this->RotateToNewForward(turnFishAround);
		//this->RotateToNewForward(dir);
	}
	if (redCurrPosition.y >= 1000.0f)
	{
		//turn around here as well
		Vector3 turnFishAround = this->GetForward();
		turnFishAround.x = -turnFishAround.x;
		turnFishAround.y = -turnFishAround.y;
		turnFishAround.z = -turnFishAround.z;
		turnFishAround.Normalize();
		Vector3 dir = this->GetForward();
		dir = Vector3::Reflect(dir, info.mNormal);
		this->RotateToNewForward(turnFishAround);
		//this->RotateToNewForward(dir);
	}
	if (redCurrPosition.y <= 300.0f) //This one wont work for some reason
	{
		//turn around here as well
		Vector3 turnFishAround = this->GetForward();
		turnFishAround.x = -turnFishAround.x;
		turnFishAround.y = -turnFishAround.y;
		turnFishAround.z = -turnFishAround.z;
		turnFishAround.Normalize();
		Vector3 dir = this->GetForward();
		dir = Vector3::Reflect(dir, info.mNormal);
		this->RotateToNewForward(turnFishAround);
		//this->RotateToNewForward(dir);
	}
	 // Here is the code that SHOULD be causing reflections but isnt working Rebecca.
	// Test segment vs world
	//const float segmentLength = 30.0f;
	//Vector3 start = GetGame()->GetRedFish()->GetPosition();
	//Vector3 dir = GetGame()->GetRedFish()->GetForward();
	//Vector3 end = start + dir * segmentLength;
	//LineSegment l(start, end);
	//PhysWorld* phys = GetGame()->GetPhysWorld();
	//PhysWorld::CollisionInfo info;
	//if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	//{
	//	// If we collided, reflect the ball about the normal
	//	dir = Vector3::Reflect(dir, info.mNormal);
	//	GetGame()->GetRedFish()->RotateToNewForward(dir);
	//}
	
	

}

//void BasicFish::SetPlayer(Actor* player) // why is this here what is this used for
//{
//}

void RedFish::GetOnLine() // Rebecca Morris
{
	// When the fish is hit then stop moving (Caught)
	SetMovementSpeed(0.0f);
	//SetAngularSpeed(0.0f);
	this->GetGame()->StartReeling();
	isOnLine = true;
}

void RedFish::FixCollisions() // pulled from Madhav FPSActor
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetUnderPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			

			if (this->GetLineStatus())
			{
				// If the fish collides with any of the walls, the player is no longer reeling it in
				// It either got away or was caught
				GetGame()->StopReeling();
			}

			// Calculate all our differences
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
			
		}
	}

	auto& invisPlanes = GetGame()->GetInvisiblePlanes();
	for (auto pa : invisPlanes)
	{
		// Do we collide with this PlaneActor?

		const AABB& planeBox = pa->GetBox()->GetWorldBox();

		if (Intersect(playerBox, planeBox))
		{
			

			if (this->GetLineStatus())
			{
				// If the fish collides with any of the walls, the player is no longer reeling it in
				// It either got away or was caught

				isCaught = true;
				isOnLine = false;
				GetGame()->StopReeling();

			}

			// Calculate all our differences
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}

	Vector3 currentPos = this->GetPosition();

	if (isOnLine && currentPos.y <= 300.0)
	{
		// If the fish collides with any of the walls, the player is no longer reeling it in
		// It either got away or was caught

		isCaught = true;
		isOnLine = false;
		GetGame()->StopReeling();

	}
}

void RedFish::SetAngularSpeed(float newSpeed)
{
	//angularMovement = newSpeed;
	mMoveComp->SetAngularSpeed(newSpeed);
}

void RedFish::SetMovementSpeed(float newMovementSpeed)
{
	//forwardMovement = newMovementSpeed;
	mMoveComp->SetForwardSpeed(newMovementSpeed);
}

void RedFish::SetFishTimer(float newTimer)
{
	fishTimer = newTimer;
}
