#pragma once
#include "BasicFish.h"

class YellowFish : public BasicFish
{
public:
	YellowFish(class Game* game);

	void UpdateActor(float deltaTime) override;
	void GetOnLine(); // Rebecca Morris
	void FixCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);
	void SetPlayer(Actor* player) { mPlayer = player; } // Rebecca Morris
	float GetFishTimer() { return fishTimer; }
	bool GetLineStatus() { return isOnLine; }
	bool GetCatchStatus() { return isCaught; }
	void SetFishTimer(float newTimer);
	bool GetFleeingStatus() { return isFleeing; }
	void SetFleeingStatus(bool fleeingStatus) { isFleeing = fleeingStatus; }

private:
	class BoxComponent* mBoxComp;
	float angularMovement;
	float forwardMovement;
	float fishTimer;
	class MoveComponent* mMoveComp;
	class Actor* mPlayer; // Rebecca Morris
	bool isOnLine;
	bool isCaught;
	bool isFleeing;
	// here is where I will have to put the movement parts
};