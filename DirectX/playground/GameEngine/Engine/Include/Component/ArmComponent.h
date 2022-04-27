#pragma once

#include "SceneComponent.h"

class CArmComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CArmComponent();
	CArmComponent(const CArmComponent& com);
	virtual ~CArmComponent();

protected:
	float m_TargetDistance;
	Vector3 m_Offset;

public:
	inline float GetTargetDistance() const
	{
		return m_TargetDistance;
	}

	inline Vector3 GetOffset() const
	{
		return m_Offset;
	}

public:
	inline void SetOffset(const Vector3& offset)
	{
		m_Offset = offset;
	}

	inline void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	inline void SetTargetDistance(float distance)
	{
		m_TargetDistance = distance;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CArmComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

