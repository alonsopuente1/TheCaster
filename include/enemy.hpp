#pragma once

#include "castengine/entity.hpp"
#include "astar.hpp"

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

    std::vector<Node> mPath;

public:
    
    using Entity::Entity;
    
    /// @brief thinker function to implement simple AI
    void Think();
    void Update(float dtMs) override;
    void Draw(CastEngine::Renderer& render) override;
    
    inline void SetTexture(CastEngine::Texture* tex) { mTex = tex; }
    inline void SetMaxSpeed(float maxSpeed) { mMaxSpeed = maxSpeed; }

};
