#include "CoinWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CCoinWidget::CCoinWidget() :
    m_CoinTexture{}
{
    m_TextCapacity = 32;
    m_Coins = 0;
    m_TextCount = 0;
    m_Text = new TCHAR[m_TextCapacity];
    m_CurrentFrame = 0;
    m_CurrentTime = 0.0f;

    memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

    m_TextColor = RGB(0, 0, 0);
}

CCoinWidget::CCoinWidget(const CCoinWidget& widget) :
    CUIWidget(widget)
{
    m_TextCapacity = widget.m_TextCapacity;
    m_TextCount = widget.m_TextCount;
    m_TextColor = widget.m_TextColor;

    m_Text = new TCHAR[m_TextCapacity];

    memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

    memcpy(m_Text, widget.m_Text, sizeof(TCHAR) * m_TextCount);

    m_Font = widget.m_Font;
}

CCoinWidget::~CCoinWidget()
{
    SAFE_DELETE_ARRAY(m_Text);
}

void CCoinWidget::SetFont(const std::string& name)
{
    m_Font = CResourceManager::GetInst()->FindFont(name);
}

void CCoinWidget::AddCoin(int amount)
{
    m_Coins += amount;

    if (m_Coins > 10)
        m_Coins = 10;
    else if (m_Coins < 0)
        m_Coins = 0;

    std::wstring s;
    std::string temp;
    int coins = m_Coins;

    for (int i = 0; i < 2; i++)
    {
        int r = coins % 10;
        coins /= 10;
        temp.push_back(r + '0');
    }

    temp.push_back(' ');
    temp.push_back('X');

    s.assign(temp.rbegin(), temp.rend());

    m_SpriteText.clear();

    int index = 1;
    for (size_t i = 0; i < temp.size(); i++)
    {
        std::string name;
        switch (temp[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            name = "Small_";
            name += temp[i];
            m_SpriteText.push_back(std::make_pair(name, Vector2(13.0f * index, 0.0f)));
            index++;
            break;
        case ' ':
            name = "Space";
            m_SpriteText.push_back(std::make_pair(name, Vector2(13.0f * index, 0.0f)));
            index++;
            break;
        default:
            name = temp[i];
            m_SpriteText.push_back(std::make_pair(name, Vector2(42.0f, 0.0f)));
            break;
        }

    }
}

bool CCoinWidget::Init()
{
    m_Font = CResourceManager::GetInst()->FindFont("DefaultFont");

    for (int i = 0; i < 3; i++)
    {
        std::string name;
        TCHAR filePath[MAX_PATH] = {};
        name = "Coin_" + std::to_string(i + 1);
        wsprintf(filePath, TEXT("UI/Effect/Coin_%d.bmp"), i + 1);
        m_Scene->GetSceneResoruce()->LoadTexture(name, filePath);

        m_CoinTexture[i] = m_Scene->GetSceneResoruce()->FindTexture(name);
        m_CoinTexture[i]->SetColorKey(255, 0, 255);
    }

    m_TextColor = RGB(255, 255, 255);

    m_SpriteText.push_back(std::make_pair("X", Vector2(42.0f, 0.0f)));
    m_SpriteText.push_back(std::make_pair("Space", Vector2(39.0f, 0.0f)));
    m_SpriteText.push_back(std::make_pair("Small_0", Vector2(26.0f, 0.0f)));
    m_SpriteText.push_back(std::make_pair("Small_0", Vector2(13.0f, 0.0f)));

    return true;
}

void CCoinWidget::Update(float deltaTime)
{
    m_CurrentTime += deltaTime;

    if (m_CurrentTime >= COINWIDGET_BETWEEN_FRAME)
    {
        m_CurrentTime = 0.0f;
        m_CurrentFrame = (m_CurrentFrame + 1) % 3;
    }
}

void CCoinWidget::PostUpdate(float deltaTime)
{
}

void CCoinWidget::Render(HDC hDC)
{
    CSharedPtr<CTexture> temp = m_CoinTexture[m_CurrentFrame]->Clone();
    temp->SetColorKey(255, 0, 255);
    temp->Render(hDC, m_Pos, Vector2(0.0f, 0.0f),
        Vector2((float)temp->GetWidth(), temp->GetHeight()));

    Vector2 offset = m_Pos + Vector2(60.0f, 0.0f);

    for (int i = m_SpriteText.size() - 1; i >= 0; i--)
    {
        temp = m_Scene->GetSceneResoruce()->FindSpriteFont(m_SpriteText[i].first);
        temp->Render(hDC, offset - m_SpriteText[i].second, Vector2(0.0f, 0.0f), Vector2(9.0f, 9.0f));
    }
}

void CCoinWidget::Render(const Vector2& pos, HDC hDC)
{
}

CCoinWidget* CCoinWidget::Clone()
{
    return new CCoinWidget(*this);
}
