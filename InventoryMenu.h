// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"

class InventoryMenu : public UIScreen
{
public:
	InventoryMenu(class Game* game);
	~InventoryMenu();

	void HandleKeyPress(int key) override;
};
