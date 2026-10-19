#pragma once

#include <unordered_map>

#include "castengine/animatedtexture.hpp"
#include "weaponanimator.hpp"

struct GunDef
{


    std::unordered_map<WeaponAnimationType, std::string> animFilePath; // map of animation names to file paths


    int fireRateMs;
    float texScale;     // value to scale gun textures by
    int damage;

    int magSize;
    int reserves;
    
    std::string name;

};

class Gun
{
    
private:

    enum class GunState : int
    {
        Ready,
        Reloading
    };

    int mAmmo;       // amount of bullets currently in magazine
    int mReserves;   // amount of bullets left over
    int mMagSize;    // max size of magazine

    int mCooldownMs; // time since gun last been shot
    int mFireRateMs; // time before gun can shoot again

    int mDamage;
    float mTexScale;

    WeaponAnimator mAnim;
    
    std::string mName;

    CastEngine::Renderer& mRender;
    
    GunState mState = GunState::Ready;
    
    void FinishReload();
    
public:

    Gun(CastEngine::Renderer& rend) : mRender(rend) {}

    bool Load(GunDef& def);
    void Update(float dtMs);
    void Draw(CastEngine::Renderer& render);
        
    bool TryShoot();

    void BeginReload();

    inline bool IsReady() { return mCooldownMs >= mFireRateMs && mState == GunState::Ready; }

    inline int GetDamage() { return mDamage; }

    inline void AddAmmo(int numBullets) { mAmmo += numBullets; }
    inline int GetAmmo() const { return mAmmo; }
    inline int GetMagSize() const { return mMagSize; }
    inline int GetReserves() const { return mReserves; }
    inline const std::string& GetName() const { return mName; }
};
