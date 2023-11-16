#pragma once

#include "PlaneActor.h"

class SkyBox : public Actor
{
public:
	SkyBox(Game* game);
	void UpdateActor(float deltatime);
private:
	enum { TEXTURE_SCALE = 100, OFFSET = TEXTURE_SCALE * 50 };
	Actor* front = NULL;
	Actor* back = NULL;
	Actor* left = NULL;
	Actor* right = NULL;
	Actor* top = NULL;
	Actor* bottom = NULL;
};

