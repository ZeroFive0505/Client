#pragma once

#include "Scene/SceneMode.h"

class CMainSceneMode :
	public CSceneMode
{
public:
	CMainSceneMode();
	~CMainSceneMode();

private:
	std::function<void(bool, float)> m_LoadingFunction;

public:
	virtual bool Init();

private:
	void LoadMesh();
	void CreateMaterial();
	void CreateAnimationSequence();
	void CreateParticle();

public:
	template <typename T>
	void SetLoadingFunction(T* obj, void(T::* func)(bool, float))
	{
		m_LoadingFunction = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
};

