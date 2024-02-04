//Edited version of BobberMove
//Cobbled together by Adam Caligiuri
//class exists to handle collisions, the same way Madhavs ball does.

#include "HookMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "Hook.h"
#include "BasicFish.h"
#include "YellowFish.h"
#include "RedFish.h"
#include "WaterPlaneActor.h"
#include "InvisiblePlaneActor.h"
#include "FPSActor.h"
#include "BobberActor.h"

HookMove::HookMove(Actor* owner) :MoveComponent(owner)
{

}

void HookMove::Update(float deltaTime)
{
	// Construct segment in direction of travel
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// Create line segment
	LineSegment l(start, end);


	// Test segment vs world

	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	Hook* hook = mOwner->GetGame()->GetHook();
	BobberActor* bobber = mOwner->GetGame()->GetBobber();
	Vector3 bobberPosition = bobber->GetPosition();

	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{
		YellowFish* yellowFish = dynamic_cast<YellowFish*>(info.mActor);
		RedFish* redFish = dynamic_cast<RedFish*>(info.mActor);

		if (yellowFish && !bobber->GetFishOnStatus())
		{
			yellowFish->GetOnLine();
			bobber->FishOn();
			Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
			bobber->SetPosition(newBobberPosition);
		}
		if (redFish && !bobber->GetFishOnStatus())
		{
			redFish->GetOnLine();
			bobber->FishOn();
			Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
			bobber->SetPosition(newBobberPosition);
		}
	}
}