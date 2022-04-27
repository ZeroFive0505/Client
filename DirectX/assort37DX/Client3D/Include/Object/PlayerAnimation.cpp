
#include "PlayerAnimation.h"

CPlayerAnimation::CPlayerAnimation()
{
	SetTypeID<CPlayerAnimation>();
}

CPlayerAnimation::CPlayerAnimation(const CPlayerAnimation& Anim) :
	CAnimationSequenceInstance(Anim)
{
}

CPlayerAnimation::~CPlayerAnimation()
{
}

bool CPlayerAnimation::Init()
{
	if (!CAnimationSequenceInstance::Init())
		return false;

	AddAnimation("PlayerIdle", "Idle");

	return true;
}

CPlayerAnimation* CPlayerAnimation::Clone()
{
	return new CPlayerAnimation(*this);
}
