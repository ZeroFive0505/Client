#pragma once

#include "../Ref.h"

/// <summary>
/// 게임오브젝트에 부여되는
/// 컴포넌트 베이스 클래스
/// </summary>
class CComponent :
	public CRef
{
	friend class CGameObject;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent() = 0;

protected:
	// 어느 씬에 소속되어있는지
	class CScene* m_Scene;
	// 어느 게임오브젝트에 소속되어있는지
	class CGameObject* m_Object;

public:
	void SetScene(class CScene* scene);
	void SetGameObject(class CGameObject* obj);
	virtual void Destroy();

public:
	inline class CScene* GetScene() const
	{
		return m_Scene;
	}

	inline class CGameObject* GetGameObject() const
	{
		return m_Object;
	}

protected:
	Component_Type m_ComponentType;

public:
	inline Component_Type GetComponentType() const
	{
		return m_ComponentType;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime) = 0;
	virtual void PostUpdate(float deltaTime) = 0;
	virtual void PrevRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual CComponent* Clone() = 0;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

