#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CCheerleaderAnimation :
	public CAnimationSequence2DInstance
{
public:
	CCheerleaderAnimation();
	CCheerleaderAnimation(const CCheerleaderAnimation& anim);
	~CCheerleaderAnimation();

public:
	virtual bool Init();
	virtual CCheerleaderAnimation* Clone();
};

