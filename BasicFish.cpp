//Start of the basic fish for the game.
//We are starting by making the basic fish a car model for the time being.

#include "BasicFish.h"
#include "Game.h"
#include "Actor.h"
#include "MobileActor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "PlaneActor.h"
#include "MoveComponent.h"
#include "BobberActor.h"
#include "PhysWorld.h"
#include "ScreenSaysFishOn.h"
#include "Texture.h"
#include "AssimpLoad.h"
#include <string.h>
#include "TargetComponent.h"


BasicFish::BasicFish(Game* game, char color)
	:Actor(game)
	, fishDistance(900.0) // Rebecca you can edit this value as you see fit. Yellow fish has one too.
	, fishOnLineStartPosition(0.0, 0.0, 0.0)
	, mColor(color)
	, angularMovement(0.3)
	, forwardMovement(100)
	, fishTimer(2.0)
	, hookedSpeed(-20.0) // This is a member variable now that can be changed as we create the fish
{
	SetScale(5.0f);

	new TargetComponent(this);

	Texture* texture = new Texture;

	std::vector<Mesh*> meshes;

	switch (mColor)
	{
		case 'r':
		{
			fishDistance = 800.0;
			fishTimer = 1.0;
			texture->Load("Assets/models/RedFish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/RedFish.obj", texture);
			hookedSpeed = -10.0f;
			break; 
		}
		case 'y':
		{
			fishDistance = 600.0;
			texture->Load("Assets/models/YellowFish.jpg");
			LoadAssimpMeshes(meshes, game, "Assets/models/YellowFish.obj", texture);
			hookedSpeed = -30.0f;
			break; 
		}
		case 'o':
		{

			fishDistance = 600.0;
			texture->Load("Assets/models/OrangeFish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/OrangeFish.obj", texture);
			hookedSpeed = -30.0f;
			break; 
		}
		case 'g':
		{

			fishDistance = 600.0;
			texture->Load("Assets/models/GreenFish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/GreenFish.obj", texture);
			hookedSpeed = -30.0f;
			break;
		}
		case 'b':
		{
			fishDistance = 600.0;
			texture->Load("Assets/models/BlueFish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/BlueFish.obj", texture);
			break;
		}
		case 'p':
		{
			fishDistance = 600.0;
			texture->Load("Assets/models/PurpleFish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/PurpleFish.obj", texture);
			hookedSpeed = -40.0f;
			break;
		}
		//K is pink because we already have p
		case 'k':
		{
			fishDistance = 600.0;
			texture->Load("Assets/models/Pinkfish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/PinkFish.obj", texture);
			break;
		}
		//S is for spotted
		case 's':
		{
			fishDistance = 500.0;
			texture->Load("Assets/models/PolkaDotFish.png");
			LoadAssimpMeshes(meshes, game, "Assets/models/PolkaDotFish.obj", texture);
			hookedSpeed = -60.0;
			break;
		}
	}

	//Texture* texture = new Texture; texture->Load("Assets/models/Redfish.png");
	//Texture* texture = new Texture; texture->Load(textureFileName);
	setSpecular(meshes, 30000.0);

	for (auto&& mesh : meshes)
	{
		MeshComponent* mc = new MeshComponent(this); mc->SetMesh(mesh);
		meshComponents_.push_back(mc);
		mBoxComp = new BoxComponent(this);
		mBoxComp->SetObjectBox(mc->GetMeshComp()->GetBox());
	}


	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(forwardMovement);
	mMoveComp->SetAngularSpeed(angularMovement);

	isOnLine = false;
	isCaught = false;
	isFleeing = false;

	GetGame()->AddBasicFish(this);
}

void BasicFish::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();
	Vector3 currentPos = this->GetPosition();
	constexpr float CATCHYPOS = 330.0;
	if (isOnLine && currentPos.y <= CATCHYPOS) // This is here so the fish get caught a bit earlier than intersecting with the wall
	{
		// If the fish collides with any of the walls, the player is no longer reeling it in
		// It either got away or was caught Adam Caligiuri

		isCaught = true;
		isOnLine = false;
		GetGame()->StopReeling();
		GetGame()->JustCaughtFish();

	}

	constexpr float LOSEZPOS = -700.0;
	if (isOnLine && currentPos.z <= LOSEZPOS) // This is here so the fish get caught a bit earlier than intersecting with the wall
	{
		// If the fish collides with any of the walls, the player is no longer reeling it in
		// It either got away or was caught

		isCaught = false;
		isOnLine = false;

		GetGame()->StopReeling();
		GetGame()->GetBobber()->FishOff(this);

	}
	

}


void BasicFish::GetOnLine() // Rebecca Morris
{
	// When the fish is hit then stop moving (Caught)
	SetMovementSpeed(0.0f);
	//SetAngularSpeed(0.0f);
	this->GetGame()->StartReeling();
	isOnLine = true;
	// here maybe drop z by 10???
	Vector3 currPos = this->GetPosition();
	Vector3 currPosZDown = currPos;
	currPosZDown.z = currPosZDown.z - 10;
	this->SetPosition(currPosZDown);
	SetOnLinePosition();

	fishDistance = 800.0f; //Made the game too hard :(

	new ScreenSaysFishOn(this->GetGame());
	this->GetGame()->TurnScreenSaysFishOnOn();
}

void BasicFish::FixCollisions() // uses the collisions not present in the regular fixCollisions function
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// Construct segment in direction of travel
			const float segmentLength = 30.0f;
			Vector3 start = this->GetPosition();
			Vector3 dir = this->GetForward();
			Vector3 end = start + dir * segmentLength;

			// Create line segment
			LineSegment l(start, end);

			// Test segment vs world
			PhysWorld* phys = this->GetGame()->GetPhysWorld();
			PhysWorld::CollisionInfo info;

			if (phys->SegmentCast(l, info))
			{
				dir = Vector3::Reflect(dir, info.mNormal);
				this->RotateToNewForward(dir);
			}
		}
	}
}

void BasicFish::SetAngularSpeed(float newSpeed)
{
	//angularMovement = newSpeed;
	mMoveComp->SetAngularSpeed(newSpeed);
}

void BasicFish::SetMovementSpeed(float newMovementSpeed)
{
	//forwardMovement = newMovementSpeed;
	mMoveComp->SetForwardSpeed(newMovementSpeed);
}


void BasicFish::SetFishTimer(float newTimer)
{
	fishTimer = newTimer;
}

void BasicFish::SetOnLinePosition()
{
	//cant use switch statement because mColor is not an int
	//Yes you can i was dumb, change later

	switch (mColor)
	{
		case 'r':
			fishOnLineStartPosition = GetGame()->GetRedFish()->GetPosition();
			break;
		case 'o':
			fishOnLineStartPosition = GetGame()->GetOrangeFish()->GetPosition();
			break;
		case 'y':
			fishOnLineStartPosition = GetGame()->GetYellowFish()->GetPosition();
			break;
		case 'g':
			fishOnLineStartPosition = GetGame()->GetGreenFish()->GetPosition();
			break;
		case 'b':
			fishOnLineStartPosition = GetGame()->GetBlueFish()->GetPosition();
			break;
		case 'p':
			fishOnLineStartPosition = GetGame()->GetPurpleFish()->GetPosition();
			break;
		case 'k':
			fishOnLineStartPosition = GetGame()->GetPinkFish()->GetPosition();
			break;
		case 's':
			fishOnLineStartPosition = GetGame()->GetSpottedFish()->GetPosition();
			break;
	}

}


