#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CSchoolBoyAnimation2D :
	public CAnimationSequence2DInstance
{
public:
	CSchoolBoyAnimation2D();
	CSchoolBoyAnimation2D(const CSchoolBoyAnimation2D& anim);
	~CSchoolBoyAnimation2D();

public:
	virtual bool Init();
	virtual CSchoolBoyAnimation2D* Clone();
};

