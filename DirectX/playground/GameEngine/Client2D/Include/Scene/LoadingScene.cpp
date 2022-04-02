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

	// �ε� ������ ����
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	m_LoadingThread->Start();

	return true;
}

void CLoadingScene::Update(float deltaTime)
{
	CSceneMode::Update(deltaTime);

	CThreadQueue<sLoadingMessage>* queue = m_LoadingThread->GetLoadingQueue();

	// ���� �޼��� ť�� �޼����� �ִٸ�
	if (!queue->empty())
	{
		sLoadingMessage msg = queue->front();

		queue->pop();

		m_LoadingWidget->SetLoadingPercent(msg.percent);

		// �޼����� Ȯ���غ��� �ε��� �Ϸ� �׷� ���� ��ȯ�Ѵ�.
		if (msg.complete)
			CSceneManager::GetInst()->ChangeNextScene();
	}
}
