// Header for the BasicFish

#pragma once
#include "Actor.h"
#include "BasicFish.h"

class MeshComponent;

class RedFish : public BasicFish
{
public:
	RedFish(class Game* game);

	void UpdateActor(float deltaTime) override;
	void GetOnLine(); // Rebecca Morris
	void FixCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);
	void SetPlayer(Actor* player) { mPlayer = player; } // Rebecca Morris
	float GetFishTimer() { return fishTimer; }
	bool GetLineStatus() { return isOnLine; }
	void SetLineStatus(bool status) { isOnLine = status; }
	bool GetCatchStatus() { return isCaught; }
	void SetFishTimer(float newTimer);
	bool GetFleeingStatus() { return isFleeing; }
	void SetFleeingStatus(bool fleeingStatus) { isFleeing = fleeingStatus; }
	void SetOnLinePosition();
	Vector3 GetOnLinePosition() { return fishOnLineStartPosition; }
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
	bool isFleeing;
	float fishDistance;
	Vector3 fishOnLineStartPosition;
	std::vector<MeshComponent*> meshComponents_;
	// here is where I will have to put the movement parts
};