#pragma once

#include "../GameInfo.h"
#include "../Component/CameraComponent.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	class CScene* m_Scene;

private:
	CSharedPtr<CCameraComponent> m_CurrentCamera;
	CSharedPtr<CCameraComponent> m_PrevCamera;
	// 직교 투영으로 UI를 그려낼때 이용
	CSharedPtr<CCameraComponent> m_UICamera;

public:
	inline CCameraComponent* GetCurrentCamera() const
	{
		return m_CurrentCamera;
	}

	inline CCameraComponent* GetUICamera() const
	{
		return m_UICamera;
	}

	inline void SetCurrentCamera(CCameraComponent* camera)
	{
		m_CurrentCamera = camera;
	}

	inline void SetPrevCamera()
	{
		m_PrevCamera = m_CurrentCamera;
	}

	inline void ReturnToPrevCamera()
	{
		m_CurrentCamera = m_PrevCamera;

		m_PrevCamera = nullptr;
	}

	void RayCast();

public:
	void Start();
	void Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
};

