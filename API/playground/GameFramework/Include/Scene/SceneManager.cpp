
#include "SceneManager.h"
#include "../Input.h"
#include "Scene.h"

CSceneManager* CSceneManager::m_Inst = nullptr;

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
}

// �������ִ� ���� ����
CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
}

// �ʱ�ȭ�� �ϴ� ���� �����.
bool CSceneManager::Init()
{
	m_Scene = new CScene;

	m_Scene->Init();

	return true;
}

// �������ִ� ���� ������Ʈ
bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);

	return ChangeScene();;
}

// ������ �ִ� ���� PostUpdate
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

// �׻� ���� ���� �ִ����� Ȯ���Ѵ�. ���� ���� ��� ���� ��ü�ϰ� ���� ��ȯ�Ѵ�.
bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		// ���� ����� �����Ѵ�.
		SAFE_DELETE(m_Scene);

		// �Է� �Ŵ����� ����� �ݹ��Լ����� ��� �ʱ�ȭ��Ų��.
		CInput::GetInst()->ClearCallback();

		// �����Ǿ� �ִ� ���� ����� ���� ������� �����Ѵ�.
		m_Scene = m_NextScene;

		// ������ �ʱ�ȭ�Ѵ�.
		m_NextScene = nullptr;

		return true;
	}

	return false;
}
