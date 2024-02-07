// ----------------------------------------------------------------
// Written by Will Briggs 
// 2023-3-7
// This supports scenery (or characters, if they aren't supposed to move)
// To make it move, add a MoveComponent
// ----------------------------------------------------------------

#pragma once

#include <vector>
#include "Actor.h"

class Game;
class MeshComponent;

class Scenery: public Actor
{
public:
	Scenery(::Game* game, const char* modelFileName, const char* textureFileName=nullptr);
	Scenery(const Scenery&) = delete;
	~Scenery() = default;

	bool Update(float dt) {}
private:
	std::vector<MeshComponent*> meshComponents_;
};
