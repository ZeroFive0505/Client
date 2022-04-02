#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "../GameObject/GameObject.h"
#include "Viewport.h"
#include "NavigationManager.h"

/// <summary>
/// �� Ŭ����
/// </summary>
class CScene
{
	// �� �Ŵ����� ���� �����Ӱ� ������ �����ؾ���
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CSharedPtr<CSceneMode> m_Mode;
	// ���� ���� �Ҽӵ� ���ҽ�
	CSceneResource* m_Resource;
	// ���� ���� �Ҽӵ� �浹ü
	CSceneCollision* m_Collision;
	CCameraManager* m_CameraManager;
	CViewport* m_Viewport;
	CNavigationManager* m_NavManager;

	std::list<CSharedPtr<CGameObject>> m_ObjList;
	std::vector<CColliderComponent*> m_vecTileColliders;
	bool m_Start;
	bool m_Change;

public:
	inline void SetAutoChange(bool change)
	{
		m_Change = change;
	}

public:
	inline CSceneResource* GetSceneResource() const
	{
		return m_Resource;
	}

	inline CSceneCollision* GetCollision() const
	{
		return m_Collision;
	}

	inline CCameraManager* GetCameraManager() const
	{
		return m_CameraManager;
	}

	inline CNavigationManager* GetNavigationManager() const
	{
		return m_NavManager;
	}

	inline CViewport* GetViewport() const
	{
		return m_Viewport;
	}

	inline CGameObject* GetPlayerObject() const
	{
		return m_Mode->GetPlayerObject();
	}

	inline const std::list<CSharedPtr<CGameObject>>& GetAllObjects() const
	{
		return m_ObjList;
	}

	CGameObject* FindObject(const std::string& name)
	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetName() == name)
				return *iter;
		}

		return nullptr;
	}

	inline void AddTileColldier(CColliderComponent* collider)
	{
		m_vecTileColliders.push_back(collider);
	}

	inline const std::vector<CColliderComponent*>& GetTileColliders() const
	{
		return m_vecTileColliders;
	}

public:
	void Start();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Save(const char* fileName, const std::string& pathName = SCENE_PATH);
	void SaveFullPath(const char* fullPath);
	void Load(const char* fileName, const std::string& pathName = SCENE_PATH);
	void LoadFullPath(const char* fullPath);

private:
	static inline bool YSort(const CGameObject* src, const CGameObject* dest)
	{
		if (src->GetCustomY() == FLT_MAX ||
			dest->GetCustomY() == FLT_MAX)
			return src->GetRootComponent()->GetWorldPos().y > dest->GetRootComponent()->GetWorldPos().y;
		else
			return src->GetCustomY() > dest->GetCustomY();
	}

public:
	template <typename T>
	bool CreateSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		if (!m_Mode->Init())
		{
			m_Mode = nullptr;
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateSceneModeEmpty()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return (T*)*m_Mode;
	}

	// ����带 �ε�ÿ� ����Ѵ�.
	// �ε��Ҷ����� �ҷ��� �����ͷ� �ʱ�ȭ�� �ϱ⿡
	// Init�� ȣ������ �ʴ´�.
	template <typename T>
	bool LoadSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return true;
	}

	template <typename T>
	T* CreateGameObject(const std::string& name)
	{
		T* obj = new T;

		obj->SetName(name);
		obj->SetScene(this);

		if (!obj->Init())
		{
			SAFE_RELEASE(obj);
			return nullptr;
		}

		m_ObjList.push_back(obj);

		if (m_Start)
			obj->Start();

		return obj;
	}

	// ���� ������Ʈ �ε��Լ��� ���������� Init�� ��ŵ�Ѵ�.
	template <typename T>
	T* LoadGameObject()
	{
		T* obj = new T;

		obj->SetScene(this);

		m_ObjList.push_back(obj);

		if (m_Start)
			obj->Start();

		return obj;
	}
};

