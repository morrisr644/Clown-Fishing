// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Header file fot the controls menu

#pragma once
#include "UIScreen.h"

class ControlsMenu : public UIScreen
{
public:
	ControlsMenu(class Game* game);
	~ControlsMenu();
};
