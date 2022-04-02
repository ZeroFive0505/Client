#pragma once

#include "GameObject/GameObject.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"

class CAfterImage :
	public CGameObject
{
	friend class CScene;

protected:
	CAfterImage();
	CAfterImage(const CAfterImage& obj);
	virtual ~CAfterImage();

protected:
	CSharedPtr<CSceneComponent> m_Transform;
	std::vector<CSharedPtr<CSpriteComponent>> m_vecSprites;
	Vector4 m_TintColor;
	Vector3 m_Offset;

	float m_Opacity;
	int m_Size;
	int m_Index;
	bool m_OffsetOn;
	bool m_EffectOn;
	bool m_Flip;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CAfterImage* Clone();

public:
	inline Vector4 GetTintColor() const
	{
		return m_TintColor;
	}

	inline bool IsEffectOn() const
	{
		return m_EffectOn;
	}

public:
	inline void SetEffect(bool effectOn)
	{
		m_EffectOn = effectOn;
	}

	void SetAnimation(const std::string name);

	void SetFrame(int frame);

	void SetPos(const Vector3& pos);

	inline void SetOffset(const Vector3& offset)
	{
		m_Offset = offset;
	}

	inline void SetOffsetMode(bool offsetMode)
	{
		m_OffsetOn = offsetMode;
	}

	void Prepare();

	void SetTintColor(const Vector4& color);

	void FlipImage(bool flip);

	inline void IncreaseIndex()
	{
		m_Index++;
	}

	template <typename T>
	void SetAnimationInstance()
	{
		size_t size = m_vecSprites.size();

		for (size_t i = 0; i < size; i++)
		{
			m_vecSprites[i]->CreateAnimationInstance<T>();
			m_vecSprites[i]->GetAnimationInstance()->Stop();
		}
	}
};