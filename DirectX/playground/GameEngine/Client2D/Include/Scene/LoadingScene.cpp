#include "LoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "LoadingThread.h"
#include "Scene/SceneManager.h"

CLoadingScene::CLoadingScene()
{
	SetTypeID<CLoadingScene>();
}

CLoadingScene::~CLoadingScene()
{
	SAFE_DELETE(m_LoadingThread);
}

bool CLoadingScene::Init()
{
	m_LoadingWidget = m_Scene->GetViewport()->CreateWidgetWindow<CLoadingWidget>("LoadingWidget");

	// 로딩 스레드 생성
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	m_LoadingThread->Start();

	return true;
}

void CLoadingScene::Update(float deltaTime)
{
	CSceneMode::Update(deltaTime);

	CThreadQueue<sLoadingMessage>* queue = m_LoadingThread->GetLoadingQueue();

	// 만약 메세지 큐에 메세지가 있다면
	if (!queue->empty())
	{
		sLoadingMessage msg = queue->front();

		queue->pop();

		m_LoadingWidget->SetLoadingPercent(msg.percent);

		// 메세지를 확인해보니 로딩이 완료 그럼 씬을 전환한다.
		if (msg.complete)
			CSceneManager::GetInst()->ChangeNextScene();
	}
}
