//Edited version of BobberMove
//Cobbled together by Adam Caligiuri
//class exists to handle collisions, the same way Madhavs ball does.

#include "HookMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "Hook.h"
#include "BasicFish.h"
#include "FPSActor.h"
#include "BobberActor.h"

HookMove::HookMove(Actor* owner) :MoveComponent(owner)
{

}

void HookMove::Update(float deltaTime)
{

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	Hook* hook = mOwner->GetGame()->GetHook();
	BobberActor* bobber = mOwner->GetGame()->GetBobber();
	Vector3 bobberPosition = bobber->GetPosition();

	std::vector<class BasicFish*> otherFishArray = mOwner->GetGame()->GetBasicFishes();
	for (BasicFish* fish : otherFishArray)
	{
		// Construct segment in direction of travel
		const float segmentLength = 30.0f;
		Vector3 start = fish->GetPosition();
		Vector3 dir = fish->GetForward();
		Vector3 end = start + dir * segmentLength;

		// Create line segment
		LineSegment l(start, end);

		//This was the big bug we had, I changed it so that the fish was the thing moving, and the hook was the "box" Adam Caligiuri
		// when a fish moves through or past the hook it is considered a collision and it then gets hooked
		if (phys->SegmentCast(l, info) && info.mActor == hook)
		{
			//Can we verify that if bobber think's there's a fish on, at least/exactly one fish thinks it's caught?
			//typeid(*fish) == typeid(BasicFish) && 
			if (fish->GetState() == Actor::EActive && !bobber->GetFishOnStatus())
			{
				fish->GetOnLine();
				mOwner->GetGame()->SetCurrentFish(fish);
				bobber->FishOn(); //WSB 2024-03-25 What's this "20.0f"?
				Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
				bobber->SetPosition(newBobberPosition);
			}
		}

		BasicFish* caughtFish = mOwner->GetGame()->GetCurrentFish();
		if (caughtFish && caughtFish->GetLineStatus() && caughtFish->GetState() == Actor::EActive) // this handles the fishes tension
		{
			// make the fish face bobber instead of the player.
			// Once the fish is on the line we have it face away from teh bobber as if its trying to break free. Adam Caligiuri
			Vector3 bobberPos = bobber->GetPosition();
			Vector3 bobberOppPlayer = mOwner->GetGame()->GetPlayer()->GetPosition() - bobberPos;
			Vector3 fishOppBobber = bobberPos - caughtFish->GetPosition();
			bobberOppPlayer.Normalize();
			fishOppBobber.Normalize();
			bobberOppPlayer.Reverse(); // Adam example of inversing a vector3
			bobber->RotateToNewForward(bobberOppPlayer);
			caughtFish->RotateToNewForward(fishOppBobber);

			float fishHookedSpeed = caughtFish->getHookedSpeed();

			caughtFish->SetMovementSpeed(fishHookedSpeed);
			bobber->SetTensionSpeed(-fishHookedSpeed);

			//bobber->SetTensionSpeed(20.0);
			//caughtFish->SetMovementSpeed(-20.0); //WSB 2024-03-25 make this fish->iAmHookedSpeed
		}
	}
}
