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

	// �ؽ�Ʈ â�� �����.
	CUIImage* Back = CreateWidget<CUIImage>("TextBarBack");

	Back->SetTexture("Back", TEXT("Back.bmp"));

	Back->SetPos(RS.Width / 2.0f - 200.0f, RS.Height - 150.0f);

	// �ؽ�Ʈ ������ �����.
	m_Text = CreateWidget<CUIText>("TestText");

	m_Text->SetPos(RS.Width / 2.0f - 180.f, RS.Height - 130.0f);
	m_Text->SetTextColor(255, 255, 255);
	m_Text->SetZOrder(1);

	m_TextTime = 0.0f;
	m_OutputText = 0;
	m_OutputIndex = 0;

	// ���� ������ �����.
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
	// ���� CUIMain�� Update�� ������ �ߴ�. ���� �⺻ ������ ������Ʈ�� ȣ���Ϸ���
	// �θ��� Update�� ȣ���ؾ��Ѵ�.
	CUIWindow::Update(DeltaTime);

	m_TextTime += DeltaTime;

	// 0.1�ʸ��� ���Ӱ� �ؽ�Ʈ�� ����Ѵ�.
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

		// ���� ���� ��µǴ� �ؽ�Ʈ�� ���� ����ߴٸ�
		if (lstrlen(TestText[m_OutputText]) == m_OutputIndex)
		{
			// �ؽ�Ʈ�� �ٲ۴�.
			m_OutputIndex = 0;
			m_Text->Clear();
			m_OutputText = (m_OutputText + 1) % 2;
		}
	}

	// �ð��� �����Ѵ�.
	m_Time += DeltaTime;

	// 60�ʰ� �Ѿ��� ��� ���� �÷��ش�.
	if (m_Time >= 60.0f)
	{
		m_Minute++;

		m_Time -= 60.0f;
	}

	int Number1, Number2;

	// �ʸ� 2���� �������� ǥ���Ѵ�.
	Number1 = (int)m_Time / 10;
	Number2 = (int)m_Time % 10;

	m_MinuteWidget->SetNumber((int)m_Minute);
	m_NumberWidget->SetNumber((int)Number1);
	m_Number1Widget->SetNumber((int)Number2);
}
