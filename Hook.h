//Edited version of BobberActor
//Cobbled together by Adam Caligiuri

#pragma once
#include "Actor.h"
#include "AudioComponent.h"

class Hook : public Actor
{
public:
	Hook(class Game* game);
	void UpdateActor(float deltaTime) override;
	void SetCurrPosition(Vector3 newPosition) { currentPosition = newPosition; }
	Vector3 GetCurrPosition() { return currentPosition; }
	void SetPlayer(Actor* player) { mPlayer = player; }

private:
	Vector3 currentPosition;
protected:
	class Actor* mPlayer;
	class HookMove* mMyMove;
};