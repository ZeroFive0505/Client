#include "NumberWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CNumberWidget::CNumberWidget() :
    m_Number(0)
{
}

CNumberWidget::CNumberWidget(const CNumberWidget& widget) :
    CUIImage(widget)
{
    m_Number = widget.m_Number;
}

CNumberWidget::~CNumberWidget()
{
}

bool CNumberWidget::Init()
{
    return true;
}

void CNumberWidget::Update(float deltaTime)
{
    int number = m_Number;

    while (number)
    {
        m_vecNumber.push_back(number % 10);
        number /= 10;
    }


    std::reverse(m_vecNumber.begin(), m_vecNumber.end());
}

void CNumberWidget::PostUpdate(float deltaTime)
{
}

void CNumberWidget::Render(HDC hDC)
{
    if (m_vecNumber.empty())
    {
        m_FrameIndex = 0;
        CUIImage::Render(hDC);
    }
    else
    {
        size_t size = m_vecNumber.size();

        for (size_t i = 0; i < size; i++)
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
                m_FrameIndex = m_vecNumber[i];
            else
                m_FrameIndex = 0;

            m_Offset.x = i * m_Size.x;

            CUIImage::Render(hDC);
        }
    }
}

void CNumberWidget::Render(const Vector2& pos, HDC hDC)
{
    if (m_vecNumber.empty())
    {
        m_FrameIndex = 0;
        CUIImage::Render(pos, hDC);
    }
    else
    {
        size_t size = m_vecNumber.size();

        for (size_t i = 0; i < size; i++)
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
                m_FrameIndex = m_vecNumber[i];
            else
                m_FrameIndex = 0;

            m_Offset.x = i * m_Size.x;

            CUIImage::Render(pos, hDC);
        }
    }
}

CNumberWidget* CNumberWidget::Clone()
{
    return new CNumberWidget(*this);
}
