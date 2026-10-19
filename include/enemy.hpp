#pragma once

#include "castengine/spriteentity.hpp"
#include "ai.hpp"

namespace CastEngine
{
    class Texture;
}    

class Enemy : public CastEngine::SpriteEntity
{

private:

    float mMaxSpeed;

    // @brief direction of the enemy in radians
    float mDir;
    EntityAI mAI;

    // Timer for enemy, every five seconds, the enemy will update its path to the player. 
    // This is to prevent the enemy from constantly recalculating its path every frame, 
    // which would be inefficient.
    int mTimer;

public:
    
    Enemy(CastEngine::IWorld& world) : CastEngine::SpriteEntity(world), mAI(*this) {}
    
    /// @brief thinker function to implement simple AI
    void Think();
    void Update(float dtMs) override;
    
    inline void SetMaxSpeed(float maxSpeed) { mMaxSpeed = maxSpeed; }

};
