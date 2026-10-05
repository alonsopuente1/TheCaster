#pragma once

#include "castengine/vec2d.hpp"

namespace CastEngine
{
    class IWorld;
    class Renderer;
    class Entity
    {

    protected:

        IWorld& mWorld;
        
        vec2d mAcc = vec2d(0.f);
        vec2d mVel = vec2d(0.f);
        vec2d mPos = vec2d(0.f);

        float mRadius = 0.3f;

        bool mAlive = true;

    public:

        Entity(IWorld& pWorld) : mWorld(pWorld) {}
        virtual ~Entity() = default;

        virtual void Update(float dtMs) = 0;
        virtual void Draw(Renderer& render) = 0;
        
        inline bool IsAlive() const { return mAlive; }
        inline void Destroy() { mAlive = false; }

        virtual void OnCollision(Entity& other) {}

        /// @brief attempts to move to the new position given
        /// @param newPos new position to attempt to move to
        /// @details this function will check if the new position is a wall, and if it is, it will attempt to move the entity along the wall instead of through it. If the new position is not a wall, the entity will move to the new position.
        /// @warning this function will not check for collisions with other entities, only walls in the map. If you want to check for collisions with other entities, you will need to do that in the derived class.
        void AttemptMove(vec2d newPos);

        // transform
        inline void AddPos(const vec2d& pos) { mPos += pos; }
        inline void SetPos(const vec2d& pos) { mPos = pos; }
        inline const vec2d& GetPos() const { return mPos; }

        inline void AddVel(const vec2d& vel) { mVel += vel; }
        inline void SetVel(const vec2d& vel) { mVel = vel; }
        inline const vec2d& GetVel() const { return mVel; }

        inline void AddAcc(const vec2d& acc) { mAcc += acc; }
        inline void SetAcc(const vec2d& acc) { mAcc = acc; }
        inline const vec2d& GetAcc() const { return mAcc; }

        inline float GetRadius() const { return mRadius; }
        inline void SetRadius(const float& newRadius) { mRadius = newRadius; }
    };

};