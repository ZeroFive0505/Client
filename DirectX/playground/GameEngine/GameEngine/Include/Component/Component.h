#pragma once

#include "../Ref.h"

/// <summary>
/// ���ӿ�����Ʈ�� �ο��Ǵ�
/// ������Ʈ ���̽� Ŭ����
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
	// ��� ���� �ҼӵǾ��ִ���
	class CScene* m_Scene;
	// ��� ���ӿ�����Ʈ�� �ҼӵǾ��ִ���
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

