// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// The general actor for all of the planes in the game
// Takes in a filename for the nexture of the plane - Rebecca Morris

#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include <string>

PlaneActor::PlaneActor(Game* game, const std::string& fileName)
	:Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh(fileName);
	mc->SetMesh(mesh);
	// Add collision box
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(mesh->GetBox());

	game->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
	GetGame()->RemovePlane(this);
}
