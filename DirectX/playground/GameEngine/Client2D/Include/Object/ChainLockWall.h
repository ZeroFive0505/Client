#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Engine.h"

class CChainLockWall :
	public CGameObject
{
	friend class CScene;

protected:
	CChainLockWall();
	CChainLockWall(const CChainLockWall& obj);
	virtual ~CChainLockWall();

private:
	CSharedPtr<CSpriteComponent> m_WallSprite;
	CSharedPtr<CSpriteComponent> m_LockSprite;
	CSharedPtr<CColliderBox2D> m_Collider;
	float m_LerpTime;

	bool m_FadeInStart;
	bool m_FadeOutStart;
	bool m_FadeOutComplete;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CChainLockWall* Clone();

public:
	void SetAsTop();
	void SetAsBottom();
	void SetAsLeft();
	void SetAsRight();

	inline void StartFadeIn()
	{
		m_LerpTime = 0.0f;
		m_FadeInStart = true;
	}

	inline void StartFadeOut()
	{
		m_LerpTime = 1.0f;
		m_FadeOutStart = true;
	}

	inline bool FadeOutComplete() const
	{
		return m_FadeOutComplete;
	}
};

