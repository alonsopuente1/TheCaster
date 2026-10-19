#pragma once

#include "castengine/spriteentity.hpp"

class AmmoPickup : public CastEngine::SpriteEntity
{

private:

    int mAmmo = 0;

public:

    AmmoPickup(CastEngine::IWorld& world, CastEngine::Texture* texture);

    void Update(float dtMs) override;
    void Draw(CastEngine::Renderer& render) override;

    void OnCollision(Entity& other) override;

    inline void SetAmmo(int newAmmo) { mAmmo = newAmmo; }
};