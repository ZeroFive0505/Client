#pragma once

#include "Scene.h"

/// <summary>
/// 씬을 관리하는 클래스
/// </summary>
class CSceneManager
{
private:
	// 현재 씬
	CScene* m_Scene;

	// 다음 씬
	CScene* m_NextScene;

	// 로드시 씬 모드 생성 콜백
	std::function<void(CScene*, size_t)> m_CreateSceneModeCallback;

	// 로드시 오브젝트 생성 위한 콜백
	std::function<CGameObject* (CScene*, size_t)> m_CreateObjectCallback;

	// 로드시 오브젝트에 컴포넌트 추가 콜백
	std::function<class CComponent* (CGameObject*, size_t)> m_CreateComponentCallback;

	// 로드시 애니메이션 인스턴스 생성 콜백
	std::function<void(class CSpriteComponent*, size_t)> m_CreateAnimInstanceCallback;

	// 씬 매니저는 동기화를 위해서 임계영역 변수를 가진다.
	CRITICAL_SECTION m_Crt;

public:
	CScene* GetScene() const
	{
		return m_Scene;
	}

	void CallCreateSceneMode(CScene* scene, size_t type)
	{
		if (m_CreateSceneModeCallback)
			m_CreateSceneModeCallback(scene, type);
	}

	// 오브젝트 생성시에는 생성후 반환해줘서
	// 반환받은 오브젝트로 로드할 수 있게해준다.
	CGameObject* CallCreateObject(CScene* scene, size_t type)
	{
		if (m_CreateObjectCallback)
			return m_CreateObjectCallback(scene, type);

		return nullptr;
	}

	// 컴포넌트 생성시에도 반환해서 로드 함수를 호출 할 수 있게한다.
	class CComponent* CallCreateComponent(CGameObject* obj, size_t type)
	{
		if (m_CreateComponentCallback)
			return m_CreateComponentCallback(obj, type);

		return nullptr;
	}

	void CallCreateAnimInstance(class CSpriteComponent* sprite, size_t type)
	{
		if (m_CreateAnimInstanceCallback)
			m_CreateAnimInstanceCallback(sprite, type);
	}

public:
	void Start();
	bool Init();
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);

public:
	bool ChangeScene();
	void CreateNextScene(bool autoChange = true);
	void ChangeNextScene();

	template <typename T>
	bool CreateSceneMode(bool current = true)
	{
		if (current)
			return m_Scene->CreateSceneMode<T>();

		return m_NextScene->CreateSceneMode<T>();
	}

	template <typename T>
	T* CreateSceneModeEmpty(bool current = true)
	{
		if (current)
			return m_Scene->CreateSceneModeEmpty<T>();

		return m_NextScene->CreateSceneModeEmpty<T>();
	}

	// 외부(클라이언트, 에디터)에서 씬 모드를 생성시에 이 함수를 통해서 콜백함수 바인드를 해준다.
	template <typename T>
	void SetCreateSceneModeFunction(T* obj, void (T::* func)(CScene*, size_t))
	{
		m_CreateSceneModeCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	// 외부(클라이언트, 에디터)에서 오브젝트를 생성시에 이 함수를 통해서 콜백함수 바인드를 해준다.
	template <typename T>
	void SetCreateObjectFunction(T* obj, CGameObject* (T::* func)(CScene*, size_t))
	{
		m_CreateObjectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	// 외부(클라이언트, 에디터)에서 컴포넌트를 생성시에 이 함수를 통해서 콜백함수 바인드를 해준다.
	template <typename T>
	void SetCreateComponentFunction(T* obj, class CComponent* (T::* func)(CGameObject*, size_t))
	{
		m_CreateComponentCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	// 외부(클라이언트, 에디터)에서 애니메이션 인스턴스를 생성시에 이 함수를 통해서 콜백함수 바인드를 해준다.
	template <typename T>
	void SetCreateAnimInstanceFunction(T* obj, void (T::* func)(CSpriteComponent*, size_t))
	{
		m_CreateAnimInstanceCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	DECLARE_SINGLE(CSceneManager)
};

