#include "UIStart.h"
#include "Button.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/EditorScene.h"
#include "../Scene/SceneManager.h"
#include "UIText.h"

CUIStart::CUIStart()
{
}

CUIStart::~CUIStart()
{
}

bool CUIStart::Init()
{
	// 현재 해상도를 받아온다.
	Resolution RS = CGameManager::GetInst()->GetResolution();

	// 버튼을 생성한다.(스타트)
	CButton* Button = CreateWidget<CButton>("StartButton");

	// 스타트 버튼을 생성하고 위치를 설정한다.
	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.0f - 100.0f, RS.Height / 2.0f - 175.0f);
	// 버튼의 경우 상태에 따른 애니메이션을 가질 수 있다.
	Button->SetFrameData(EButton_State::Normal, Vector2(0.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.0f, 0.0f), Vector2(200.0f, 100.0f));
	// 마우스가 버튼위에 올라와있을 경우 재상되는 사운드
	Button->SetMouseOnSound("ButtonMouseOn");
	// 버튼이 클릭될 경우 재생되는 사운드
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);
	Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);

	// 텍스트를 만든다.
	CUIText* Text = CreateWidget<CUIText>("StartButtonText");

	// 텍스트의 설정을 하고 Z값을 2로한다 따라서 버튼위에 출력된다.
	Text->SetText(TEXT("START"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.0f - 50.0f, RS.Height / 2.0f - 155.0f);
	Text->SetZOrder(2);


	Button = CreateWidget<CButton>("EditorButton");

	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.0f - 100.0f, RS.Height / 2.0f - 50.0f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIStart>(this, &CUIStart::EditorClick);

	Text = CreateWidget<CUIText>("EditorButtonText");

	Text->SetText(TEXT("EDITOR"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.0f - 50.0f, RS.Height / 2.0f - 30.0f);
	Text->SetZOrder(2);

	Button = CreateWidget<CButton>("ExitButton");

	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.0f - 100.0f, RS.Height / 2.0f + 75.0f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);
	Button->SetClickCallback<CUIStart>(this, &CUIStart::ExitClick);

	Text = CreateWidget<CUIText>("ExitButton");

	Text->SetText(TEXT("EXIT"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.0f - 50.0f, RS.Height / 2.0f + 95.0f);
	Text->SetZOrder(2);

	return true;
}

void CUIStart::StartClick()
{
	// 버튼이 눌릴시 메인씬으로 넘어간다.
	CSceneManager::GetInst()->CreateScene<CMainScene>();
}

void CUIStart::ExitClick()
{
	CGameManager::GetInst()->Exit();
}

void CUIStart::EditorClick()
{
	CSceneManager::GetInst()->CreateScene<CEditorScene>();
}
