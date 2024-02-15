// Header for the Rod

#pragma once
#include "Actor.h"

class MeshComponent;

class RodActor : public Actor
{
public:
	RodActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void FixCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);
	void SetPlayer(Actor* player) { mPlayer = player; } // Rebecca Morris

private:
	class BoxComponent* mBoxComp;
	float angularMovement;
	float forwardMovement;
	float fishTimer;
	class MoveComponent* mMoveComp;
	class Actor* mPlayer; // Rebecca Morris
	bool isOnLine;
	bool isCaught;
	// here is where I will have to put the movement parts
	float fishDistance;
	Vector3 fishOnLineStartPosition;
	std::vector<MeshComponent*> meshComponents_;
};