#pragma once

#include "GameObject/GameObject.h"
#include "Component/StaticMeshComponent.h"

class CCircle :
	public CGameObject
{
	friend class CScene;

protected:
	CCircle();
	CCircle(const CCircle& obj);
	virtual ~CCircle();

private:
	CSharedPtr<CStaticMeshComponent> m_Mesh;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CCircle* Clone();
};

