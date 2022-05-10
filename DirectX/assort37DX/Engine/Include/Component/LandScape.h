#pragma once
#include "SceneComponent.h"
class CLandScape :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CLandScape();
	CLandScape(const CLandScape& com);
	virtual ~CLandScape();

protected:

public:

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CLandScape* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

