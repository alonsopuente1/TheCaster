#pragma once

#include "castengine/entity.hpp"
#include "astar.hpp"

class EntityAI
{

public:

    EntityAI(CastEngine::Entity& parentEntity) : mParentEntity(parentEntity) {}

    void SetDestination(const Node& destNode, const CastEngine::Map& map)
    {
        Node startNode = { static_cast<int>(mParentEntity.GetPos().x), static_cast<int>(mParentEntity.GetPos().y), -1, -1, 0.0f, 0.0f, 0.0f };  
        
        mPath = aStar(startNode, destNode, map);
        
        if(!mPath.empty())
            mPathIndex = 0;
        else
            mPathIndex = -1;
    }

    /// @brief used to get the current target position of the entity, which is the next node in the path
    /// @return The position of the current target
    vec2d GetCurrentTarget()
    {
        if(mPath.size() == 0)
            return mParentEntity.GetPos();

        vec2d currentTarget = GetNodePos(mPath[mPathIndex]);

        vec2d toTarget = currentTarget - mParentEntity.GetPos();

        if(toTarget.GetMagnitude() < 0.3f)
            mPathIndex++;

        if(mPathIndex >= 0 && mPathIndex < static_cast<int>(mPath.size()))
            return GetNodePos(mPath[mPathIndex]);
        else
            return mParentEntity.GetPos();
    }



private:

    std::vector<Node> mPath;
    int mPathIndex = -1;

    CastEngine::Entity& mParentEntity;

    vec2d GetNodePos(const Node& node)
    {
        return vec2d(static_cast<float>(node.x) + 0.5f, static_cast<float>(node.y) + 0.5f);
    }

};