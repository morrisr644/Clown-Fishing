// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Header file for the fish has gotten on the line screen - Rebecca Morris

#pragma once
#include "UIScreen.h"

class ScreenSaysFishOn : public UIScreen
{
public:
	ScreenSaysFishOn(class Game* game);
	~ScreenSaysFishOn();
	//float startTime;
	float TimePassed;

	void Update(float deltaTime);
};
