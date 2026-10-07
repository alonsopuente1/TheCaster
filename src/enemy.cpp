#include "enemy.hpp"

#include "castengine/world.hpp"
#include "castengine/texture.hpp"
#include "castengine/vec2d.hpp"
#include "castengine/renderer.hpp"
#include "castengine/logger.hpp"

void Enemy::Think()
{
    if(mTimer >= 5000)
    {
        mTimer = 0;

        vec2d playerPos = mWorld.GetPlayerPos();
        Node destNode = { static_cast<int>(playerPos.x), static_cast<int>(playerPos.y), -1, -1, 0.0f, 0.0f, 0.0f };

        mAI.SetDestination(destNode, mWorld.GetMap());
    }
}

void Enemy::Update(float dtMs)
{
    mTimer += dtMs;

    Think();

    vec2d targetPos = mAI.GetCurrentTarget();
    vec2d toTarget = targetPos - mPos;
    vec2d desiredVel = toTarget.Normalised() * mMaxSpeed;

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
