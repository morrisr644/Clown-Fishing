// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"

class FishOnScreen : public UIScreen
{
public:
	FishOnScreen(class Game* game);
	~FishOnScreen();
	//float startTime;
	float TimePassed;

	void Update(float deltaTime);
};
