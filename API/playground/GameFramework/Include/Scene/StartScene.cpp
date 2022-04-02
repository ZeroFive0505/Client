#include "StartScene.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../UI/UIStart.h"


CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	LoadSound();

	LoadAniamtionSequence();

	CUIStart* StartWindow = CreateUIWindow<CUIStart>("StartWindow");

	return true;
}

void CStartScene::LoadAniamtionSequence()
{
}

void CStartScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("StartBGM");

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "1Up.wav");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "Stun.wav");

	GetSceneResource()->SetVolume("UI", 70);
}
