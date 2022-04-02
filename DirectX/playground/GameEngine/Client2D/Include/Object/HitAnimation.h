#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CHitAnimation :
	public CAnimationSequence2DInstance
{
public:
	CHitAnimation();
	CHitAnimation(const CHitAnimation& anim);
	virtual ~CHitAnimation();

public:
	virtual bool Init();
	virtual CHitAnimation* Clone();
};

