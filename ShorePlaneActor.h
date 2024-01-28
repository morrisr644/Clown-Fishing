// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class ShorePlaneActor : public Actor
{
public:
	ShorePlaneActor(class Game* game);
	~ShorePlaneActor();
	class BoxComponent* GetBox() { return mBox; }
private:
	class BoxComponent* mBox;
};
