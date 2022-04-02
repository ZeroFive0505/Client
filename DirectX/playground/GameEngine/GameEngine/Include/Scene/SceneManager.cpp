#include "SceneManager.h"
#include "../Render/RenderManager.h"
#include "../Sync.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
    m_Scene(nullptr),
    m_NextScene(nullptr)
{
    InitializeCriticalSection(&m_Crt);
}

CSceneManager::~CSceneManager()
{
    DeleteCriticalSection(&m_Crt);
    SAFE_DELETE(m_Scene);
    SAFE_DELETE(m_NextScene);
}

void CSceneManager::Start()
{
    m_Scene->Start();
}

bool CSceneManager::Init()
{
    m_Scene = new CScene;

    // �� �Ŵ����� �ʱ�ȭ�ϸ鼭 ���ÿ� �����Ŵ����� �������� ������Ʈ���� ��Ͻ�Ų��.
    // ������ �޾Ƽ� ����ϱ⿡ �ѹ� ����ϸ� �������� ���� ���� �Ҽӵ� ������Ʈ���� ��ȸ�� �� �ִ�.
    CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

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

bool CSceneManager::ChangeScene()
{
    // �� ��ȭ�ÿ��� ����ȭ�� �̿��Ѵ�.
    // �� ������ ���ÿ����� �Ҵ�Ǿ� �ڵ����� �Ҹ�Ǹ� ���� Ǭ��.
    CSync sync(&m_Crt);

    if (m_NextScene)
    {
        // ���� ���� ��� �غ� �Ǿ��ٸ�
        if (m_NextScene->m_Change)
        {
            // ���� ��ü�Ѵ�.
            SAFE_DELETE(m_Scene);
            m_Scene = m_NextScene;
            m_NextScene = nullptr;

            // ��ü�� ���� �ִ� ��� ������Ʈ���� ������ �����Ѵ�.
            CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

            m_Scene->Start();

            return true;
        }
    }

    return false;
}

void CSceneManager::CreateNextScene(bool autoChange)
{
    // ���� ���� �����ÿ��� ����ȭ�� �̿��Ѵ�.
    CSync sync(&m_Crt);

    SAFE_DELETE(m_NextScene);

    m_NextScene = new CScene;

    m_NextScene->SetAutoChange(autoChange);
}

void CSceneManager::ChangeNextScene()
{
    // ���� ������ ��ȯ�Ҷ��� ����ȭ�� �̿�
    CSync sync(&m_Crt);

    m_NextScene->SetAutoChange(true);
}
