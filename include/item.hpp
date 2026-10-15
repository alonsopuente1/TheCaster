#pragma once

#include "castengine/entity.hpp"

class Item : public CastEngine::Entity
{

public:

    void Update(float dtMs) override;
    void Draw(CastEngine::Renderer& render) override;

    void OnCollision(Entity& other) override;
};