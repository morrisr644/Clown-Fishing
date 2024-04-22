// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Header file for the fish has gotten off the line screen - Rebecca Morris

#pragma once
#include "UIScreen.h"

class ScreenSaysFishOff : public UIScreen
{
public:
	ScreenSaysFishOff(class Game* game);
	~ScreenSaysFishOff();
	//float startTime;
	float TimePassed;

	void Update(float deltaTime);
};
