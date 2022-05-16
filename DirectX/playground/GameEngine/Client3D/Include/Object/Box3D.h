#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox3D.h"

class CBox3D :
	public CGameObject
{
	friend class CScene;

protected:
	CBox3D();
	CBox3D(const CBox3D& obj);
	virtual ~CBox3D();

private:
	CSharedPtr<CColliderBox3D> m_Collider;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBox3D* Clone();
};

