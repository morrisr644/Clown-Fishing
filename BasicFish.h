// Header for the BasicFish

#pragma once
#include "Actor.h"
#include "MobileActor.h"
#include <string.h>

class MeshComponent;

class BasicFish : public Actor
{
public:
	BasicFish(class Game* game, char color);

	void UpdateActor(float deltaTime) override;
	void GetOnLine(); // Rebecca Morris
	void FixCollisions();
	//void FixFishCollisions();
	void SetAngularSpeed(float newSpeed);
	void SetMovementSpeed(float newMovementSpeed);
	//void SetPlayer(Actor* player) { mPlayer = player; } // Rebecca Morris
	float GetFishTimer() { return fishTimer; }
	bool GetLineStatus() { return isOnLine; }
	bool GetCatchStatus() { return isCaught; }
	void SetCatchStatus(bool status) { isCaught = status; }
	void SetFishTimer(float newTimer);
	float GetFishDistance() { return fishDistance; }
	void SetFishDistance(float newDistance) { fishDistance = newDistance + fishDistance; }

	void SetLineStatus(bool status) { isOnLine = status; }
	bool GetFleeingStatus() { return isFleeing; }
	void SetFleeingStatus(bool fleeingStatus) { isFleeing = fleeingStatus; }
	void SetOnLinePosition();
	Vector3 GetOnLinePosition() { return fishOnLineStartPosition; }

	char GetColor() { return mColor; }
	
	const Quaternion& BasicFish::GetRotationForDisplay() const override
	{
		//Makes fish swim in the right direction
		return Quaternion::Concatenate(GetRotation(), Quaternion(Vector3::UnitZ, 210.5));
	}

private:
	class BoxComponent* mBoxComp;
	float angularMovement;
	float forwardMovement;
	float fishTimer;
	class MoveComponent* mMoveComp;
	//class Actor* mPlayer; // Rebecca Morris
	bool isOnLine;
	bool isCaught;
	// here is where I will have to put the movement parts
	float fishDistance;
	Vector3 fishOnLineStartPosition;
	char mColor;

	bool isFleeing;
	std::vector<MeshComponent*> meshComponents_;
};