#include "weaponanimator.hpp"

bool WeaponAnimator::ResolveMissingAnim(WeaponAnimationType type, MissingAnimPolicy policy)
{
    switch(policy)
    {   
    case MissingAnimPolicy::UseIdle:
        Loop(WeaponAnimationType::Idle, MissingAnimPolicy::Error);
        break;
    case MissingAnimPolicy::Error:
        LogMsgf(ERROR, "missing '%s' animation", gAnimTypeStrings[static_cast<int>(policy)].c_str());
        return false;
    default:
        LogMsg(ERROR, "unrecognised missing animation policy");
        return false;
    }

    return true;
}

CastEngine::AnimatedTexture* WeaponAnimator::Find(WeaponAnimationType type)
{
    auto it = mAnim.find(type);
    if(it != mAnim.end())
        return &it->second;
    return nullptr;
}

const CastEngine::AnimatedTexture* WeaponAnimator::Find(WeaponAnimationType type) const
{
    auto it = mAnim.find(type);
    if(it != mAnim.end())
        return &it->second;
    return nullptr;
}

void WeaponAnimator::Update(float dtMs)
{
    CastEngine::AnimatedTexture* animation = Find(mCurrentAnim);

    if(!animation)
        return;

    animation->Update(dtMs);
}

bool WeaponAnimator::IsPlaying() const
{
    const CastEngine::AnimatedTexture* animation = Find(mCurrentAnim);

    if(!animation)
        return false;

    return animation->IsPlaying();
}

bool WeaponAnimator::HasFinished() const
{
    const CastEngine::AnimatedTexture* animation = Find(mCurrentAnim);

    if(!animation)
        return false;

    return animation->HasFinished();
}

void WeaponAnimator::Add(WeaponAnimationType type, CastEngine::AnimatedTexture&& anim)
{
    if(!anim.IsInitialised())
        return;

    mAnim.insert_or_assign(type, std::move(anim));
}

bool WeaponAnimator::PlayOnce(WeaponAnimationType type, MissingAnimPolicy policy)
{
    CastEngine::AnimatedTexture* animation = Find(type);
        
    if(!animation)
        return ResolveMissingAnim(type, policy);

    mCurrentAnim = type;

    animation->PlayOnce();

    return true;
}

bool WeaponAnimator::Loop(WeaponAnimationType type, MissingAnimPolicy policy)
{
    CastEngine::AnimatedTexture* animation = Find(type);

    if(!animation)
        return ResolveMissingAnim(type, policy);
    
    mCurrentAnim = type;

    animation->Loop();

    return true;
}


CastEngine::AnimatedTexture* WeaponAnimator::GetCurrentAnimation()
{
    return Find(mCurrentAnim);
}

WeaponAnimationType WeaponAnimator::GetCurrentType() const
{
    return mCurrentAnim;
}