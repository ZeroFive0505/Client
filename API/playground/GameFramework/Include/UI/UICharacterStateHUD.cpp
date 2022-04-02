#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD()
{
}

CUICharacterStateHUD::~CUICharacterStateHUD()
{
}

bool CUICharacterStateHUD::Init()
{
	// 먼저 이 윈도우의 위치를 정한다.
	SetPos(30.0f, 20.0f);

	Resolution RS = CGameManager::GetInst()->GetResolution();

	// 프로그레스바의 배경
	CUIImage* Back = CreateWidget<CUIImage>("HPBarBack");

	// 배경의 설정
	Back->SetTexture("BarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);

	// 체력 생성
	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	m_HPBar->SetTexture("HPBar", TEXT("BarDefault.bmp"));
	// 체력바는 이 윈도우에 속한 위젯이다. 따라서 이 위젯에 위치값은 윈도우의 상대적인 위치.
	m_HPBar->SetPos(10.0f, 10.0f);

	return true;
}
