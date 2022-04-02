#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CSchoolGirlAnimation :
	public CAnimationSequence2DInstance
{
public:
	CSchoolGirlAnimation();
	CSchoolGirlAnimation(const CSchoolGirlAnimation& anim);
	~CSchoolGirlAnimation();

public:
	virtual bool Init();
	virtual CSchoolGirlAnimation* Clone();
};

