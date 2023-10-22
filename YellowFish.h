#pragma once
#include "Actor.h"

class YellowFish : public Actor
{
public:
	YellowFish(class Game* game);

	void UpdateActor(float deltaTime) override;
	void GetOnLine(); // Rebecca Morris
	void FixCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);
	void SetPlayer(Actor* player) { mPlayer = player; } // Rebecca Morris
	float GetYellowFishTimer() { return yellowFishTimer; }
	void SetYellowFishTimer(float newTimer);

private:
	class BoxComponent* mBoxComp;
	float angularMovement;
	float forwardMovement;
	float yellowFishTimer;
	class MoveComponent* mMoveComp;
	class Actor* mPlayer; // Rebecca Morris
	// here is where I will have to put the movement parts
};