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
	// ���� �� �������� ��ġ�� ���Ѵ�.
	SetPos(30.0f, 20.0f);

	Resolution RS = CGameManager::GetInst()->GetResolution();

	// ���α׷������� ���
	CUIImage* Back = CreateWidget<CUIImage>("HPBarBack");

	// ����� ����
	Back->SetTexture("BarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);

	// ü�� ����
	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	m_HPBar->SetTexture("HPBar", TEXT("BarDefault.bmp"));
	// ü�¹ٴ� �� �����쿡 ���� �����̴�. ���� �� ������ ��ġ���� �������� ������� ��ġ.
	m_HPBar->SetPos(10.0f, 10.0f);

	return true;
}
