#pragma once

#include "GameObject/GameObject.h"
#include "Component/StaticMeshComponent.h"

class CLine :
	public CGameObject
{
	friend class CScene;

protected:
	CLine();
	CLine(const CLine& obj);
	virtual ~CLine();

private:
	CSharedPtr<CStaticMeshComponent> m_Mesh;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CLine* Clone();
};

