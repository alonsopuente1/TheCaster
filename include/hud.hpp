#pragma once

#include "castengine/hudelement.hpp"

#include <SDL2/SDL_ttf.h>

namespace CastEngine
{
    class Renderer;
    class EntityManager;
    class Map;
}

class Player;
class HUD
{

private:

    CastEngine::HUDElement mAmmo;
    CastEngine::HUDElement mHealth;
    CastEngine::HUDElement mScore;

    CastEngine::Texture* mMinimapTex;

    CastEngine::Renderer& mParentRender;
        
    void UpdatePlayerInfo(Player& player);
    void UpdateMinimap(const CastEngine::EntityManager& entManager, const CastEngine::Map& map);
    
    void DrawMinimap();

public:
    
    HUD(CastEngine::Renderer& rend) : mAmmo(rend), mHealth(rend), mScore(rend), mParentRender(rend) {}
    ~HUD() { mAmmo.Destroy(); mHealth.Destroy(); mScore.Destroy(); }
    
    void Init(TTF_Font* font);
    void Update(CastEngine::EntityManager& entManager, const CastEngine::Map& map);
    void Draw();

};

