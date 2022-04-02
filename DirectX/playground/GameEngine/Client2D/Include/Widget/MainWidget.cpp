#include "MainWidget.h"
#include "Engine.h"

CMainWidget::CMainWidget()
{
	SetTypeID<CMainWidget>();
}

CMainWidget::CMainWidget(const CMainWidget& window)
{
	m_Button = FindWidget<CButton>("Button");
	m_Button1 = FindWidget<CButton>("Button1");
	m_Button1Text = FindWidget<CText>("Text");
	m_Hour = FindWidget<CNumber>("Hour");
	m_Minute = FindWidget<CNumber>("Minute");
	m_Second = FindWidget<CNumber>("Second");
	m_FPSText = FindWidget<CText>("FPSText");
}

CMainWidget::~CMainWidget()
{
}

void CMainWidget::Start()
{
	CWidgetWindow::Start();
}

bool CMainWidget::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(1280.0f, 720.0f);

	m_Button = CreateWidget<CButton>("Button");

	m_SliderWidget = CreateWidget<CSliderWidget>("Slider");

	m_SliderWidget->SetPos(100.0f, 500.0f);
	m_SliderWidget->SetSize(200.0f, 50.0f);
	m_SliderWidget->SetTextureTint(255, 0, 0, 255);
	m_SliderWidget->SetBGTexturePos(100.0f, 500.0f);
	m_SliderWidget->SetBGTextureSize(200.0f, 50.0f);
	m_SliderWidget->SetBGTextureTint(127, 0, 0, 127);
	m_SliderWidget->SetPercent(0.5f);
	m_SliderWidget->SetDirection(ProgressBar_Direction::BottomToTop);
	m_SliderWidget->SetHandleSize(20.0f, 50.0f);
	m_SliderWidget->SetHandleTextureTint(255, 255, 255, 255);
	m_SliderWidget->SetHandlePos(m_SliderWidget->GetWindowPos().x +
		(m_SliderWidget->GetPercent() * m_SliderWidget->GetWindowSize().x - 
			(m_SliderWidget->GetHandleSize().x / 2.0f)), m_SliderWidget->GetWindowPos().y);


	m_Button->SetPos(100.0f, 300.0f);
	m_Button->SetSize(100.0f, 100.0f);
	m_Button->SetTexture(Button_State::Normal, "MainTestButton", TEXT("Player.png"));
	m_Button->SetTexture(Button_State::MouseOn, "MainTestButtonOn", TEXT("Start.png"));
	m_Button->SetTexture(Button_State::Click, "MainTestButtonClick", TEXT("Start.png"));
	
	m_Button->SetPlayScale(Button_State::Normal, 1.0f);
	m_Button->SetPlayTime(Button_State::Normal, 1.0f);
	for (int i = 0; i < 7; ++i)
	{
		//void AddFrameData(Button_State state, const Vector2& start, const Vector2& size);
		m_Button->AddFrameData(Button_State::Normal, Vector2(i * 50.0f, 148.0f), Vector2(50.0f, 37.0f));
		//m_Scene->GetSceneResource()->AddAnimationSequence2DFrame("EnemyIdle", Vector2(i * 50.f, 148.f), Vector2(50.f, 37.f));
	}

	m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

	m_Button->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
	m_Button->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "TeemoStartClicck.mp3");

	m_Button1 = CreateWidget<CButton>("Button1");

	m_Button1->SetPos(50.0f, 200.0f);
	m_Button1->SetSize(100.0f, 40.0f);

	m_Button1->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button1->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button1->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

	m_Button1->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
	m_Button1->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "TeemoStartClicck.mp3");

	m_Button1Text = CreateWidget<CText>("Text");

	m_Button1Text->SetText(TEXT("Button"));
	m_Button1Text->SetPos(50.0f, 200.0f);
	m_Button1Text->SetSize(100.0f, 40.0f);
	m_Button1Text->SetZOrder(1);
	m_Button1Text->SetColor(1.0f, 0.0f, 0.0f);
	m_Button1Text->SetAlignH(TEXT_ALIGN_H::Center);
	m_Button1Text->SetShadowEnable(true);
	m_Button1Text->SetShadowOffset(2.0f, 2.0f);

	m_Hour = CreateWidget<CNumber>("Hour");
	m_Minute = CreateWidget<CNumber>("Minute");
	m_Second = CreateWidget<CNumber>("Second");

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);

		vecFileName.push_back(FileName);
	}

	m_Hour->SetTexture("Number", vecFileName);
	m_Hour->AddFrameData(10);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);

		vecFileName.push_back(FileName);
	}

	m_Minute->SetTexture("Number", vecFileName);
	m_Minute->AddFrameData(10);
	m_Minute->SetPos(100.f, 0.f);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);

		vecFileName.push_back(FileName);
	}

	m_Second->SetTexture("Number", vecFileName);
	m_Second->AddFrameData(10);
	m_Second->SetPos(200.f, 0.f);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

	m_FPSText = CreateWidget<CText>("FPSText");
	
	m_FPSText->SetText(TEXT("FPS"));
	m_FPSText->SetPos(900.0f, 650.0f);
	m_FPSText->SetSize(300.0f, 40.0f);
	m_FPSText->SetZOrder(1);
	m_FPSText->SetColor(0.0f, 1.0f, 0.0f);
	m_FPSText->SetAlignH(TEXT_ALIGN_H::Center);

	return true;
}

void CMainWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);

	SYSTEMTIME time;

	GetLocalTime(&time);

	m_Hour->SetNumber((int)time.wHour);
	m_Minute->SetNumber((int)time.wMinute);
	m_Second->SetNumber((int)time.wSecond);

	char fps[256] = {};
	sprintf_s(fps, "FPS : %.5f", CEngine::GetInst()->GetFPS());

	TCHAR wFPS[256] = {};
	
	int length = MultiByteToWideChar(CP_ACP, 0, fps, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, fps, -1, wFPS, length);

	m_FPSText->SetText(wFPS);
}

void CMainWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMainWidget::Render()
{
	CWidgetWindow::Render();
}
