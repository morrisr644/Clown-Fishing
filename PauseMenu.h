// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Header file for pause menu - Rebecca Morris

#pragma once
#include "UIScreen.h"

class PauseMenu : public UIScreen
{
public:
	PauseMenu(class Game* game);
	~PauseMenu();

	void HandleKeyPress(int key) override;
};
