// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"

class TensionMeter : public UIScreen
{
public:
	TensionMeter(class Game* game);
	~TensionMeter();
	//float startTime;
	int TensionLevel;
	class Texture* mTensionBar;

	void Draw(class Shader* shader) override;

	void Update(float deltaTime);
};
