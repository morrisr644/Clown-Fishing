// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Header file for Inventory Box - Rebecca Morris

#pragma once
#include "UIScreen.h"

class InventoryBox : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	InventoryBox(class Game* game, const std::string& text, int fish);
	~InventoryBox();
};
