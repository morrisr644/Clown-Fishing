//Start of the basic fish for the game.
//We are starting by making the basic fish a car model for the time being.

#include "BasicFish.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

BasicFish::BasicFish(Game* game)
	:Actor(game)
{
	SetScale(0.5f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh");
	mc->SetMesh(mesh);
}

void BasicFish::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

void BasicFish::SetPlayer(Actor* player)
{
}
