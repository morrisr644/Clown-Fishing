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
	float GetFishTimer() { return fishTimer; }
	bool GetLineStatus() { return isOnLine; }
	bool GetCatchStatus() { return isCaught; }
	void SetFishTimer(float newTimer);
	float GetFishDistance() { return fishDistance; }
	void SetFishDistance(float newDistance) { fishDistance = newDistance + fishDistance; }

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
};