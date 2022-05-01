#include "PlayerAnimation.h"

CPlayerAnimation::CPlayerAnimation()
{
	SetTypeID<CPlayerAnimation>();
}

CPlayerAnimation::CPlayerAnimation(const CPlayerAnimation& anim) :
	CAnimationSequenceInstance(anim)
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
	AddAnimation("PlayerAttack", "Attack", false);
	AddAnimation("PlayerWalk", "Walk");

	SetEndFunction<CPlayerAnimation>("Attack", this, &CPlayerAnimation::AttackEnd);

	return true;
}

CPlayerAnimation* CPlayerAnimation::Clone()
{
	return new CPlayerAnimation(*this);
}
