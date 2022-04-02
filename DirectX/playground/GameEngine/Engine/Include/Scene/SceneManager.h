#pragma once

#include "Scene.h"

/// <summary>
/// ���� �����ϴ� Ŭ����
/// </summary>
class CSceneManager
{
private:
	// ���� ��
	CScene* m_Scene;

	// ���� ��
	CScene* m_NextScene;

	// �ε�� �� ��� ���� �ݹ�
	std::function<void(CScene*, size_t)> m_CreateSceneModeCallback;

	// �ε�� ������Ʈ ���� ���� �ݹ�
	std::function<CGameObject* (CScene*, size_t)> m_CreateObjectCallback;

	// �ε�� ������Ʈ�� ������Ʈ �߰� �ݹ�
	std::function<class CComponent* (CGameObject*, size_t)> m_CreateComponentCallback;

	// �ε�� �ִϸ��̼� �ν��Ͻ� ���� �ݹ�
	std::function<void(class CSpriteComponent*, size_t)> m_CreateAnimInstanceCallback;

	// �� �Ŵ����� ����ȭ�� ���ؼ� �Ӱ迵�� ������ ������.
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

	// ������Ʈ �����ÿ��� ������ ��ȯ���༭
	// ��ȯ���� ������Ʈ�� �ε��� �� �ְ����ش�.
	CGameObject* CallCreateObject(CScene* scene, size_t type)
	{
		if (m_CreateObjectCallback)
			return m_CreateObjectCallback(scene, type);

		return nullptr;
	}

	// ������Ʈ �����ÿ��� ��ȯ�ؼ� �ε� �Լ��� ȣ�� �� �� �ְ��Ѵ�.
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

	// �ܺ�(Ŭ���̾�Ʈ, ������)���� �� ��带 �����ÿ� �� �Լ��� ���ؼ� �ݹ��Լ� ���ε带 ���ش�.
	template <typename T>
	void SetCreateSceneModeFunction(T* obj, void (T::* func)(CScene*, size_t))
	{
		m_CreateSceneModeCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	// �ܺ�(Ŭ���̾�Ʈ, ������)���� ������Ʈ�� �����ÿ� �� �Լ��� ���ؼ� �ݹ��Լ� ���ε带 ���ش�.
	template <typename T>
	void SetCreateObjectFunction(T* obj, CGameObject* (T::* func)(CScene*, size_t))
	{
		m_CreateObjectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	// �ܺ�(Ŭ���̾�Ʈ, ������)���� ������Ʈ�� �����ÿ� �� �Լ��� ���ؼ� �ݹ��Լ� ���ε带 ���ش�.
	template <typename T>
	void SetCreateComponentFunction(T* obj, class CComponent* (T::* func)(CGameObject*, size_t))
	{
		m_CreateComponentCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	// �ܺ�(Ŭ���̾�Ʈ, ������)���� �ִϸ��̼� �ν��Ͻ��� �����ÿ� �� �Լ��� ���ؼ� �ݹ��Լ� ���ε带 ���ش�.
	template <typename T>
	void SetCreateAnimInstanceFunction(T* obj, void (T::* func)(CSpriteComponent*, size_t))
	{
		m_CreateAnimInstanceCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	DECLARE_SINGLE(CSceneManager)
};

