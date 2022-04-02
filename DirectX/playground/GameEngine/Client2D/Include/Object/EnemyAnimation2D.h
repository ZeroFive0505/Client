#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CEnemyAnimation2D :
	public CAnimationSequence2DInstance
{
public:
	CEnemyAnimation2D();
	CEnemyAnimation2D(const CEnemyAnimation2D& anim);
	~CEnemyAnimation2D();

public:
	virtual bool Init();
	virtual CEnemyAnimation2D* Clone();
};

