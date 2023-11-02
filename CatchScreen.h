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

class CatchScreen : public UIScreen
{
public:
	CatchScreen(class Game* game);
	~CatchScreen();

	void HandleKeyPress(int key) override;
};
