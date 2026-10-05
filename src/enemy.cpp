#include "enemy.hpp"

#include "castengine/world.hpp"
#include "castengine/texture.hpp"
#include "castengine/vec2d.hpp"
#include "castengine/renderer.hpp"
#include "castengine/logger.hpp"

void Enemy::Think()
{
    // pathfind to player
    vec2d playerPos = mWorld.GetPlayerPos();
    vec2d enemyPos = mPos;

    Node startNode = { static_cast<int>(enemyPos.x), static_cast<int>(enemyPos.y), -1, -1, 0.0f, 0.0f, 0.0f };
    Node destNode = { static_cast<int>(playerPos.x), static_cast<int>(playerPos.y), -1, -1, 0.0f, 0.0f, 0.0f };

    mPath = aStar(startNode, destNode, mWorld.GetMap());

    if(mPath.empty())
    {
        mAcc = vec2d(0.0f);
        mVel = vec2d::AngToVec(mDir);
        return;
    }

    vec2d targetPos = vec2d(static_cast<float>(mPath.front().x) + 0.5f, static_cast<float>(mPath.front().y) + 0.5f);
    vec2d toTarget = targetPos - mPos;

    if(toTarget.GetMagnitude() < 0.5f)
    {
        mPath.erase(mPath.begin());
        toTarget = vec2d(static_cast<float>(mPath.front().x) + 0.5f, static_cast<float>(mPath.front().y) + 0.5f);
    }
    
    vec2d desiredAcc = toTarget.Normalised() * mMaxSpeed;
    mAcc = desiredAcc;
}

void Enemy::Update(float dtMs)
{
    Think();

    mVel += mAcc * dtMs;
    if(mVel.GetMagnitude() > mMaxSpeed)
        mVel.SetMagnitude(mMaxSpeed);

    AttemptMove(mPos + (mVel * dtMs));

    mDir += 0.01f * dtMs;
    while(mDir > M_PI * 2.f)
        mDir -= M_PI * 2.f;
}

void Enemy::Draw(CastEngine::Renderer& render)
{
    render.RenderSprite(mTex, mPos);
}
