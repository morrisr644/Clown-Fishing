// Header for the BasicFish

#pragma once
#include "Actor.h"

class BasicFish : public Actor
{
public:
	BasicFish(class Game* game);

	void UpdateActor(float deltaTime) override;
	void GetOnLine(); // Rebecca Morris
	void FixCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);
	void SetPlayer(Actor* player) { mPlayer = player; } // Rebecca Morris

private:
	class BoxComponent* mBoxComp;
	float angularMovement;
	float forwardMovement;
	class MoveComponent* mMoveComp;
	class Actor* mPlayer; // Rebecca Morris
	// here is where I will have to put the movement parts
};