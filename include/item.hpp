#pragma once

#include "castengine/spriteentity.hpp"

class AmmoPickup : public CastEngine::SpriteEntity
{

private:

public:

    void Update(float dtMs) override;
    void Draw(CastEngine::Renderer& render) override;

    void OnCollision(Entity& other) override;
};