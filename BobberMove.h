// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Previously BallMove.h

#pragma once
#include "MoveComponent.h"

class BobberMove : public MoveComponent
{
public:
	BobberMove(class Actor* owner);

	void SetPlayer(Actor* player) { mPlayer = player; }
	void Update(float deltaTime) override;
protected:
	class Actor* mPlayer;
};