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
		//BasicFish* yellowFish = dynamic_cast<BasicFish*>(info.mActor); //Same comment, do we need both or just one? -Rebecca
		//BasicFish* redFish = dynamic_cast<BasicFish*>(info.mActor);
		FPSActor* player = mOwner->GetGame()->GetPlayer();
		BasicFish* yFish = mOwner->GetGame()->GetYellowFish();
		BasicFish* rFish = mOwner->GetGame()->GetRedFish();
		BasicFish* oFish = mOwner->GetGame()->GetOrangeFish();
		BasicFish* gFish = mOwner->GetGame()->GetGreenFish();

		/*const int NUMOFFISH = 2;
		BasicFish* fishArray[NUMOFFISH] = { yFish, rFish };*/
		std::vector<class BasicFish*> otherFishArray = mOwner->GetGame()->GetBasicFishes();
		for (BasicFish* fish : otherFishArray)
		{
			if (fish->GetState() == Actor::EActive && fish == info.mActor)
			{
				if (fish && !bobber->GetFishOnStatus())
				{
					fish->GetOnLine();
					bobber->FishOn();
					Vector3 newBobberPosition = Vector3(bobberPosition.x, bobberPosition.y, bobberPosition.z - 20.0f);
					bobber->SetPosition(newBobberPosition);
				}

				if (fish->GetLineStatus() && fish->GetState() == Actor::EActive) // this handles the fishes tension
				{
					// make the fish face bobber instead of the player.
					// get fish to reflect off of wall.
					Vector3 playerPos = player->GetPosition();
					Vector3 bobberPos = bobber->GetPosition();
					Vector3 fishPos = fish->GetPosition();
					Vector3 bobberOppPlayer = playerPos - bobberPos;
					Vector3 fishOppBobber = bobberPos - fishPos;
					bobberOppPlayer.Normalize();
					fishOppBobber.Normalize();
					bobberOppPlayer.Reverse(); // Adam example of inversing a vector3
					bobber->RotateToNewForward(bobberOppPlayer);
					fish->RotateToNewForward(fishOppBobber);

					float fishHookedSpeed = fish->getHookedSpeed();
					
					fish->SetMovementSpeed(fishHookedSpeed);
					bobber->SetTensionSpeed(-fishHookedSpeed);

					//if (fish == rFish) // red fish is easer to catch
					//{
					//	bobber->SetTensionSpeed(20.0);
					//	rFish->SetMovementSpeed(-20.0);
					//}
					//else if (fish == yFish)
					//{
					//	bobber->SetTensionSpeed(30.0);
					//	fish->SetMovementSpeed(-30.0);
					//}
					//else
					//{
					//	bobber->SetTensionSpeed(20.0);
					//	fish->SetMovementSpeed(-20.0);
					//}
				}
			}
			
		}
	}
}