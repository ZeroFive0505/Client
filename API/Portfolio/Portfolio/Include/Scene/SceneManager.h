#pragma once

#include "../GameInfo.h"
#include "../Object/ObjectInfo.h"
#include "SelectScene.h"
#include "RaceScene.h"
#include "Scene.h"

class CSceneManager
{
private:
	CSceneManager();
	~CSceneManager();

private:
	CScene* m_Scene;
	CScene* m_NextScene;

public:
	inline CScene* GetScene()
	{
		return m_Scene;
	}

public:
	bool Init();
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);
	bool Collision(float deltaTime);
	bool Render(HDC hDC);

private:
	bool ChangeScene();

private:
	static CSceneManager* m_Inst;

public:
	inline static CSceneManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CSceneManager;

		return m_Inst;
	}

	inline static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}

public:
	template <typename T>
	bool CreateScene()
	{
		m_NextScene = new T;

		if (!m_NextScene->Init())
		{
			SAFE_DELETE(m_NextScene);
			return false;
		}

		return true;
	}

	template <typename T>
	bool CreateSelectScene(ECCOption option)
	{
		m_NextScene = new T;

		if (!m_NextScene->Init())
		{
			SAFE_DELETE(m_NextScene);
			return false;
		}

		((CSelectScene*)m_NextScene)->SetCurrentCC(option);
		((CSelectScene*)m_NextScene)->SetCCText(option);

		return true;
	}

	template <typename T>
	bool CreateRaceScene(ECCOption cc, ECupSelect cup, const std::string& course, EMenuPlayer player)
	{
		m_NextScene = new T;

		if (!m_NextScene->Init())
		{
			SAFE_DELETE(m_NextScene);
			return false;
		}

		((CRaceScene*)m_NextScene)->CircuitInit(cc, cup, course, player);

		return true;
	}
};

