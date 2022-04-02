#include "UIMain.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "UIText.h"
#include "NumberWidget.h"

CUIMain::CUIMain()
{
}

CUIMain::~CUIMain()
{
}

bool CUIMain::Init()
{
	Resolution RS = CGameManager::GetInst()->GetResolution();

	// 텍스트 창을 만든다.
	CUIImage* Back = CreateWidget<CUIImage>("TextBarBack");

	Back->SetTexture("Back", TEXT("Back.bmp"));

	Back->SetPos(RS.Width / 2.0f - 200.0f, RS.Height - 150.0f);

	// 텍스트 위젯을 만든다.
	m_Text = CreateWidget<CUIText>("TestText");

	m_Text->SetPos(RS.Width / 2.0f - 180.f, RS.Height - 130.0f);
	m_Text->SetTextColor(255, 255, 255);
	m_Text->SetZOrder(1);

	m_TextTime = 0.0f;
	m_OutputText = 0;
	m_OutputIndex = 0;

	// 숫자 위젯을 만든다.
	m_NumberWidget = CreateWidget<CNumberWidget>("Number");

	std::vector<std::wstring> vecNumberFileName;

	for (int i = 0; i < 10; i++)
	{
		TCHAR FileName[256] = {};

		wsprintf(FileName, TEXT("Number/%d.bmp"), i);

		vecNumberFileName.push_back(FileName);
	}

	m_NumberWidget->SetTexture("Number", vecNumberFileName);

	for (int i = 0; i < 10; i++)
	{
		m_NumberWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_NumberWidget->SetPos(518.0f, 100.0f);
	m_NumberWidget->SetSize(29.0f, 48.0f);

	m_Number1Widget = CreateWidget<CNumberWidget>("Number1");

	m_Number1Widget->SetTexture("Number", vecNumberFileName);

	for (int i = 0; i < 10; i++)
	{
		m_Number1Widget->SetTextureColorKey(255, 255, 255, i);
	}

	m_Number1Widget->SetPos(547.0f, 100.0f);
	m_Number1Widget->SetSize(29.0f, 48.0f);

	m_MinuteWidget = CreateWidget<CNumberWidget>("Minute");

	m_MinuteWidget->SetTexture("Number", vecNumberFileName);

	for (int i = 0; i < 10; i++)
	{
		m_MinuteWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_MinuteWidget->SetPos(460.0f, 100.0f);
	m_MinuteWidget->SetSize(29.0f, 48.0f);

	CUIImage* Colon = CreateWidget<CUIImage>("Colon");

	Colon->SetTexture("Colon", TEXT("Number/Colon.bmp"));
	Colon->SetTextureColorKey(255, 255, 255);
	Colon->SetPos(489.0f, 100.0f);

	m_Time = 0.0f;
	m_Minute = 0;

	return true;
}

void CUIMain::Update(float DeltaTime)
{
	// 현재 CUIMain은 Update를 재정의 했다. 따라서 기본 위젯의 업데이트를 호출하려면
	// 부모의 Update를 호출해야한다.
	CUIWindow::Update(DeltaTime);

	m_TextTime += DeltaTime;

	// 0.1초마다 새롭게 텍스트를 출력한다.
	if (m_TextTime >= 0.1f)
	{
		m_TextTime -= 0.1f;

		const TCHAR* TestText[2] =
		{
			TEXT("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut enim leo, sagittis a nisi sed, pretium tempus erat."),
			TEXT("Donec ac ligula non purus posuere pharetra eget non mauris. Phasellus ac sagittis dui. Suspendisse commodo leo id lorem placerat, vel imperdiet metus semper.")
		};

		TCHAR Select[2] = { TestText[m_OutputText][m_OutputIndex], 0};

		m_Text->AddText(Select);

		m_Scene->GetSceneResource()->SoundPlay("TextSound");

		m_OutputIndex++;

		// 만약 현재 출력되는 텍스트를 전부 출력했다면
		if (lstrlen(TestText[m_OutputText]) == m_OutputIndex)
		{
			// 텍스트를 바꾼다.
			m_OutputIndex = 0;
			m_Text->Clear();
			m_OutputText = (m_OutputText + 1) % 2;
		}
	}

	// 시간을 누적한다.
	m_Time += DeltaTime;

	// 60초가 넘었을 경우 분을 늘려준다.
	if (m_Time >= 60.0f)
	{
		m_Minute++;

		m_Time -= 60.0f;
	}

	int Number1, Number2;

	// 초를 2개의 위젯으로 표현한다.
	Number1 = (int)m_Time / 10;
	Number2 = (int)m_Time % 10;

	m_MinuteWidget->SetNumber((int)m_Minute);
	m_NumberWidget->SetNumber((int)Number1);
	m_Number1Widget->SetNumber((int)Number2);
}
