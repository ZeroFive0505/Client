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

void CUIText::SetFont(const std::string& name)
{
	m_Font = CResourceManager::GetInst()->FindFont(name);
}

bool CUIText::Init()
{
	m_Font = CResourceManager::GetInst()->FindFont("DefaultFont");

	return true;
}

void CUIText::Update(float deltaTime)
{
}

void CUIText::PostUpdate(float deltaTime)
{
}

void CUIText::Render(HDC hDC)
{
	m_Font->SetFont(hDC);

	SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)m_Pos.x, (int)m_Pos.y, m_Text, m_TextCount);


	m_Font->ResetFont(hDC);
}

void CUIText::Render(const Vector2& pos, HDC hDC)
{
	m_Font->SetFont(hDC);

	SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)pos.x, (int)pos.y, m_Text, m_TextCount);

	m_Font->ResetFont(hDC);
}

CUIText* CUIText::Clone()
{
	return new CUIText(*this);
}
