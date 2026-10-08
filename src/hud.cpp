#include "hud.hpp"

#include "castengine/renderer.hpp"
#include "castengine/window.hpp"
#include "castengine/entitymanager.hpp"
#include "castengine/logger.hpp"
#include "castengine/map.hpp"

#include "gun.hpp"
#include "player.hpp"
#include "enemy.hpp"

void HUD::Init(TTF_Font *font)
{
    mAmmo.Init(font);
    mHealth.Init(font);
    mScore.Init(font);

    CastEngine::Texture minimapTex(mParentRender.GetWindow());

    minimapTex.CreateBlankTexture("minimap", 200, 200);

    mMinimapTex = mParentRender.texBank.PushTexture(std::move(minimapTex));
    if(!mMinimapTex)
        LogMsg(ERROR, "failed to create minimap texture");

}

void HUD::UpdatePlayerInfo(Player& player)
{
    std::string newAmmoText;
    std::string newHealthText;
    std::string newScoreText;

    const Gun& playerGun = player.GetGun();

    newAmmoText = playerGun.GetName() + " " + std::to_string(playerGun.GetAmmo()) + "/" + std::to_string(playerGun.GetReserves());
    newHealthText = "Health: 100";
    newScoreText = "Score: 0";

    mAmmo.Update(newAmmoText);
    mHealth.Update(newHealthText);
    mScore.Update(newScoreText);
}

void HUD::UpdateMinimap(const CastEngine::EntityManager& entManager, const CastEngine::Map& map)
{
    if(!mMinimapTex)
        return;

    const auto& entityList = entManager.GetEntities();

    int numMapCells = map.GetHeight() * map.GetWidth();

    int rectWidth = mMinimapTex->GetWidth() / map.GetWidth();
    int rectHeight = mMinimapTex->GetHeight() / map.GetHeight();

    SDL_SetRenderTarget(mParentRender.GetWindow().GetRenderer(), mMinimapTex->GetTexture());

    SDL_Color black = {0, 0, 0, 255};

    mParentRender.ClearScreen(black);

    SDL_SetRenderDrawColor(mParentRender.GetWindow().GetRenderer(), 0, 0, 0xff, 0xff);

    for(int i = 0; i < numMapCells; i++)
    {
        if(map[i] == 0)
            continue;

        int x = i % map.GetWidth();
        int y = i / map.GetWidth();

        SDL_Rect cellScreenRect = {
            x * rectWidth,
            y * rectHeight,
            rectWidth,
            rectHeight
        };

        if(SDL_RenderFillRect(mParentRender.GetWindow().GetRenderer(), &cellScreenRect) < 0)
        {
            LogMsgf(ERROR, "failed to render map rect onto minimap texture. SDL_ERROR: %s", SDL_GetError());
        }
    }

    for(const auto& ent : entityList)
    {
        if(ent->IsAlive() == false)
            continue;

        if(dynamic_cast<Player*>(ent.get()))
            mParentRender.RenderFillCircle({static_cast<int>(ent->GetPos().x * rectWidth), static_cast<int>(ent->GetPos().y * rectHeight)}, ent->GetRadius() * rectWidth, {0, 255, 0, 255});
        else
            mParentRender.RenderFillCircle({static_cast<int>(ent->GetPos().x * rectWidth), static_cast<int>(ent->GetPos().y * rectHeight)}, ent->GetRadius() * rectWidth, {255, 0, 0, 255});
    }

    SDL_SetRenderTarget(mParentRender.GetWindow().GetRenderer(), NULL);
}

void HUD::Update(CastEngine::EntityManager& entManager, const CastEngine::Map& map)
{
    UpdateMinimap(entManager, map);
    Player* player = nullptr;

    for(const auto& ent : entManager.GetEntities())
    {
        player = dynamic_cast<Player*>(ent.get());
        if(player)
            UpdatePlayerInfo(*player);
    }
        
}

void HUD::DrawMinimap()
{
    if(!mMinimapTex)
    {
        LogMsg(WARN, "failed to fetch the minimap tex");
        return;
    }

    SDL_Rect minimapRect = {
        0, 0, 
        static_cast<int>(mMinimapTex->GetWidth()), 
        static_cast<int>(mMinimapTex->GetHeight())};

    SDL_Rect minimapDst = {
        mParentRender.GetWindow().GetWidth() / 20, 
        mParentRender.GetWindow().GetHeight() / 20, 
        mParentRender.GetWindow().GetWidth() / 10, 
        mParentRender.GetWindow().GetHeight() / 10};

    mParentRender.RenderTexture(*mMinimapTex, minimapRect, minimapDst);
}

void HUD::Draw()
{
    int windowW = mParentRender.GetWindow().GetWidth();
    int windowH = mParentRender.GetWindow().GetHeight();

    int textHeight = windowH / 10;
    int textWidth = windowW / 10;
        
    mAmmo.Draw({
        windowW - textWidth,
        windowH - textHeight,
        textWidth,
        textHeight
    });

    mScore.Draw({
        windowW - textWidth,
        windowH - textHeight * 2,
        textWidth,
        textHeight
    });

    mHealth.Draw({
        windowW - textWidth,
        windowH - textHeight * 3,
        textWidth,
        textHeight
    });

    DrawMinimap();
}
