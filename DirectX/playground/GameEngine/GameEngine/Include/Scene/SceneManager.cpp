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

    // 씬 매니저를 초기화하면서 동시에 렌더매니저에 렌더링할 오브젝트들을 등록시킨다.
    // 참조를 받아서 등록하기에 한번 등록하면 언제든지 현재 씬에 소속된 오브젝트들을 순회할 수 있다.
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
    // 씬 변화시에는 동기화를 이용한다.
    // 이 변수는 스택영역에 할당되어 자동으로 소멸되며 락을 푼다.
    CSync sync(&m_Crt);

    if (m_NextScene)
    {
        // 다음 씬이 모두 준비가 되었다면
        if (m_NextScene->m_Change)
        {
            // 씬을 교체한다.
            SAFE_DELETE(m_Scene);
            m_Scene = m_NextScene;
            m_NextScene = nullptr;

            // 교체된 씬에 있는 모든 오브젝트들을 가져와 설정한다.
            CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

            m_Scene->Start();

            return true;
        }
    }

    return false;
}

void CSceneManager::CreateNextScene(bool autoChange)
{
    // 다음 씬을 생성시에도 동기화를 이용한다.
    CSync sync(&m_Crt);

    SAFE_DELETE(m_NextScene);

    m_NextScene = new CScene;

    m_NextScene->SetAutoChange(autoChange);
}

void CSceneManager::ChangeNextScene()
{
    // 다음 씬으로 변환할때도 동기화를 이용
    CSync sync(&m_Crt);

    m_NextScene->SetAutoChange(true);
}
