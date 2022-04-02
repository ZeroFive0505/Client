#include "SceneManager.h"
#include "Scene.h"
#include "../Input.h"

CSceneManager* CSceneManager::m_Inst = nullptr;

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	if (!m_Scene->Init())
		return false;

	return true;
}

bool CSceneManager::Update(float deltaTime)
{
	m_Scene->Update(deltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float deltaTime)
{
	m_Scene->PostUpdate(deltaTime);

	return ChangeScene();
}

bool CSceneManager::Collision(float deltaTime)
{
	m_Scene->Collision(deltaTime);

	return ChangeScene();
}

bool CSceneManager::Render(HDC hDC)
{
	m_Scene->Render(hDC);

	return ChangeScene();
}

bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		SAFE_DELETE(m_Scene);

		CInput::GetInst()->ClearCallback();

		m_Scene = m_NextScene;

		m_NextScene = nullptr;

		return true;
	}

	return false;
}
