//Edited version of BobberMove
//Cobbled together by Adam Caligiuri
#pragma once
#include "MoveComponent.h"

class HookMove :public MoveComponent
{
public:
	HookMove(class Actor* owner);

	void SetPlayer(Actor* player) { mPlayer = player; }
	void Update(float deltaTime) override;
protected:
	class Actor* mPlayer;
};
