#include "UISpriteText.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture.h"

std::string GetSpriteName(const char c)
{
	std::string name;

	switch (c)
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
		name += c;
		break;
	case ' ':
		name = "Space";
		break;
	case '!':
		name = "Exclamanation";
		break;
	case '?':
		name = "Question";
		break;
	case '\'':
		name = "SmallSingle";
		break;
	case '"':
		name = "SmallDouble";
		break;
	case '-':
		name = "SmallBar";
		break;
	default:
		name = c;
		break;
	}


	return name;
}

CUISpriteText::CUISpriteText()
{
}

CUISpriteText::CUISpriteText(const CUISpriteText& widget)
{
	m_SpriteText = widget.m_SpriteText;
}

CUISpriteText::~CUISpriteText()
{
}

bool CUISpriteText::Init()
{
	return true;
}

void CUISpriteText::Update(float deltaTime)
{
}

void CUISpriteText::PostUpdate(float deltaTime)
{
}

void CUISpriteText::Render(HDC hDC)
{
	for (size_t i = 0; i < m_SpriteText.size(); i++)
	{
		CTexture* text = m_Scene->GetSceneResoruce()->FindSpriteFont(m_SpriteText[i].first);

		text->Render(hDC, m_Pos + m_SpriteText[i].second, Vector2(0.0f, 0.0f), Vector2(9.0f, 9.0f));
	}
}

void CUISpriteText::Render(const Vector2& pos, HDC hDC)
{
}

CUISpriteText* CUISpriteText::Clone()
{
	return new CUISpriteText(*this);
}

void CUISpriteText::SetText(const std::string& text)
{
	for (size_t i = 0; i < text.size(); i++)
	{
		std::string name = GetSpriteName(std::toupper(text[i]));

		m_SpriteText.push_back(std::make_pair(name, Vector2(10.0f * i, 0.0f)));
	}
}
