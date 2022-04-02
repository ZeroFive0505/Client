#include "EnemyAnimation2D.h"

CEnemyAnimation2D::CEnemyAnimation2D()
{
    SetTypeID<CEnemyAnimation2D>();
}

CEnemyAnimation2D::CEnemyAnimation2D(const CEnemyAnimation2D& anim) :
    CAnimationSequence2DInstance(anim)
{
}

CEnemyAnimation2D::~CEnemyAnimation2D()
{
}

bool CEnemyAnimation2D::Init()
{
    if (!CAnimationSequence2DInstance::Init())
        return false;

    AddAnimation("EnemyIdle", "Idle");

    CAnimationSequence2DInstance::Play();

    return true;
}

CEnemyAnimation2D* CEnemyAnimation2D::Clone()
{
    return new CEnemyAnimation2D(*this);
}
