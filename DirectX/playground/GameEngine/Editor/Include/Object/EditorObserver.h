#pragma once

#include "GameObject/GameObject.h"
#include "Component/CameraComponent.h"
#include "Component/SceneComponent.h"

class CEditorObserver :
	public CGameObject
{
	friend class CScene;

protected:
	CEditorObserver();
	CEditorObserver(const CEditorObserver& obj);
	virtual ~CEditorObserver();

private:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CCameraComponent> m_Camera;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CEditorObserver* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void ResetPos(float deltaTime);
};

