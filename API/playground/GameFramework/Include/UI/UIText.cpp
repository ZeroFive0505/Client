#include "UIText.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"
#include "../Resource/ResourceManager.h"

CUIText::CUIText()
{
    m_TextCapacity = 32;
    m_TextCount = 0;

    m_Text = new TCHAR[m_TextCapacity];

    memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

    m_TextColor = RGB(0, 0, 0);
}

CUIText::CUIText(const CUIText& widget) :
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

CUIText::~CUIText()
{
    SAFE_DELETE_ARRAY(m_Text);
}

void CUIText::SetFont(const std::string& Name)
{
    m_Font = CResourceManager::GetInst()->FindFont(Name);
}

bool CUIText::Init()
{
    // 폰트를 가장 기본 폰트로 설정한다.
    m_Font = CResourceManager::GetInst()->FindFont("DefaultFont");

    return true;
}

void CUIText::Update(float DeltaTime)
{
}

void CUIText::PostUpdate(float DeltaTime)
{
}

void CUIText::Collision(float DeltaTime)
{
}

void CUIText::Render(HDC hDC)
{
    m_Font->SetFont(hDC);
    // 텍스트 출력시 생성되는 뒷 배경을 없앤다.
    SetBkMode(hDC, TRANSPARENT);
    // 현재 SetTextColor는 클래스내에 선언이 되어있다. 따라서 전역 함수인 SetTextColorKey를 이용하기 위해 :: 붙인다.
    ::SetTextColor(hDC, m_TextColor);
    TextOut(hDC, (int)m_Pos.x, (int)m_Pos.y, m_Text, m_TextCount);

    m_Font->ResetFont(hDC);
}

void CUIText::Render(const Vector2& Pos, HDC hDC)
{
    m_Font->SetFont(hDC);
    SetBkMode(hDC, TRANSPARENT);
    ::SetTextColor(hDC, m_TextColor);
    TextOut(hDC, (int)Pos.x, (int)Pos.y, m_Text, m_TextCount);

    m_Font->ResetFont(hDC);
}

CUIText* CUIText::Clone()
{
    return new CUIText(*this);
}
