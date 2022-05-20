#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/Material/Material.h"

enum class Decal_Fade_State
{
	FadeIn,
	FadeOut,
	Duration
};

class CDecalComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CDecalComponent();
	CDecalComponent(const CDecalComponent& com);
	virtual ~CDecalComponent();

protected:
#ifdef _DEBUG
	CSharedPtr<CStaticMesh> m_DebugMesh;
	CSharedPtr<CMaterial> m_DebugMaterial;
#endif // _DEBUG

	CSharedPtr<CStaticMesh> m_Mesh;
	CSharedPtr<CMaterial> m_Material;
	Decal_Fade_State m_FadeState;

	float m_FadeInTime;
	float m_FadeInTimeAcc;
	float m_FadeOutTime;
	float m_FadeOutTimeAcc;
	float m_DurationTime;
	float m_DurationTimeAcc;

	bool m_FadeLoop;

public:
	inline void SetFadeInTime(float time)
	{
		m_FadeInTime = time;
	}

	inline void SetFadeOutTime(float time)
	{
		m_FadeOutTime = time;
	}

	inline void SetDuration(float time)
	{
		m_DurationTime = time;
	}

	inline void SetFadeLoop(bool loop)
	{
		m_FadeLoop = loop;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CDecalComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

