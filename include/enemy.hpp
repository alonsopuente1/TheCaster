#pragma once

#include "castengine/entity.hpp"
#include "ai.hpp"

namespace CastEngine
{
    class Texture;
}    

class Enemy : public CastEngine::Entity
{

private:

    CastEngine::Texture* mTex;
    float mMaxSpeed;

    // @brief direction of the enemy in radians
    float mDir;
    EntityAI mAI;

    // Timer for enemy, every five seconds, the enemy will update its path to the player. 
    // This is to prevent the enemy from constantly recalculating its path every frame, 
    // which would be inefficient.
    int mTimer;

public:
    
    Enemy(CastEngine::IWorld& world) : CastEngine::Entity(world), mAI(*this) {}
    
    /// @brief thinker function to implement simple AI
    void Think();
    void Update(float dtMs) override;
    void Draw(CastEngine::Renderer& render) override;
    
    inline void SetTexture(CastEngine::Texture* tex) { mTex = tex; }
    inline void SetMaxSpeed(float maxSpeed) { mMaxSpeed = maxSpeed; }

};
