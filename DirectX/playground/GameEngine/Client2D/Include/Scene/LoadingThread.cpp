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
	// 스레드 동작 시작 로딩을 시작한다.
	// 먼저 자동 씬전환을 끈다.
	CSceneManager::GetInst()->CreateNextScene(false);
	// 다음 씬을 생성한다.
	CRCGStage01* mainScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CRCGStage01>(false);

	// 다음 씬의 콜백함수를 설정한다.
	mainScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

	mainScene->Init();

	AddMessage(true, 1.0f);
}
