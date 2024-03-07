#pragma once
#include "Actor.h"
class MobileActor :
    public Actor
{
public:
    MobileActor(class Game* game);
    void FixCollisions();

    //void Collisions(Vector3 pos, const AABB& playerBox, const AABB& planeBox);

private:

    float mScale;
    bool mRecomputeWorldTransform;
    class BoxComponent* mBoxComp;

};

