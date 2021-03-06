#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "../GameObject/GameObject.h"
#include "Viewport.h"
#include "NavigationManager.h"
#include "Navigarion3DManager.h"
#include "LightManager.h"

/// <summary>
/// 씬 클래스
/// </summary>
class CScene
{
	// 씬 매니저는 씬을 자유롭게 생성이 가능해야함
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CSharedPtr<CSceneMode> m_Mode;
	// 현재 씬에 소속된 리소스
	CSceneResource* m_Resource;
	// 현재 씬에 소속된 충돌체
	CSceneCollision* m_Collision;
	CCameraManager* m_CameraManager;
	CViewport* m_Viewport;
	CNavigationManager* m_NavManager;
	CNavigarion3DManager* m_Nav3DManager;
	CLightManager* m_LightManager;

	std::list<CSharedPtr<CGameObject>> m_ObjList;
	CSharedPtr<CGameObject> m_SkyObject;

	std::list<class CSceneComponent*> m_RenderComponentList;


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

	inline CGameObject* GetSkyObject() const
	{
		return m_SkyObject;
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

	inline CNavigarion3DManager* GetNavigation3DManager() const
	{
		return m_Nav3DManager;
	}

	inline CLightManager* GetLightManager() const
	{
		return m_LightManager;
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

	inline void TileCollidersRenderToggle()
	{
		size_t size = m_vecTileColliders.size();

		for (size_t i = 0; i < size; i++)
		{
			m_vecTileColliders[i]->RenderToggle();
		}
	}

public:
	void Start();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Save(const char* fileName, const std::string& pathName = SCENE_PATH);
	void SaveFullPath(const char* fullPath);
	void Load(const char* fileName, const std::string& pathName = SCENE_PATH);
	void LoadFullPath(const char* fullPath);

public:
	bool Picking(CGameObject*& result, Vector3& hitPoint);

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

	// 씬모드를 로드시에 사용한다.
	// 로드할때에는 불러온 데이터로 초기화를 하기에
	// Init을 호출하지 않는다.
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

	// 게임 오브젝트 로드함수도 마찬가지로 Init은 스킵한다.
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

private:
	static bool SortRenderList(class CSceneComponent* src, class CSceneComponent* dest);
};

