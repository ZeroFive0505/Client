
#include "SceneManager.h"
#include "../Input.h"
#include "Scene.h"

CSceneManager* CSceneManager::m_Inst = nullptr;

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
}

// 가지고있는 씬을 삭제
CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
}

// 초기화시 일단 씬을 만든다.
bool CSceneManager::Init()
{
	m_Scene = new CScene;

	m_Scene->Init();

	return true;
}

// 가지고있는 씬을 업데이트
bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);

	return ChangeScene();;
}

// 가지고 있는 씬을 PostUpdate
bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();;
}

bool CSceneManager::Collision(float DeltaTime)
{
	m_Scene->Collision(DeltaTime);

	return ChangeScene();;
}

bool CSceneManager::Render(HDC hDC)
{
	m_Scene->Render(hDC);

	return ChangeScene();
}

// 항상 다음 씬이 있는지를 확인한다. 만약 있을 경우 씬을 교체하고 참을 반환한다.
bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		// 기존 장면을 제거한다.
		SAFE_DELETE(m_Scene);

		// 입력 매니저에 연결된 콜백함수들을 모두 초기화시킨다.
		CInput::GetInst()->ClearCallback();

		// 생성되어 있는 다음 장면을 기존 장면으로 지정한다.
		m_Scene = m_NextScene;

		// 변수를 초기화한다.
		m_NextScene = nullptr;

		return true;
	}

	return false;
}
