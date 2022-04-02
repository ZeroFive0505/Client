#include "RaceGUI.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "UIText.h"
#include "ItemWidget.h"
#include "TimerWidget.h"
#include "CoinWidget.h"
#include "RankWidget.h"
#include "UISpriteText.h"


CRaceGUI::CRaceGUI() :
    m_CoinWidget(nullptr),
    m_ItemWidget(nullptr),
    m_Thunder(false),
    m_ThunderCnt(0),
    m_EffectEnd(false),
    m_RaceFinished(false)
{

}

CRaceGUI::~CRaceGUI()
{
}

void CRaceGUI::AddCoin(int amount)
{
    if (m_CoinWidget)
        m_CoinWidget->AddCoin(amount);
}

void CRaceGUI::SetItem(EItem item)
{
    if (m_ItemWidget)
        m_ItemWidget->SetItem(item);
}

void CRaceGUI::SetRank(int rank)
{
    if (m_RankWidget)
        m_RankWidget->SetRank(rank - 1);
}

bool CRaceGUI::CanUseItem()
{
    if (m_ItemWidget)
        return m_ItemWidget->CanUse();
    return false;
}

void CRaceGUI::EffectStart()
{
    Resolution rs = CGameManager::GetInst()->GetResolution();
    m_FadeOut = true;
    m_FadeIn = false;
    m_EffectEnd = false;
    m_BlackScreen = true;
    m_LT = Vector2(rs.width / 2.0f - 1, rs.height / 2.0f - 1);
    m_RB = Vector2(m_LT.x + 1, m_LT.y + 1);
    m_BlackScreenTime = 0.0f;
}

void CRaceGUI::ForceStop()
{
    m_ItemWidget->SetForceStop();
}

void CRaceGUI::AddFinishTime(EMenuPlayer character, float time)
{
    std::string s;
    std::string name;
    switch (character)
    {
    case EMenuPlayer::MARIO:
        s = "MARIO ";
        name = s;
        break;
    case EMenuPlayer::PEARCH:
        s = "PEACH ";
        name = s;
        break;
    case EMenuPlayer::BOWSER:
        s = "BOWSER ";
        name = s;
        break;
    case EMenuPlayer::KOOPA:
        s = "KOOPA ";
        name = s;
        break;
    case EMenuPlayer::LUIGI:
        s = "LUIGI ";
        name = s;
        break;
    case EMenuPlayer::YOSHI:
        s = "YOSHI ";
        name = s;
        break;
    case EMenuPlayer::DK:
        s = "DK JR ";
        name = s;
        break;
    case EMenuPlayer::TOAD:
        s = "TOAD ";
        name = s;
        break;
    }

    unsigned int currentTimeAsMili = unsigned int(time * 1000);

    int minutes = currentTimeAsMili / 60000;

    currentTimeAsMili -= minutes * 60000;

    int seconds = currentTimeAsMili / 1000;

    currentTimeAsMili -= seconds * 1000;

    int mili = currentTimeAsMili / 10;

    s += std::to_string(minutes) + " ";
    s += std::to_string(seconds) + " ";
    s += std::to_string(mili);

    CUISpriteText* text = CreateWidget<CUISpriteText>(name);

    text->SetText(s);

    text->SetVisibility(false);

    m_FinishTimes.push_back(text);
}

bool CRaceGUI::Init()
{
    Resolution rs = CGameManager::GetInst()->GetResolution();

    CItemWidget* itemWidget = CreateWidget<CItemWidget>("Item Widget");

    itemWidget->SetPos(rs.width / 2.0f + 50.0f, 15.0f);

    m_ItemWidget = itemWidget;

    CTimerWidget* timerWidget = CreateWidget<CTimerWidget>("Timer Widget");

    timerWidget->SetWindowSize();

    CCoinWidget* coinWidget = CreateWidget<CCoinWidget>("Coin Widget");

    coinWidget->SetPos(rs.width * 75.0f / 100.0f, (rs.height / 2.0f) * 80.0f / 100.0f);

    m_CoinWidget = coinWidget;

    CRankWidget* rankWidget = CreateWidget<CRankWidget>("Rank Widget");

    rankWidget->SetPos(rs.width * 92.0f / 100.0f, (rs.height / 2.0f) * 80.0f / 100.0f);

    m_RankWidget = rankWidget;

    return true;
}

void CRaceGUI::Update(float deltaTime)
{

	for (int i = 0; i < m_WidgetCount; i++)
	{
		m_WidgetArray[i]->Update(deltaTime);
	}

	if (m_BlackScreen)
	{
		if (m_FadeOut)
		{
			float pct = fminf(m_BlackScreenTime / GUI_FADEOUT_TIME, 1.0f);
			m_BlackScreenTime += deltaTime;

			if (pct >= 1.0f)
			{
				m_FadeOut = false;
				m_FadeIn = true;
				m_BlackScreenTime = 0.0f;
			}
			else
			{
				m_LT.x = 256.0f - (255.0f * pct);
				m_LT.y = 224.0f - (223.0f * pct);
				m_RB.x = 256.0f + (255.0f * pct);
				m_RB.y = 224.0f + (223.0f * pct);
			}


		}
		else if (m_FadeIn)
		{
			float pct = fminf(m_BlackScreenTime / GUI_FADEIN_TIME, 1.0f);

			m_BlackScreenTime += deltaTime;

			if (pct >= 1.0f)
			{
				m_BlackScreen = false;
				m_FadeIn = false;
				m_FadeOut = false;
				m_EffectEnd = true;
				m_BlackScreenTime = 0.0f;
			}
			else
			{
				m_LT.x = 0.0f + (255.0f * pct);
				m_LT.y = 0.0f + (223.0f * pct);
				m_RB.x = 512.0f - (255.0f * pct);
				m_RB.y = 448.0f - (223.0f * pct);
			}
		}
	}
 

    if (m_RaceFinished)
    {
        for (size_t i = 0; i < m_FinishTimes.size(); i++)
        {
            m_FinishTimes[i]->SetVisibility(true);
        }
    }
}

void CRaceGUI::Render(HDC hDC)
{
        
    Resolution rs = CGameManager::GetInst()->GetResolution();

    if (m_Thunder)
    {

        if(m_ThunderCnt % 2 == 0)
            Rectangle(hDC, -1, -1, rs.width + 1, rs.height + 1);

        m_ThunderCnt++;

        if (m_ThunderCnt == 10)
            m_Thunder = false;
    }

    if (m_BlackScreen)
    {
        RECT rect;
        
        rect.left = m_LT.x;
        rect.top = m_LT.y;
        rect.right = m_RB.x;
        rect.bottom = m_RB.y;

        FillRect(hDC, &rect, CGameManager::GetInst()->GetBlackBrush());
    }

    for (size_t i = 0; i < m_FinishTimes.size(); i++)
    {
        m_FinishTimes[i]->SetPos(rs.width * 35.0f / 100.0f, rs.height * (20.0f + (i * 5.0f)) / 100.0f);
    }

    for (int i = 0; i < m_WidgetCount; i++)
    {
        m_WidgetArray[i]->Render(hDC);
    }
}
