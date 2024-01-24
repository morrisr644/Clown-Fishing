// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"

class FishOffScreen : public UIScreen
{
public:
	FishOffScreen(class Game* game);
	~FishOffScreen();
	//float startTime;
	float TimePassed;

	void Update(float deltaTime);
};
