#include "ItemWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "UIWindow.h"


CItemWidget::CItemWidget() :
    m_ItemList{},
    m_SeletedItem(0),
    m_FinalItem(0),
    m_Time(0.0f),
    m_Spining(false),
    m_ForceStop(false)
{
}

CItemWidget::CItemWidget(const CItemWidget& widget)
{
    for (int i = 0; i < 8; i++)
    {
        m_ItemList[i] = widget.m_ItemList[i];
    }

    m_SeletedItem = widget.m_SeletedItem;
    m_FinalItem = widget.m_FinalItem;
}

CItemWidget::~CItemWidget()
{
}

void CItemWidget::SetItem(EItem item)
{
    if (item != EItem::NONE)
    {
        m_Spining = true;
        m_ForceStop = false;
        m_SeletedItem = 0;
        m_FinalItem = (int)item % 9;
    }
    else
    {
        m_SeletedItem = 0;
        m_FinalItem = 0;
        m_CurrentItem = m_ItemList[m_FinalItem];
    }
}

bool CItemWidget::Init()
{
    m_Scale = Vector2(2.0f, 2.0f);

    m_Scene->GetSceneResoruce()->LoadTexture("Empty UI", TEXT("UI/Item/Empty.bmp"));

    m_ItemList[0] = m_Scene->GetSceneResoruce()->FindTexture("Empty UI");

    m_Scene->GetSceneResoruce()->LoadTexture("Mushroom UI", TEXT("UI/Item/Mushroom.bmp"));

    m_ItemList[1] = m_Scene->GetSceneResoruce()->FindTexture("Mushroom UI");

    m_Scene->GetSceneResoruce()->LoadTexture("Coin UI", TEXT("UI/Item/Coin.bmp"));

    m_ItemList[2] = m_Scene->GetSceneResoruce()->FindTexture("Coin UI");

    m_Scene->GetSceneResoruce()->LoadTexture("Star UI", TEXT("UI/Item/Star.bmp"));

    m_ItemList[3] = m_Scene->GetSceneResoruce()->FindTexture("Star UI");

    m_Scene->GetSceneResoruce()->LoadTexture("Banana UI", TEXT("UI/Item/Banana.bmp"));

    m_ItemList[4] = m_Scene->GetSceneResoruce()->FindTexture("Banana UI");

    m_Scene->GetSceneResoruce()->LoadTexture("GreenShell UI", TEXT("UI/Item/GreenShell.bmp"));

    m_ItemList[5] = m_Scene->GetSceneResoruce()->FindTexture("GreenShell UI");

    m_Scene->GetSceneResoruce()->LoadTexture("RedShell UI", TEXT("UI/Item/RedShell.bmp"));

    m_ItemList[6] = m_Scene->GetSceneResoruce()->FindTexture("RedShell UI");

    m_Scene->GetSceneResoruce()->LoadTexture("Thunder UI", TEXT("UI/Item/Thunder.bmp"));

    m_ItemList[7] = m_Scene->GetSceneResoruce()->FindTexture("Thunder UI");

    m_Scene->GetSceneResoruce()->LoadTexture("Feather UI", TEXT("UI/Item/Feather.bmp"));

    m_ItemList[8] = m_Scene->GetSceneResoruce()->FindTexture("Feather UI");


    Resolution rs = CGameManager::GetInst()->GetResolution();

    float xFactor = rs.width / BASIC_WIDTH;


    for (int i = 0; i < 9; i++)
    {
        m_ItemList[i]->SetScale(m_Scale * xFactor);
        m_ItemList[i]->SetColorKey(255, 0, 255);
        m_ItemList[i]->SetPivot(m_ItemList[i]->GetWidth() / 2.0f, m_ItemList[i]->GetHeight() / 2.0f);
        m_ItemList[i]->SetOffset(0.0f, 0.0f);
    }

    m_CurrentItem = m_ItemList[0];

    
    return true;
}

void CItemWidget::Update(float deltaTime)
{
    if (m_Spining)
    {
        m_Time += deltaTime;

        m_SeletedItem = ((m_SeletedItem + 1) % 8);
        m_CurrentItem = m_ItemList[m_SeletedItem];
        if (m_Time > 2.0f || (m_Time >= 1.0f && m_ForceStop))
        {
            m_Spining = false;
            m_ForceStop = false;
            m_Scene->GetSceneResoruce()->SoundStop("ItemReel");
            m_Scene->GetSceneResoruce()->SoundPlay("GotItem");
            m_CurrentItem = m_ItemList[m_FinalItem];
            m_FinalItem = 0;
            m_SeletedItem = 0;
            m_Time = 0.0f;
        }
    }
}

void CItemWidget::PostUpdate(float deltaTime)
{
}

void CItemWidget::Collision(float deltaTime)
{
}

void CItemWidget::Render(HDC hDC)
{
    if (m_CurrentItem)
    {
        Vector2 pos = m_Pos + m_Owner->GetPos();

        m_ItemList[0]->Render(hDC, pos, Vector2(0.0f, 0.0f), Vector2((float)m_ItemList[0]->GetWidth(), (float)m_ItemList[0]->GetHeight()));
        m_CurrentItem->Render(hDC, pos, Vector2(0.0f, 0.0f), Vector2((float)m_CurrentItem->GetWidth(), (float)m_CurrentItem->GetHeight()));
    }
}

void CItemWidget::Render(const Vector2& pos, HDC hDC)
{
    if (m_CurrentItem)
    {
        m_ItemList[0]->Render(hDC, pos, Vector2(0.0f, 0.0f), Vector2((float)m_ItemList[0]->GetWidth(), (float)m_ItemList[0]->GetHeight()));
        m_CurrentItem->Render(hDC, pos, Vector2(0.0f, 0.0f), Vector2((float)m_CurrentItem->GetWidth(), (float)m_CurrentItem->GetHeight()));
    }
}

CItemWidget* CItemWidget::Clone()
{
    return new CItemWidget(*this);
}
