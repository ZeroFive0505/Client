#include "HitAnimation.h"

CHitAnimation::CHitAnimation()
{
    SetTypeID<CHitAnimation>();
}

CHitAnimation::CHitAnimation(const CHitAnimation& anim) :
    CAnimationSequence2DInstance(anim)
{
}

CHitAnimation::~CHitAnimation()
{
}

bool CHitAnimation::Init()
{
    if (!CAnimationSequence2DInstance::Init())
        return false;

    AddAnimation("HitVFX", "HitVFX", false, 0.5f);

    CAnimationSequence2DInstance::Play();

    return true;
}

CHitAnimation* CHitAnimation::Clone()
{
    return new CHitAnimation(*this);
}
