// ----------------------------------------------------------------
// Written by Will Briggs
// 2023-3-7
// This is a 3d model of a man. He's supposed to walk, but this code doesn't support that yet.
// ----------------------------------------------------------------

#include "SimpleManActor.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL/SDL_log.h> //for SDL_Log, which reports errors
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "MoveComponent.h"
#include "AssimpLoad.h"

SimpleManActor::SimpleManActor(::Game* game)
	: Actor(game)
	, rotationSpeed_(1.1f)
	, rotationAngle_(0.0f)
{
	SetScale   (0.55f);

	Texture* texture = new Texture; texture->Load("Assets/man-skin.png");
	std::vector<Mesh*> meshes; LoadAssimpMeshes(meshes, game, "Assets/SimpleMan2.6.fbx", texture);
	setSpecular(meshes, 30000.0); // we don't want a shiny man :)
											  // apparently high numbers -> less shiny
	for (auto&& mesh : meshes)
	{
		MeshComponent* mc = new MeshComponent(this); mc->SetMesh(mesh);
		meshComponents_.push_back(mc);
	}

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetAngularSpeed(rotationSpeed_);
}

bool SimpleManActor::Update(float dt) 
{
	rotationAngle_ += rotationSpeed_ * dt;
	while (rotationAngle_ > 2.f * Math::Pi)
		rotationAngle_ -= 2.f * Math::Pi;

	SetRotation(::Quaternion(Vector3::UnitZ, rotationAngle_));

	return true;
}




