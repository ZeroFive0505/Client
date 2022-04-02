#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"
#include "Component/SceneComponent.h"

class CLiquid :
	public CGameObject
{
	friend class CScene;

protected:
	CLiquid();
	CLiquid(const CLiquid& obj);
	virtual ~CLiquid();

private:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CColliderBox2D> m_Collider;
	std::vector<CSharedPtr<class CMover>> m_vecMovers;

	float m_DragConstant;

public:
	void ApplyDrag(float deltaTime);
	void AddMover(const sCollisionResult& result);

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CLiquid* Clone();
};

