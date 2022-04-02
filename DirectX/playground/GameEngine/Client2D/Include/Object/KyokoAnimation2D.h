#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CKyokoAnimation2D :
	public CAnimationSequence2DInstance
{
public:
	CKyokoAnimation2D();
	CKyokoAnimation2D(const CKyokoAnimation2D& anim);
	~CKyokoAnimation2D();

public:
	virtual bool Init();
	virtual CKyokoAnimation2D* Clone();
};

