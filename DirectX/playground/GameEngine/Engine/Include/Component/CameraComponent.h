#pragma once

#include "SceneComponent.h"

/// <summary>
/// 카메라 컴포넌트
/// 씬에 배치되어서 타겟이 되는 오브젝트를 따라간다.
/// </summary>
class CCameraComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CCameraComponent();
	CCameraComponent(const CCameraComponent& com);
	virtual ~CCameraComponent();

protected:
	Camera_Type m_CameraType;
	Matrix m_matView;
	Matrix m_matProj;
	// FOV 값
	float m_ViewAngle;
	// Far
	float m_Distance;
	sResolution m_RS;
	bool m_Shake;
	float m_Trauma;
	float m_RecoverySpeed;
	float m_TraumaAmount;

public:
	inline sResolution GetResolution() const
	{
		return m_RS;
	}

	inline Camera_Type GetCameraType() const
	{
		return m_CameraType;
	}

	inline Matrix GetViewMatrix() const
	{
		return m_matView;
	}

	inline Matrix GetProjMatrix() const
	{
		return m_matProj;
	}

	inline Vector2 GetLeftBottom() const
	{
		Vector2 LB;

		// 이미 카메라의 현재 위치가 좌하단 위치
		LB.x = GetWorldPos().x;
		LB.y = GetWorldPos().y;

		return LB;
	}

public:
	inline void SetViewAngle(float angle)
	{
		m_ViewAngle = angle;

		// 3d 카메라일 경우 FOV값이 변경되면 새롭게 투영 행렬를 만든다.
		if (m_CameraType == Camera_Type::Camera3D)
			CreateProjectionMatrix();
	}

	inline void SetDistance(float distance)
	{
		m_Distance = distance;

		CreateProjectionMatrix();
	}

	inline void SetCameraType(Camera_Type type)
	{
		m_CameraType = type;

		CreateProjectionMatrix();
	}

public:
	// 2D 전용
	void OnViewPortCenter()
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float z = GetRelativePos().z;

			SetRelativePos(m_RS.width / -2.0f, m_RS.height / -2.0f, z);
		}
	}

	// 2D 전용
	void SetViewPortRatio(float x, float y)
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float z = GetRelativePos().z;

			SetRelativePos(m_RS.width * -x, m_RS.height * -y, z);
		}
	}

private:
	void CreateProjectionMatrix();

public:
	inline void ActiveCameraShake()
	{
		m_Trauma = m_TraumaAmount;
		m_Shake = true;
	}

	inline void SetTraumaAmount(float amount)
	{
		m_TraumaAmount = amount;
	}

	inline void SetRecoverySpeed(float speed)
	{
		m_RecoverySpeed = speed;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CCameraComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

