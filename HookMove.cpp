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
				bobber->FishOn(); // let the game know a fish is on the bobber
				Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
				// the minus 20.0f is just an arbitrary number, we use it to move the bobber under water so it makes it seem like the fish pulled the bobber under
				bobber->SetPosition(newBobberPosition); // sets the bobbers position to under the water
			}
		}

		BasicFish* caughtFish = mOwner->GetGame()->GetCurrentFish();
		if (caughtFish && caughtFish->GetLineStatus() && caughtFish->GetState() == Actor::EActive) // this handles the fishes tension
		{
			// make the fish face bobber instead of the player.
			// Once the fish is on the line we have it face away from teh bobber as if its trying to break free. Adam Caligiuri
			Vector3 bobberPos = bobber->GetPosition(); // grabs the bobbers position
			Vector3 bobberOppPlayer = mOwner->GetGame()->GetPlayer()->GetPosition() - bobberPos; // subtract the players position from the bobbers position
			Vector3 fishOppBobber = bobberPos - caughtFish->GetPosition(); // get the direction of the fishes current position to the bobber
			bobberOppPlayer.Normalize();
			fishOppBobber.Normalize();
			bobberOppPlayer.Reverse(); 
			bobber->RotateToNewForward(bobberOppPlayer); // turn the bobber to face the player
			caughtFish->RotateToNewForward(fishOppBobber); // turns the fish to face the bobber, this makes it look like its pulling away from where the line would be

			float fishHookedSpeed = caughtFish->getHookedSpeed();

			caughtFish->SetMovementSpeed(fishHookedSpeed); // this is the speed that the fish moves away from the bobber
			bobber->SetTensionSpeed(-fishHookedSpeed); // the bobber also moves away from the player slowly, making it seem like the fish is pulling the bobber away.
		}
	}
}
