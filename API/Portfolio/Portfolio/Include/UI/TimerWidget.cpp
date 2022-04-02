#include "TimerWidget.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CTimerWidget::CTimerWidget() :
    m_Time(0.0f),
    m_Factor(0.0f),
    m_Scale(1.0f, 1.0f),
    m_LeftTop(0.0f, 0.0f),
    m_Digits{},
    m_SingleQuote(nullptr),
    m_DoubleQuote(nullptr),
    m_Colon(nullptr),
    m_Period(nullptr)
{
    m_TimerIdx.resize(6, 0);
}

CTimerWidget::CTimerWidget(const CTimerWidget& widget) :
    CUIWidget(widget)
{
    m_Time = widget.m_Time;
    m_Factor = widget.m_Factor;
    m_Scale = widget.m_Scale;
    m_LeftTop = widget.m_LeftTop;

    for (int i = 0; i < 10; i++)
    {
        m_Digits[i] = widget.m_Digits[i];
    }

    m_TimerIdx = widget.m_TimerIdx;

    m_SingleQuote = widget.m_SingleQuote;
    m_DoubleQuote = widget.m_DoubleQuote;
    m_Colon = widget.m_Colon;
    m_Period = widget.m_Period;
}

CTimerWidget::~CTimerWidget()
{
}

void CTimerWidget::SetWindowSize()
{
    Resolution rs = CGameManager::GetInst()->GetResolution();

    m_Factor = rs.width / BASIC_WIDTH;

    for (int i = 0; i < 9; i++)
    {
        m_Digits[i]->SetScale(m_Scale.x * m_Factor, m_Scale.y * m_Factor);
    }

    m_SingleQuote->SetScale(m_Scale.x * m_Factor, m_Scale.y * m_Factor);
    m_DoubleQuote->SetScale(m_Scale.x * m_Factor, m_Scale.y * m_Factor);
    m_Bar->SetScale(m_Scale.x * m_Factor, m_Scale.y * m_Factor);
    m_Colon->SetScale(m_Scale.x * m_Factor, m_Scale.y * m_Factor);
    m_Period->SetScale(m_Scale.x * m_Factor, m_Scale.y * m_Factor);

    int padding = 6 * (int)m_Factor;
    int size = m_Digits[0]->GetWidth();

    m_LeftTop = Vector2(rs.width * 92.0f / 100.0f, rs.height * 2 / 100.0f);

    int pos = (int)m_LeftTop.x - 8 * (size + padding);
    
    m_LeftTop = Vector2((float)pos, m_LeftTop.y);

    for (int i = 0; i < 8; i++)
    {
        if (i == 2)
        {
            m_QuotePos.push_back(Vector2((float)pos, m_LeftTop.y));
        }
        else if (i == 5)
        {
            m_QuotePos.push_back(Vector2((float)pos, m_LeftTop.y));
        }
        else
        {
            m_TimerPos.push_back(Vector2((float)pos, m_LeftTop.y));
        }

        pos += size + padding;
    }
}

bool CTimerWidget::Init()
{
    m_Scale = Vector2(2.0f, 2.0f);

    for (int i = 0; i < 10; i++)
    {
        std::string name;
        TCHAR filePath[MAX_PATH] = {};

        name = std::to_string(i);
        wsprintf(filePath, TEXT("UI/Timer/%d.bmp"), i);

        m_Scene->GetSceneResoruce()->LoadTexture(name, filePath);

        m_Digits[i] = m_Scene->GetSceneResoruce()->FindTexture(name);

        m_Digits[i]->SetColorKey(255, 0, 255);

        m_Digits[i]->SetScale(m_Scale);
    }

    m_Scene->GetSceneResoruce()->LoadTexture("Single", TEXT("UI/Timer/Single.bmp"));

    m_SingleQuote = m_Scene->GetSceneResoruce()->FindTexture("Single");

    m_SingleQuote->SetColorKey(255, 0, 255);

    m_Scene->GetSceneResoruce()->LoadTexture("Double", TEXT("UI/Timer/Double.bmp"));

    m_DoubleQuote = m_Scene->GetSceneResoruce()->FindTexture("Double");

    m_DoubleQuote->SetColorKey(255, 0, 255);

    m_Scene->GetSceneResoruce()->LoadTexture("Bar", TEXT("UI/Timer/Bar.bmp"));

    m_Bar = m_Scene->GetSceneResoruce()->FindTexture("Bar");

    m_Bar->SetColorKey(255, 0, 255);

    m_Scene->GetSceneResoruce()->LoadTexture("Colon", TEXT("UI/Timer/Colon.bmp"));

    m_Colon = m_Scene->GetSceneResoruce()->FindTexture("Colon");

    m_Colon->SetColorKey(255, 0, 255);

    m_Scene->GetSceneResoruce()->LoadTexture("Period", TEXT("UI/Timer/Period.bmp"));

    m_Period = m_Scene->GetSceneResoruce()->FindTexture("Period");

    m_Period->SetColorKey(255, 0, 255);


    for (int i = 0; i < 6; i++)
    {
        m_TimerIdx[i] = 0;
    }

    return true;
}

void CTimerWidget::Update(float deltaTime)
{
    m_Time += deltaTime;

    unsigned int elapsedTimeAsMilli = unsigned int(m_Time * 1000);

    int minutes = elapsedTimeAsMilli / 60000;
    elapsedTimeAsMilli -= minutes * 60000;
    int seconds = elapsedTimeAsMilli / 1000;
    elapsedTimeAsMilli -= seconds * 1000;
    int milliSec = elapsedTimeAsMilli / 10;

    m_TimerIdx[0] = minutes / 10;
    m_TimerIdx[1] = minutes % 10;

    m_TimerIdx[2] = seconds / 10;
    m_TimerIdx[3] = seconds % 10;

    m_TimerIdx[4] = milliSec / 10;
    m_TimerIdx[5] = milliSec % 10;
}

void CTimerWidget::PostUpdate(float deltaTime)
{
}

void CTimerWidget::Render(HDC hDC)
{
    for (size_t i = 0; i < m_TimerIdx.size(); i++)
    {
        m_Digits[m_TimerIdx[i]]->Render(hDC, m_TimerPos[i], Vector2(0.0f, 0.0f),
            Vector2((float)m_Digits[m_TimerIdx[i]]->GetWidth(), (float)m_Digits[m_TimerIdx[i]]->GetHeight()));
    }

    m_SingleQuote->Render(hDC, m_QuotePos[0], Vector2(0.0f, 0.0f), Vector2((float)m_SingleQuote->GetWidth(), (float)m_SingleQuote->GetHeight()));
    m_DoubleQuote->Render(hDC, m_QuotePos[1], Vector2(0.0f, 0.0f), Vector2((float)m_DoubleQuote->GetWidth(), (float)m_DoubleQuote->GetHeight()));
}

void CTimerWidget::Render(const Vector2& pos, HDC hDC)
{
    for (size_t i = 0; i < m_TimerIdx.size(); i++)
    {
        m_Digits[m_TimerIdx[i]]->Render(hDC, m_TimerPos[i], Vector2(0.0f, 0.0f),
            Vector2((float)m_Digits[m_TimerIdx[i]]->GetWidth(), (float)m_Digits[m_TimerIdx[i]]->GetHeight()));
    }

    m_SingleQuote->Render(hDC, m_QuotePos[0], Vector2(0.0f, 0.0f), Vector2((float)m_SingleQuote->GetWidth(), (float)m_SingleQuote->GetHeight()));
    m_DoubleQuote->Render(hDC, m_QuotePos[1], Vector2(0.0f, 0.0f), Vector2((float)m_DoubleQuote->GetWidth(), (float)m_DoubleQuote->GetHeight()));
}

CTimerWidget* CTimerWidget::Clone()
{
    return new CTimerWidget(*this);
}
