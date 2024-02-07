// ----------------------------------------------------------------
// Written by Will Briggs, Greg Kopack, John Akpinar, and whoever wrote the Assimp library tester program I'm using
//   see if I can track down a name!
// 2023-3-7
// This is an "icosphere," a multisided 3D shape
// ----------------------------------------------------------------

#pragma once

#include "Actor.h"

class Mesh;
class MoveComponent;

class IcosphereActor: public Actor
{
public:
	IcosphereActor(::Game* game);
	IcosphereActor(const IcosphereActor&) = delete;
	~IcosphereActor() = default;

	bool Update(float dt);
		
protected:
	float rotationSpeed_;
	float rotationAngle_;

	MoveComponent* mMoveComp;

	Mesh* CreateIcosphereMesh();
};


