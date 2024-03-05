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

    //void Collisions(Vector3 pos, const AABB& playerBox, const AABB& planeBox);

private:

    float mScale;
    bool mRecomputeWorldTransform;
    class BoxComponent* mBoxComp;

};

