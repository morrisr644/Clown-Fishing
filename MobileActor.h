#pragma once
#include "Actor.h"
class MobileActor :
    public Actor
{
public:
    MobileActor(class Game* game);
    void SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true; }
    void UpdateActor(float deltaTime);
    void FixCollisions();

private:

    float mScale;
    bool mRecomputeWorldTransform;
    class BoxComponent* mBoxComp;

};

