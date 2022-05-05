#pragma once

#include "GameObject.h"
#include "../Component/LightComponent.h"

class CLightObject :
	public CGameObject
{
	friend class CScene;


protected:
	CLightObject();
	CLightObject(const CLightObject& obj);
	virtual ~CLightObject();

private:
	CSharedPtr<CLightComponent> m_Light;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CLightObject* Clone();
};

