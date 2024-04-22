// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Header file for the plane actor - Rebecca Morris

#pragma once
#include "Actor.h"
#include <string>

class PlaneActor : public Actor
{
public:
	PlaneActor(class Game* game, const std::string& fileName);
	~PlaneActor();
	class BoxComponent* GetBox() { return mBox; }
private:
	class BoxComponent* mBox;
};
