//Edited version of BobberActor
//Cobbled together by Adam Caligiuri

#include "BobberActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BobberMove.h"
#include "AudioComponent.h"
#include "Math.h"
#include "BasicFish.h"
#include "YellowFish.h"
#include "RedFish.h"
#include "FishOffScreen.h"
#include "Hook.h"

Hook::Hook(Game* game)
	:Actor(game)
	
{
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/HookSphere.gpmesh"); // just a placeholder for testing currently
	mc->SetMesh(mesh);
}

void Hook::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}
