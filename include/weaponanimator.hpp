#pragma once

#include <unordered_map>

#include "castengine/animatedtexture.hpp"
#include "castengine/logger.hpp"

enum class WeaponAnimationType : int
{
    Idle = 0,
    Walk,
    Shoot,
    Reload
};

static std::string gAnimTypeStrings[] = {
    "Idle",
    "Walk",
    "Shoot",
    "Reload"
};

class WeaponAnimator
{

public:

    enum class MissingAnimPolicy : int
    {
        UseIdle = 0,
        Error
    };

private:

    std::unordered_map<
        WeaponAnimationType, 
        CastEngine::AnimatedTexture> mAnim;

    WeaponAnimationType mCurrentAnim = WeaponAnimationType::Idle;

    bool ResolveMissingAnim(WeaponAnimationType type, MissingAnimPolicy policy);

public:

    CastEngine::AnimatedTexture* Find(WeaponAnimationType type);

    const CastEngine::AnimatedTexture* Find(WeaponAnimationType type) const;

    void Add(WeaponAnimationType type, CastEngine::AnimatedTexture&& anim);

    bool Play(WeaponAnimationType type, MissingAnimPolicy policy = MissingAnimPolicy::Error);

    CastEngine::AnimatedTexture* GetCurrentAnimation();

};