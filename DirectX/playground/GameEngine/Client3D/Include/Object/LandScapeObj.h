#pragma once

#include "GameObject/GameObject.h"
#include "Component/LandScape.h"

class CLandScapeObj :
	public CGameObject
{
	friend class CScene;

protected:
	CLandScapeObj();
	CLandScapeObj(const CLandScapeObj& obj);
	virtual ~CLandScapeObj();

private:
	CSharedPtr<CLandScape> m_LandScape;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CLandScapeObj* Clone();
};

