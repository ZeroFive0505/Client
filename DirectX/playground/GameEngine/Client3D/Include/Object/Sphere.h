#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderSphere.h"

class CSphere :
	public CGameObject
{
	friend class CScene;

protected:
	CSphere();
	CSphere(const CSphere& obj);
	virtual ~CSphere();

private:
	CSharedPtr<CColliderSphere> m_Collider;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSphere* Clone();
};

