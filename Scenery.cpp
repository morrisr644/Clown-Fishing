// ----------------------------------------------------------------
// Written by Will Briggs 
// 2023-3-7
// This supports scenery (or characters, if they aren't supposed to move)
// To make it move, add a MoveComponent
// ----------------------------------------------------------------

#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Texture.h"
#include "AssimpLoad.h"
#include "Scenery.h"
#include "Renderer.h"

Scenery::Scenery(::Game* game, const char* modelFileName, const char* textureFileName)
	: Actor(game)
{
	SetScale   (0.55f);

	Texture* texture = nullptr;
	if (textureFileName) 
	{ 
		texture = new Texture; 
		texture = game->GetRenderer()->GetTexture(textureFileName);
		if (texture == nullptr)
			// If it's still null, just use the default texture
			texture = game->GetRenderer()->GetTexture("Assets/Default.png");
	}
	std::vector<Mesh*> meshes; LoadAssimpMeshes(meshes, game, modelFileName, texture);
	setSpecular(meshes, 30000.0); // we don't want shiny scenery (probably) 
								  // apparently high numbers -> less shiny
	for (auto&& mesh : meshes)
	{
		MeshComponent* mc = new MeshComponent(this); mc->SetMesh(mesh);
		meshComponents_.push_back(mc);
	}
}



