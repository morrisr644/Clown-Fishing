// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InvisiblePlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

InvisiblePlaneActor::InvisiblePlaneActor(Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	//What would happen if we didn't add a mesh at all?
	//
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/InvisiblePlane.gpmesh");
	mc->SetMesh(mesh);
	// Add collision box
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(mesh->GetBox());

	game->AddInvisiblePlane(this);
}

InvisiblePlaneActor::~InvisiblePlaneActor()
{
	GetGame()->RemoveInvisiblePlane(this);
}
