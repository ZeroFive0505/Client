#pragma once

#include "../GameInfo.h"
#include "../Component/LightComponent.h"

class CLightManager
{
	friend class CScene;

private:
	CLightManager();
	~CLightManager();

private:
	class CScene* m_Scene;
	CSharedPtr<class CGameObject> m_GlobalLight;
	CSharedPtr<CLightComponent> m_GlobalLightComponent;

private:
	std::list<CSharedPtr<CLightComponent>> m_LightList;

public:
	inline void AddLight(CLightComponent* light)
	{
		m_LightList.push_back(light);
	}

	void DeleteLight(CLightComponent* light);

	void DeleteLight(const std::string& name);

public:
	inline bool IsEmpty() const
	{
		return m_LightList.empty();
	}

public:
	void Start();
	void Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void SetShader();
	void Destroy();
	void Render();
};

