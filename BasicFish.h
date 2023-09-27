// Header for the BasicFish

#pragma once
#include "Actor.h"

class BasicFish : public Actor
{
public:
	BasicFish(class Game* game);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);
	void FixCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);

private:
	class BoxComponent* mBoxComp;
	float angularMovement;
	float forwardMovement;
	class MoveComponent* mMoveComp;
	// here is where I will have to put the movement parts
};