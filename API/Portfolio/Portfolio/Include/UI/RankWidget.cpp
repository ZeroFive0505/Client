#include "RankWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "UIWindow.h"

CRankWidget::CRankWidget() :
	m_Ranks{},
	m_CurrentRank(0),
	m_Scale(0.0f, 0.0f)
{
}

CRankWidget::CRankWidget(const CRankWidget& widget)
{
	m_CurrentRank = widget.m_CurrentRank;
	m_Scale = widget.m_Scale;

	for (int i = 0; i < 8; i++)
	{
		m_Ranks[i] = widget.m_Ranks[i];
	}
}

CRankWidget::~CRankWidget()
{
}

bool CRankWidget::Init()
{
	m_Scale = Vector2(2.0f, 2.0f);
	
	for (int i = 0; i < 8; i++)
	{
		std::string name = std::to_string(i + 1);
		TCHAR filePath[MAX_PATH] = {};
		wsprintf(filePath, TEXT("UI/Rank/%d.bmp"), i + 1);

		m_Scene->GetSceneResoruce()->LoadTexture(name, filePath);

		m_Ranks[i] = m_Scene->GetSceneResoruce()->FindTexture(name);

		m_Ranks[i]->SetColorKey(255, 0, 255);
		
		m_Ranks[i]->SetScale(m_Scale);
	}

	m_CurrentRank = 8;



	return true;
}

void CRankWidget::Update(float deltaTime)
{
}

void CRankWidget::PostUpdate(float deltaTime)
{
}

void CRankWidget::Collision(float deltaTime)
{
}

void CRankWidget::Render(HDC hDC)
{
	m_Ranks[m_CurrentRank]->Render(hDC, m_Pos, Vector2(0.0f, 0.0f), Vector2(m_Ranks[0]->GetWidth(), m_Ranks[0]->GetHeight()));
}

void CRankWidget::Render(const Vector2& pos, HDC hDC)
{
	m_Ranks[m_CurrentRank]->Render(hDC, pos, Vector2(0.0f, 0.0f), Vector2(m_Ranks[0]->GetWidth(), m_Ranks[0]->GetHeight()));
}

CRankWidget* CRankWidget::Clone()
{
	return new CRankWidget(*this);
}
