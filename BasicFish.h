// Header for the BasicFish

#pragma once
#include "Actor.h"

class BasicFish : public Actor
{
public:
	BasicFish(class Game* game);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);

private:
	// here is where I will have to put the movement parts
};