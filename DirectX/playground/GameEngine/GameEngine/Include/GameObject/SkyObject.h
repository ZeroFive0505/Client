#pragma once

#include "GameObject.h"
#include "../Component/StaticMeshComponent.h"

class CSkyObject :
	public CGameObject
{
	friend class CScene;

protected:
	CSkyObject();
	CSkyObject(const CSkyObject& obj);
	virtual ~CSkyObject();

private:
	CSharedPtr<CStaticMeshComponent> m_Mesh;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSkyObject* Clone();
};

