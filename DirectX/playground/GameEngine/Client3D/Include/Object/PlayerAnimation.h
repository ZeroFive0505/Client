#pragma once

#include "Animation/AnimationSequenceInstance.h"

class CPlayerAnimation : 
	public CAnimationSequenceInstance
{
public:
	CPlayerAnimation();
	CPlayerAnimation(const CPlayerAnimation& anim);
	~CPlayerAnimation();

protected:
	bool m_Idle;

public:
	inline bool GetIdleEnable() const
	{
		return m_Idle;
	}

	inline void SetIdleEnable(bool enable)
	{
		m_Idle = enable;
	}

public:
	virtual bool Init();
	virtual CPlayerAnimation* Clone();

public:
	inline void AttackEnd()
	{
		ChangeAnimation("Idle");
		m_Idle = true;
	}
};

