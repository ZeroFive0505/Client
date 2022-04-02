#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CHitEffect :
	public CGameObject
{
	friend class CScene;

protected:
	CHitEffect();
	CHitEffect(const CHitEffect& obj);
	virtual ~CHitEffect();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	Vector4 m_TintColor;
	float m_Opacity;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CHitEffect* Clone();

public:
	inline void SetTintColor(const Vector4& color)
	{
		m_TintColor = color;
		m_Sprite->SetBaseColor(m_TintColor);
	}

	inline void SetOpacity(float opacity)
	{
		m_Opacity = opacity;
		m_Sprite->SetOpacity(opacity);
	}

public:
	inline Vector4 GetTintColor() const
	{
		return m_TintColor;
	}

	inline float GetOpacity() const
	{
		return m_Opacity;
	}
};

