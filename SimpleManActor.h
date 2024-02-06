// ----------------------------------------------------------------
// Written by Will Briggs and whoever wrote the Assimp library tester program I'm using
//   see if I can track down a name!
// 2023-3-7
// This is a 3d model of a man. He's supposed to walk, but this code doesn't support that yet.
// ----------------------------------------------------------------

#pragma once

#include <vector>
#include "Actor.h"

class Game;
class MoveComponent;
class MeshComponent;

class SimpleManActor: public Actor
{
public:
	SimpleManActor(::Game* game);
	SimpleManActor(const SimpleManActor&) = delete;
	~SimpleManActor() = default;

	bool Update(float dt);
protected:
	float rotationSpeed_; //from assimp lib guy I think
	float rotationAngle_; //from assimp lib guy I think
	MoveComponent* mMoveComp;

	std::vector<MeshComponent*> meshComponents_;
};
