#include "CRCGLoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "LoadingThread.h"
#include "Scene/SceneManager.h"


CRCGLoadingScene::CRCGLoadingScene()
{
    SetTypeID<CRCGLoadingScene>();
}

CRCGLoadingScene::~CRCGLoadingScene()
{
    SAFE_DELETE(m_LoadingThread);
}

bool CRCGLoadingScene::Init()
{
    LoadHitEffect();

    m_LoadingWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGLoadingWidget>("RCG Loading Widget");

    m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

    m_LoadingThread->Start();


    return true;
}

void CRCGLoadingScene::Update(float deltaTime)
{
    CSceneMode::Update(deltaTime);

    CThreadQueue<sLoadingMessage>* queue = m_LoadingThread->GetLoadingQueue();

    if (!queue->empty())
    {
        sLoadingMessage msg = queue->front();

        queue->pop();

        if (msg.complete)
            CSceneManager::GetInst()->ChangeNextScene();
    }
}

void CRCGLoadingScene::LoadHitEffect()
{
    CResourceManager::GetInst()->CreateAnimationSequence2D("HitVFX", "HitVFX", TEXT("fx_pixel_laserspak_4x2.png"));

    for (int i = 0; i < 4; i++)
    {
        CResourceManager::GetInst()->AddAnimationSequence2DFrame("HitVFX", 
            Vector2(i * 128.0f, 0.0f), Vector2(128.0f, 128.0f));
    }

    for (int i = 0; i < 3; i++)
    {
        CResourceManager::GetInst()->AddAnimationSequence2DFrame("HitVFX", 
            Vector2(i * 128.0f, 128.0f), Vector2(128.0f, 128.0f));
    }
}
