//Edited version of BobberActor
//Cobbled together by Adam Caligiuri

#include "BobberActor.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BobberMove.h"
#include "AudioComponent.h"
#include "Math.h"
#include "BasicFish.h"
#include "ScreenSaysFishOff.h"
#include "Hook.h"
#include "PhysWorld.h"
#include "FPSActor.h"
#include "HookMove.h"
#include "BoxComponent.h"


Hook::Hook(Game* game)
	:Actor(game)
	// This just handles building the textures of the bobber and loading it into the game. Adam Caligiuri
{
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/HookSphere.gpmesh"); // just a placeholder for testing currently
	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
	mc->SetMesh(mesh);
	mMyMove = new HookMove(this);
}

