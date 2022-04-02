#include "LoadingThread.h"
#include "MainScene.h"
#include "RCGStage01.h"
#include "RCGBossStage.h"
#include "Scene/SceneManager.h"

CLoadingThread::CLoadingThread()
{
}

CLoadingThread::~CLoadingThread()
{
}

bool CLoadingThread::Init()
{
	if (!CThread::Init())
		return false;

	return true;
}

void CLoadingThread::Run()
{
	// ������ ���� ���� �ε��� �����Ѵ�.
	// ���� �ڵ� ����ȯ�� ����.
	CSceneManager::GetInst()->CreateNextScene(false);
	// ���� ���� �����Ѵ�.
	CRCGStage01* mainScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CRCGStage01>(false);

	// ���� ���� �ݹ��Լ��� �����Ѵ�.
	mainScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

	mainScene->Init();

	AddMessage(true, 1.0f);
}
