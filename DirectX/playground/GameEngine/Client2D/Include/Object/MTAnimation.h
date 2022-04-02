#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CMTAnimation :
	public CAnimationSequence2DInstance
{
public:
	CMTAnimation();
	CMTAnimation(const CMTAnimation& anim);
	~CMTAnimation();

public:
	virtual bool Init();
	virtual CMTAnimation* Clone();
};

