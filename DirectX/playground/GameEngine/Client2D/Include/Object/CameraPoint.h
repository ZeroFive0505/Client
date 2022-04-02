#pragma once

#include "GameObject/GameObject.h"
#include "Component/CameraComponent.h"
#include "Component/SceneComponent.h"

class CCameraPoint :
	public CGameObject
{
	friend class CScene;

protected:
	CCameraPoint();
	CCameraPoint(const CCameraPoint& obj);
	virtual ~CCameraPoint();

private:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CCameraComponent> m_Camera;
	CGameObject* m_Target;
	bool m_Follow;
	bool m_Lerp;
	float m_LerpTime;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CCameraPoint* Clone();

public:
	inline void SetTarget(CGameObject* target)
	{
		m_Target = target;
	}

	inline void StopFollow()
	{
		m_Follow = false;
		m_Lerp = false;
	}

	inline void StartFollow()
	{
		m_Follow = true;
		m_Lerp = true;
		m_LerpTime = 0.0f;
	}
};

