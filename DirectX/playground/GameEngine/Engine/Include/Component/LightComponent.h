#pragma once

#include "SceneComponent.h"
#include "../Resource/Shader/LightConstantBuffer.h"

class CLightComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CLightComponent();
	CLightComponent(const CLightComponent& com);
	virtual ~CLightComponent();

protected:
	CLightConstantBuffer* m_CBuffer;

public:
	inline Light_Type GetLightType() const
	{
		return m_CBuffer->GetLightType();
	}

	inline float GetLightDistance() const
	{
		return m_CBuffer->GetLightDistance();
	}

	inline void SetColor(const Vector4& color)
	{
		m_CBuffer->SetColor(color);
	}

	inline void SetLightType(Light_Type lightType)
	{
		m_CBuffer->SetLightType(lightType);
	}

	inline void SetPos(const Vector3& pos)
	{
		m_CBuffer->SetPos(pos);
	}

	inline void SetDir(const Vector3& dir)
	{
		m_CBuffer->SetDir(dir);
	}

	inline void SetDistance(float distance)
	{
		m_CBuffer->SetDistance(distance);
	}

	inline void SetInnerAngle(float angle)
	{
		m_CBuffer->SetInnerAngle(angle);
	}

	inline void SetOutterAngle(float angle)
	{
		m_CBuffer->SetOutterAngle(angle);
	}

	inline void SetAttConst1(float c)
	{
		m_CBuffer->SetAttConst1(c);
	}

	inline void SetAttConst2(float c)
	{
		m_CBuffer->SetAttConst2(c);
	}

	inline void SetAttConst3(float c)
	{
		m_CBuffer->SetAttConst3(c);
	}


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CLightComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);


public:
	void SetShader();
};

