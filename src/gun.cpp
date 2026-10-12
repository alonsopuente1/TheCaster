#include "gun.hpp"

#include "castengine/logger.hpp"
#include "castengine/renderer.hpp"
#include "castengine/window.hpp"
#include "castengine/texture.hpp"

bool Gun::Load(GunDef &def)
{
    for(auto [type, filePath] : def.animFilePath)
    {
        CastEngine::AnimatedTexture animation(mRender);
        if(!animation.LoadAnimation(gAnimTypeStrings[static_cast<int>(type)], filePath))
            continue;
        
        mAnim.Add(type, std::move(animation));
    }

    if(def.fireRateMs <= 0)
    {
        LogMsgf(ERROR, "invalid firerate value. expected a value greater than 0, got '%i'", def.fireRateMs);
        return false;
    }

    mFireRateMs = def.fireRateMs;

    CastEngine::AnimatedTexture* shootAnimation = mAnim.Find(WeaponAnimationType::Shoot);
    if(shootAnimation)
        shootAnimation->SetAnimationTime(def.fireRateMs);

    if(def.damage < 0)
    {
        LogMsgf(ERROR, "invalid damage value. expected a positive value, got '%i'", def.damage);
        return false;
    }

    mDamage = def.damage;
        
    if(def.magSize < 0)
    {
        LogMsgf(ERROR, "invalid magsize value. expected positive value, got: '%i'", def.reserves);
        return false;
    }

    mAmmo = def.magSize;
    mMagSize = def.magSize;
        
    if(def.texScale < 0)
    {
        LogMsgf(ERROR, "invalid texture scale value. expected positive value got: '%f'", def.texScale);
        return false;
    }
    mTexScale = def.texScale;
        
    if(def.reserves < 0)
    {
        LogMsgf(ERROR, "invalid reserves value. expected a positive value, got: '%i'", def.reserves);
        return false;
    }
    mReserves = def.reserves;

    mName = def.name;

    return true;
}

void Gun::Update(float dtMs)
{
    if(mCooldownMs < mFireRateMs)
        mCooldownMs += dtMs;

    mAnim.GetCurrentAnimation()->Update(dtMs);
}

void Gun::Draw(CastEngine::Renderer &render)
{
    CastEngine::AnimatedTexture* animTex = mAnim.GetCurrentAnimation();
    if(!animTex || !animTex->IsInitialised()) return;
        
    CastEngine::Texture* tex = animTex->GetTexture();

    CastEngine::Window& window = render.GetWindow();
    
    int windowW = window.GetWidth();
    int windowH = window.GetHeight();

    float ratio = static_cast<float>(windowW) / (2.f * tex->GetWidth());

    int dstW = static_cast<int>(tex->GetWidth() * ratio * mTexScale);
    int dstH = static_cast<int>(tex->GetHeight() * ratio * mTexScale);

    int dstX = static_cast<int>(windowW * 0.5f - dstW * 0.5f);
    int dstY = static_cast<int>(windowH - dstH);

    SDL_Rect src { 0, 0,
        static_cast<int>(tex->GetWidth()),
        static_cast<int>(tex->GetHeight()) };
    SDL_Rect dst { dstX, dstY, dstW, dstH };

    if(!render.RenderTexture(*tex, src, dst))
        LogMsg(ERROR, "failed to render gun");
}
 
bool Gun::TryShoot()
{
    if(mAmmo <= 0 || !IsReady())
        return false;
    mCooldownMs = 0;
    mAnim.Play(WeaponAnimationType::Shoot);
    mAmmo--;

    return true;
}
    
void Gun::Reload()
{
    if(mReserves <= 0)
        return;

    int bulletsNeeded = mMagSize - mAmmo;

    if(bulletsNeeded == 0)
        return;

    if(bulletsNeeded > mReserves)
        bulletsNeeded = mReserves;

    mReserves -= bulletsNeeded;
    mAmmo += bulletsNeeded;
}
