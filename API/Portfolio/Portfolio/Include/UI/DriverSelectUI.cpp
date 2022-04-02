#include "DriverSelectUI.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "UIImage.h"

CDriverSelectUI::CDriverSelectUI() :
    m_BackGround(nullptr),
    m_Confirm(nullptr),
    m_TitleBG(nullptr),
    m_Title(nullptr)
{
    for (int i = 0; i < 8; i++)
    {
        m_DriverWidgets[i] = nullptr;
    }
}

CDriverSelectUI::~CDriverSelectUI()
{
}

bool CDriverSelectUI::Init()
{
    m_Pos = Vector2(0.0f, 0.0f);

    m_BackGround = CreateWidget<CUIImage>("BackGround");

    m_BackGround->SetTexture("DriverBackGround", TEXT("UI/DriverSelect/select_window.bmp"));

    m_BackGround->SetPos(0.0f, 0.0f);

    m_BackGround->SetTextureColorKey(255, 0, 255);

    m_BackGround->SetZOrder(1);

    m_Confirm = CreateWidget<CUIImage>("Confirm");

    m_Confirm->SetTexture("1P Confirm", TEXT("UI/DriverSelect/confirm.bmp"));

    m_Confirm->SetPos(383.0f, 381.0f);

    m_Confirm->SetTextureColorKey(255, 0, 255);

    m_Confirm->SetScale(2.0f, 2.0f);

    m_Confirm->SetZOrder(0);

    m_TitleBG = CreateWidget<CUIImage>("Select scene title bg");

    m_TitleBG->SetTexture("Select scene title bg", TEXT("UI/DriverSelect/title_bg.bmp"));

    m_TitleBG->SetPos(155.0f, 49.0f);

    m_TitleBG->SetTextureColorKey(255, 0, 255);

    m_TitleBG->SetScale(3.0f, 3.0f);

    m_TitleBG->SetZOrder(2);

    m_Title = CreateWidget<CUIImage>("Select scene title");

    m_Title->SetTexture("Select scene title", TEXT("UI/DriverSelect/title.bmp"));

    m_Title->SetPos(400.0f, 49.0f);
    
    m_Title->SetTextureColorKey(255, 0, 255);

    m_Title->SetScale(2.0f, 2.0f);

    m_Title->SetZOrder(1);
    
    m_DriverWidgets[0] = CreateWidget<CDriverWidget>("Mario");

    m_DriverWidgets[0]->AnimatorInit("Mario");

    m_DriverWidgets[0]->SetPos(73.0f, 155.0f);

    m_DriverWidgets[0]->SetZOrder(0);

    m_DriverWidgets[1] = CreateWidget<CDriverWidget>("Peach");

    m_DriverWidgets[1]->AnimatorInit("Peach");

    m_DriverWidgets[1]->SetPos(169.0f, 155.0f);

    m_DriverWidgets[1]->SetZOrder(0);

    m_DriverWidgets[2] = CreateWidget<CDriverWidget>("Bowser");

    m_DriverWidgets[2]->AnimatorInit("Bowser");

    m_DriverWidgets[2]->SetPos(264.0f, 155.0f);

    m_DriverWidgets[2]->SetZOrder(0);

    m_DriverWidgets[3] = CreateWidget<CDriverWidget>("Koopa");

    m_DriverWidgets[3]->AnimatorInit("Koopa");

    m_DriverWidgets[3]->SetPos(361.0f, 155.0f);

    m_DriverWidgets[3]->SetZOrder(0);

    m_DriverWidgets[4] = CreateWidget<CDriverWidget>("Luigi");

    m_DriverWidgets[4]->AnimatorInit("Luigi");

    m_DriverWidgets[4]->SetPos(71.0f, 283.0f);

    m_DriverWidgets[4]->SetZOrder(0);

    m_DriverWidgets[5] = CreateWidget<CDriverWidget>("Yoshi");

    m_DriverWidgets[5]->AnimatorInit("Yoshi");

    m_DriverWidgets[5]->SetPos(169.0f, 283.0f);

    m_DriverWidgets[5]->SetZOrder(0);

    m_DriverWidgets[6] = CreateWidget<CDriverWidget>("DK");

    m_DriverWidgets[6]->AnimatorInit("DK");

    m_DriverWidgets[6]->SetPos(264.0f, 283.0f);

    m_DriverWidgets[6]->SetZOrder(0);

    m_DriverWidgets[7] = CreateWidget<CDriverWidget>("Toad");

    m_DriverWidgets[7]->AnimatorInit("Toad");

    m_DriverWidgets[7]->SetPos(361.0f, 283.0f);

    m_DriverWidgets[7]->SetZOrder(0);

    m_Confirm->SetVisibility(false);
    
    SortWidget();

    return true;
}

void CDriverSelectUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);

    m_Title->Move(-50.0f * deltaTime, 0.0f);

    Vector2 LT = m_Title->GetPos();
    Vector2 RB = LT + (Vector2(m_Title->GetSize().x, m_Title->GetSize().y) * m_Title->GetScale());

    if (RB.x < 87.0f)
    {
        m_Title->SetPos(400.0f, 49.0f);
    }
}

void CDriverSelectUI::Select(int select)
{
    if (select < 0 || select > 7)
        return;

    m_Select = select;
    m_Confirm->SetVisibility(true);


	m_DriverWidgets[m_Select]->Selected();
   
}

void CDriverSelectUI::Cancel()
{
    if (m_DriverWidgets[m_Select]->HasBeenSelected())
    {
        m_Confirm->SetVisibility(false);
        m_DriverWidgets[m_Select]->Cancel();
    }
}
