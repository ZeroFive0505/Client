#pragma once

#include "GameObject/GameObject.h"
#include "Component/CameraComponent.h"
#include "Component/SceneComponent.h"

class CObserver :
	public CGameObject
{
	friend class CScene;

protected:
	CObserver();
	CObserver(const CObserver& obj);
	virtual ~CObserver();

private:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CCameraComponent> m_Camera;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CObserver* Clone();

private:
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void ResetPos(float deltaTime);
};

