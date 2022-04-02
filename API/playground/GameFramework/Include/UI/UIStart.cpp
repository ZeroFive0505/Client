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
	// ���� �ػ󵵸� �޾ƿ´�.
	Resolution RS = CGameManager::GetInst()->GetResolution();

	// ��ư�� �����Ѵ�.(��ŸƮ)
	CButton* Button = CreateWidget<CButton>("StartButton");

	// ��ŸƮ ��ư�� �����ϰ� ��ġ�� �����Ѵ�.
	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.0f - 100.0f, RS.Height / 2.0f - 175.0f);
	// ��ư�� ��� ���¿� ���� �ִϸ��̼��� ���� �� �ִ�.
	Button->SetFrameData(EButton_State::Normal, Vector2(0.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.0f, 0.0f), Vector2(200.0f, 100.0f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.0f, 0.0f), Vector2(200.0f, 100.0f));
	// ���콺�� ��ư���� �ö������ ��� ���Ǵ� ����
	Button->SetMouseOnSound("ButtonMouseOn");
	// ��ư�� Ŭ���� ��� ����Ǵ� ����
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);
	Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);

	// �ؽ�Ʈ�� �����.
	CUIText* Text = CreateWidget<CUIText>("StartButtonText");

	// �ؽ�Ʈ�� ������ �ϰ� Z���� 2���Ѵ� ���� ��ư���� ��µȴ�.
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
	// ��ư�� ������ ���ξ����� �Ѿ��.
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
