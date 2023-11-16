#include "SkyBox.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Actor.h"

SkyBox::SkyBox(Game* game)
	:Actor(game)
{
	Quaternion q;
	Quaternion q2;

	top = new Actor(game);
	q = Quaternion(Vector3::UnitZ, Math::Pi * 2.0f);
	q2 = Quaternion(Vector3::UnitY, Math::Pi);
	top->SetRotation(q.Concatenate(q, q2));
	top->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y, this->GetPosition().z + OFFSET));
	top->SetScale(TEXTURE_SCALE);
	MeshComponent* topMesh = new MeshComponent(top);
	topMesh->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/top.gpmesh"));
	topMesh->SetUsesLighting(true); //WSB 2019-3-29.  Tells MeshComponent:  don't use lighting on this one

	bottom = new Actor(game);
	q = Quaternion(Vector3::UnitZ, Math::Pi);
	bottom->SetRotation(q);
	bottom->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y, this->GetPosition().z - OFFSET));
	bottom->SetScale(TEXTURE_SCALE);
	MeshComponent* bottomMesh = new MeshComponent(bottom);
	bottomMesh->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/bottom.gpmesh"));
	bottomMesh->SetUsesLighting(true); //WSB 2019-3-29

	front = new Actor(game);
	q = Quaternion(Vector3::UnitY, Math::Pi);
	q2 = Quaternion(Vector3::UnitX, Math::PiOver2);
	front->SetRotation(q2.Concatenate(q2, q));
	front->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y + OFFSET, this->GetPosition().z));
	front->SetScale(TEXTURE_SCALE);
	MeshComponent* frontMesh = new MeshComponent(front);
	frontMesh->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/front.gpmesh"));
	frontMesh->SetUsesLighting(true); //WSB 2019-3-29

	// back is flipped
	back = new Actor(game);
	q = Quaternion(Vector3::UnitZ, Math::Pi);
	q2 = Quaternion(Vector3::UnitY, Math::Pi);
	q = q.Concatenate(q, q2);
	q2 = Quaternion(Vector3::UnitX, Math::PiOver2);
	back->SetRotation(q2.Concatenate(q, q2));
	back->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y - OFFSET, this->GetPosition().z));
	back->SetScale(TEXTURE_SCALE);
	MeshComponent* backMesh = new MeshComponent(back);
	backMesh->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/back.gpmesh"));
	backMesh->SetUsesLighting(true); //WSB 2019-3-29

	left = new Actor(game);
	q = Quaternion(Vector3::UnitY, Math::PiOver2);
	q2 = Quaternion(Vector3::UnitX, Math::PiOver2 * 3.0f);
	left->SetRotation(q2.Concatenate(q, q2));
	left->SetPosition(Vector3(this->GetPosition().x - OFFSET, this->GetPosition().y, this->GetPosition().z));
	left->SetScale(TEXTURE_SCALE);
	MeshComponent* leftMesh = new MeshComponent(left);
	leftMesh->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/right.gpmesh"));
	leftMesh->SetUsesLighting(true); //WSB 2019-3-29

	//right is flipped
	right = new Actor(game);
	q = Quaternion(Vector3::UnitZ, Math::PiOver2 * 3.0f);
	q2 = Quaternion(Vector3::UnitX, Math::Pi);
	q = q.Concatenate(q, q2);
	q2 = Quaternion(Vector3::UnitY, Math::PiOver2);
	right->SetRotation(q2.Concatenate(q, q2));
	right->SetPosition(Vector3(this->GetPosition().x + OFFSET, this->GetPosition().y, this->GetPosition().z));
	right->SetScale(TEXTURE_SCALE);
	MeshComponent* rightMesh = new MeshComponent(right);
	rightMesh->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/left.gpmesh"));
	rightMesh->SetUsesLighting(true); //WSB 2019-3-29
}

void SkyBox::UpdateActor(float deltatime)
{
	top->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y, this->GetPosition().z + OFFSET));
	bottom->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y, this->GetPosition().z - OFFSET));
	front->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y + OFFSET, this->GetPosition().z));
	back->SetPosition(Vector3(this->GetPosition().x, this->GetPosition().y - OFFSET, this->GetPosition().z));
	left->SetPosition(Vector3(this->GetPosition().x - OFFSET, this->GetPosition().y, this->GetPosition().z));
	right->SetPosition(Vector3(this->GetPosition().x + OFFSET, this->GetPosition().y, this->GetPosition().z));
}
