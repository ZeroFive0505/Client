#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CMisuzuAnimation :
	public CAnimationSequence2DInstance
{
public:
	CMisuzuAnimation();
	CMisuzuAnimation(const CMisuzuAnimation& anim);
	~CMisuzuAnimation();

public:
	virtual bool Init();
	virtual CMisuzuAnimation* Clone();
};

