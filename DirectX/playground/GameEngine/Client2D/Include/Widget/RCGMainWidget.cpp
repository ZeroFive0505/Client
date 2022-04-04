#include "RCGMainWidget.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../Object/RCGPlayer.h"

CRCGMainWidget::CRCGMainWidget()
{
	SetTypeID<CRCGMainWidget>();
}

CRCGMainWidget::CRCGMainWidget(const CRCGMainWidget& window)
{
	m_Border = FindWidget<CImage>("Border");
	m_PlayerHUD = FindWidget<CImage>("PlayerHUD");
	m_CharacterIcon = FindWidget<CImage>("CharacterIcon");
	m_SpecialBar = FindWidget<CProgressBar>("SpecialBar");
	m_SpecialBarBack = FindWidget<CImage>("SpecialBarBack");
	m_FadeOut = FindWidget<CImage>("FadeOut");
	m_BattleIntro = CreateWidget<CImage>("BattleIntro");
}

CRCGMainWidget::~CRCGMainWidget()
{
}

void CRCGMainWidget::Start()
{
	CWidgetWindow::Start();
}

bool CRCGMainWidget::Init()
{
	sResolution rs = CEngine::GetInst()->GetResolution();

	SetSize((float)rs.width, (float)rs.height);
	
	SetZOrder(1);

	m_Border = CreateWidget<CImage>("Border");
	m_PlayerHUD = CreateWidget<CImage>("PlayerHUD");
	m_CharacterIcon = CreateWidget<CImage>("CharacterIcon");
	m_SpecialBar = CreateWidget<CProgressBar>("SpecialBar");
	m_SpecialBarBack = CreateWidget<CImage>("SpecialBarBack");
	m_FadeOut = CreateWidget<CImage>("FadeOut");
	m_BattleIntro = CreateWidget<CImage>("BattleIntro");

	m_PlayerHUD->SetZOrder(1);

	m_CharacterIcon->SetZOrder(3);
	m_SpecialBar->SetZOrder(3);

	m_Border->SetPos(Vector2(0.0f, 0.0f));
	m_Border->SetSize((float)rs.width, (float)rs.height);
	m_Border->SetTexture("UIBorder", TEXT("UI_Map_BORDER.png"));
	m_Border->SetCollisionEnable(false);

	m_FadeOut->SetPos(0.0f, 0.0f);
	m_FadeOut->SetSize((float)rs.width, (float)rs.height);
	m_FadeOut->SetTexture("FadeOut", TEXT("Background.png"));
	m_FadeOut->SetOpacity(0.0f);
	m_FadeOut->SetCollisionEnable(false);
	m_FadeOut->SetZOrder(5);

	m_PlayerHUD->SetPos(100.0f, (float)rs.height - 100.0f);
	m_PlayerHUD->SetTexture("PlayerHUD", TEXT("UI_HUD_player_bars_overlay.png"));
	m_PlayerHUD->SetSize(m_PlayerHUD->GetTextureSize(0));
	m_PlayerHUD->SetCollisionEnable(false);

	m_CharacterIcon->SetPos(Vector2(60.0f, (float)rs.height - 120.0f));
	m_CharacterIcon->SetCollisionEnable(false);
	m_CharacterIcon->SetSize(120.0f, 120.0f);
	m_CharacterIcon->SetTexture("Charactet Icon", TEXT("UI_RCG_HUD_portrait_Kyoko_default.png"));

	m_SpecialBar->SetPos(Vector2(190.0f, (float)rs.height - 60.0f));
	m_SpecialBar->SetSize(300.0f, 15.0f);
	int guage = CClientManager::GetInst()->GetManager()->GetSpecialGuage();
	m_SpecialBar->SetPercent(guage / 100.0f);
	m_SpecialBar->SetCollisionEnable(false);
	m_SpecialBar->SetTexture("Special Bar", TEXT("UI_HUD_player_fill_special.png"));
	m_SpecialBar->SetTextureTint(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_SpecialBarBack->SetPos(Vector2(190.0f, (float)rs.height - 60.0f));
	m_SpecialBarBack->SetSize(300.0f, 15.0f);
	m_SpecialBarBack->SetCollisionEnable(false);
	m_SpecialBarBack->SetTexture("Special BarBack", TEXT("UI_HUD_player_fill_special.png"));
	m_SpecialBarBack->SetTextureTint(Vector4(1.0f, 1.0f, 1.0f, 0.5f));

	CImage* hp;
	float offset = 18.0f;
	for (int i = 0; i < 16; i++)
	{
		hp = CreateWidget<CImage>("HP " + std::to_string(i + 1));
		hp->SetTexture("HP", TEXT("UI_HUD_player_fill_stamina_single_digit.png"));
		hp->SetSize(30.0f, 20.0f);
		hp->SetPos(190.0f + (i * offset), (float)rs.height - 40.0f);
		hp->SetZOrder(3);
		m_HPBar.push_back(hp);
	}

	m_LerpStart = false;
	m_FadeOutStart = false;
	m_FadeOutComplete = false;
	m_FadeOutSpeed = 1.0f;


	std::vector<TCHAR*> vecFileName;

	for (int i = 1; i <= 16; i++)
	{
		TCHAR* fileName;

		fileName = new TCHAR[MAX_PATH];

		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(fileName, TEXT("BattleIntro\\battle_intro_%d.png"), i);

		vecFileName.push_back(fileName);
	}

	m_BattleIntro->SetTexture("BattleIntro", vecFileName);
	m_BattleIntro->SetSize((float)rs.width * 0.5f, (float)rs.height * 0.5f);
	m_BattleIntro->SetPos((float)rs.width * 0.25f, (float)rs.height * 0.25f);
	m_BattleIntro->SetPlayScale(0.8f);
	m_BattleIntro->AddFrameData(16);
	m_BattleIntro->SetZOrder(7);
	m_BattleIntro->SetPlayOnce(true);

	size_t size = vecFileName.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(vecFileName[i]);
	}

	return true;
}

void CRCGMainWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);

	if (m_LerpStart)
	{
		m_LerpTime += deltaTime;

		float a = m_SpecialBar->GetPercent();
		float b = m_TargetValue;

		if (m_LerpTime >= 1.0f)
		{
			m_LerpStart = false;
			m_LerpTime = 1.0f;
		}

		float t = a + m_LerpTime * (b - a);

		m_SpecialBar->SetPercent(t);
	}

	if (m_FadeOutStart && !m_FadeOutComplete)
	{
		m_FadeOutTime += deltaTime * m_FadeOutSpeed;

		if (m_FadeOutTime >= 1.0f)
		{
			m_FadeOutTime = 1.0f;
			m_FadeOutComplete = true;
			if (m_SceneChangeCallback)
				m_SceneChangeCallback();
		}

		m_FadeOut->SetOpacity(m_FadeOutTime);
	}
}

void CRCGMainWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CRCGMainWidget::Render()
{
	CWidgetWindow::Render();
}

void CRCGMainWidget::FadeOutStart()
{
	m_FadeOutTime = 0.0f;
	SavePlayerInfo();
	m_FadeOutStart = true;
}

void CRCGMainWidget::SavePlayerInfo()
{
	CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();
	CClientManager::GetInst()->GetManager()->SetHealth(player->GetHealth());
	CClientManager::GetInst()->GetManager()->SetSpecialGuage(player->GetSP());
}
