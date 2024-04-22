#pragma once
#include "Actor.h"

// Header file for Mobile Actor class - Rebecca Morris

class MobileActor :
    public Actor
{
public:
    MobileActor(class Game* game);
    void FixCollisions();

private:

    float mScale;
    bool mRecomputeWorldTransform;
    class BoxComponent* mBoxComp;

};

