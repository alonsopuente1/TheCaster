#include "ammopickup.hpp"

#include "castengine/renderer.hpp"

#include "player.hpp"

AmmoPickup::AmmoPickup(CastEngine::IWorld &world, CastEngine::Texture *texture) : CastEngine::SpriteEntity(world)
{
    SetTexture(texture);
}

void AmmoPickup::Update(float dtMs)
{
}

void AmmoPickup::Draw(CastEngine::Renderer &render)
{
    render.RenderSprite(GetTexture(), mPos);
}

void AmmoPickup::OnCollision(Entity &other)
{
    Player* targetPlayer = dynamic_cast<Player*>(&other);

    if(!targetPlayer)
        return;

    Gun& playerGun = targetPlayer->GetGun();

    playerGun.AddAmmo(mAmmo);

    mAlive = false;
}
