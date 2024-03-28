// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include "BasicFish.h"
#include "AudioComponent.h"

class ScreenSaysFishCaught : public UIScreen
{
public:
	ScreenSaysFishCaught(class Game* game);
	~ScreenSaysFishCaught();

	void HandleKeyPress(int key) override;
};
