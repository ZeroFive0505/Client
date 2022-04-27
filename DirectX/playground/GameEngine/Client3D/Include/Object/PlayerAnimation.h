#pragma once

#include "Animation/AnimationSequenceInstance.h"

class CPlayerAnimation : 
	public CAnimationSequenceInstance
{
public:
	CPlayerAnimation();
	CPlayerAnimation(const CPlayerAnimation& anim);
	~CPlayerAnimation();

public:
	virtual bool Init();
	virtual CPlayerAnimation* Clone();
};

