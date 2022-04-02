#include "Text.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"
#include "../Device.h"

CText::CText() :
	m_TextCount(0),
	m_TextCapacity(32),
	m_Font(nullptr),
	m_Layout(nullptr),
	m_ColorBrush(nullptr),
	m_2DTarget(nullptr),
	m_FontSize(20.f),
	m_Alpha(true),
	m_Opacity(1.f),
	m_ShadowEnable(false),
	m_ShadowColorBrush(nullptr),
	m_ShadowAlpha(true),
	m_ShadowOpacity(1.f),
	m_ShadowOffset(1.f, 1.f),
	m_AlignH(TEXT_ALIGN_H::Left),
	m_AlignV(TEXT_ALIGN_V::Middle)
{
	m_MouseCollisionEnable = false;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	m_FontName = new TCHAR[128];

	memset(m_FontName, 0, sizeof(TCHAR) * 128);

	lstrcpy(m_Text, TEXT("TEXT"));

	m_TextCount = 4;

	m_Color.w = 1.0f;
	m_ShadowColor.w = 1.0f;
}

CText::CText(const CText& widget) :
	CWidget(widget)
{
	*this = widget;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	if (m_TextCount > 0)
		memcpy(m_Text, widget.m_Text, sizeof(TCHAR) * m_TextCount);

	m_FontName = new TCHAR[128];

	memset(m_FontName, 0, sizeof(TCHAR) * 128);

	lstrcpy(m_FontName, widget.m_FontName);

	m_Layout = nullptr;

	CreateTextLayout();
}

CText::~CText()
{
	SAFE_RELEASE(m_Layout);
	SAFE_DELETE_ARRAY(m_FontName);
	SAFE_DELETE_ARRAY(m_Text);
}

void CText::SetFont(const std::string& name)
{
	m_FontKey = name;
	m_Font = CResourceManager::GetInst()->FindFont(m_FontKey);

	CreateTextLayout();
}

void CText::SetFontSize(float size)
{
	m_FontSize = size;

	CreateTextLayout();
}

void CText::SetAlignH(TEXT_ALIGN_H align)
{
	m_AlignH = align;

	CreateTextLayout();
}

void CText::SetAlignV(TEXT_ALIGN_V align)
{
	m_AlignV = align;

	CreateTextLayout();
}

void CText::SetColor(float r, float g, float b)
{
	m_Color.x = r;
	m_Color.y = g;
	m_Color.z = b;

	// 색을 변경할때는 레이아웃 재설정이 필요없다.

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CText::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_Color.x = r / 255.0f;
	m_Color.y = g / 255.0f;
	m_Color.z = b / 255.0f;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CText::SetColor(const Vector4& color)
{
	m_Color.x = color.x;
	m_Color.y = color.y;
	m_Color.z = color.z;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CText::SetColor(unsigned int color)
{
	m_Color.x = ((color >> 16) & 0x000000ff) / 255.0f;
	m_Color.y = ((color >> 8) & 0x000000ff) / 255.0f;
	m_Color.z = (color & 0x000000ff) / 255.0f;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CText::SetAlphaEnable(bool alphaEnable)
{
	m_Alpha = alphaEnable;
}

void CText::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void CText::SetShadowEnable(bool shadowEnable)
{
	m_ShadowEnable = shadowEnable;
}

void CText::SetShadowOffset(const Vector2& offset)
{
	m_ShadowOffset = offset;
}

void CText::SetShadowOffset(float x, float y)
{
	m_ShadowOffset = Vector2(x, y);
}

void CText::SetShadowColor(float r, float g, float b)
{
	m_ShadowColor.x = r;
	m_ShadowColor.y = g;
	m_ShadowColor.z = b;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CText::SetShadowColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_ShadowColor.x = r / 255.0f;
	m_ShadowColor.y = g / 255.0f;
	m_ShadowColor.z = b / 255.0f;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CText::SetShadowColor(const Vector4& color)
{
	m_ShadowColor.x = color.x;
	m_ShadowColor.y = color.y;
	m_ShadowColor.z = color.z;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CText::SetShadowColor(unsigned int color)
{
	m_ShadowColor.x = ((color >> 24) & 0x000000ff) / 255.0f;
	m_ShadowColor.y = ((color >> 16) & 0x000000ff) / 255.0f;
	m_ShadowColor.z = (color & 0x000000ff) / 255.0f;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

void CText::SetShadowAlphaEnable(bool alphaEnable)
{
	m_ShadowAlpha = alphaEnable;
}

void CText::SetShadowOpacity(float opacity)
{
	m_ShadowOpacity = opacity;
}

bool CText::CreateTextLayout()
{
	if (!m_Font)
		return false;

	SAFE_RELEASE(m_Layout);

	m_Layout = CResourceManager::GetInst()->CreateTextLayout(m_Text, m_Font, m_Size.x, m_Size.y);

	DWRITE_TEXT_RANGE range = {};
	range.startPosition = 0;
	range.length = lstrlen(m_Text);

	m_Layout->SetFontSize(m_FontSize, range);

	switch (m_AlignH)
	{
	case TEXT_ALIGN_H::Left:
		m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TEXT_ALIGN_H::Center:
		m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_H::Right:
		m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	switch (m_AlignV)
	{
	case TEXT_ALIGN_V::Top:
		m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TEXT_ALIGN_V::Middle:
		m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_V::Bottom:
		m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}

	return true;
}

void CText::Start()
{
	CWidget::Start();
}

bool CText::Init()
{
	if (!CWidget::Init())
		return false;

	m_2DTarget = CDevice::GetInst()->Get2DRenderTarget();

	const TCHAR* fontName = CResourceManager::GetInst()->GetFontFaceName("Default");

	lstrcpy(m_FontName, fontName);

	m_FontKey = "Default";

	m_Font = CResourceManager::GetInst()->FindFont(m_FontKey);

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);

	CreateTextLayout();

	return true;
}

void CText::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CText::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CText::Render()
{
	m_2DTarget->BeginDraw();

	sResolution rs = CDevice::GetInst()->GetResolution();

	D2D1_POINT_2F point;

	point.x = m_RenderPos.x;
	point.y = rs.height - m_RenderPos.y - m_Size.y;

	if (m_ShadowEnable)
	{
		D2D1_POINT_2F shadowPoint = point;
		shadowPoint.x += m_ShadowOffset.x;
		shadowPoint.y += m_ShadowOffset.y;

		if (m_ShadowAlpha)
			m_ShadowColorBrush->SetOpacity(m_ShadowOpacity);
		else
			m_ShadowColorBrush->SetOpacity(1.0f);

		m_2DTarget->DrawTextLayout(shadowPoint, m_Layout, m_ShadowColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	if (m_Alpha)
		m_ColorBrush->SetOpacity(m_Opacity);
	else
		m_ColorBrush->SetOpacity(1.0f);


	m_2DTarget->DrawTextLayout(point, m_Layout, m_ColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);

	m_2DTarget->EndDraw();
}
