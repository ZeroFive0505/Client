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

void CNumberWidget::Update(float DeltaTime)
{
    // ���ڸ� �ɰ���.
    int Number = m_Number;

    std::stack<int> NumberStack;

    while (Number > 0)
    {
        NumberStack.push(Number % 10);
        Number /= 10;
    }

    m_vecNumber.clear();
    // �ŲٷεǾ��ִ� ���ڸ� �ٽ� ���� ���·� ������.
    while (!NumberStack.empty())
    {
        m_vecNumber.push_back(NumberStack.top());
        NumberStack.pop();
    }
}

void CNumberWidget::PostUpdate(float DeltaTime)
{
}

void CNumberWidget::Collision(float DeltaTime)
{
}

void CNumberWidget::Render(HDC hDC)
{
    if (m_vecNumber.empty())
    {
        // ��������� 0�� ����Ѵ�.
        m_FrameIndex = 0;
        CUIImage::Render(hDC);
    }
    else
    {
        // ������� �ʴٸ� ���� �ִٴ� ��.
        size_t Size = m_vecNumber.size();

        // ��ȸ�ϸ鼭 ����Ѵ�.
        for (size_t i = 0; i < Size; i++)
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
                m_FrameIndex = m_vecNumber[i];
            else
                m_FrameIndex = 0;

            // i�� �þ���� x�� ���� Ŀ���� ���� ���������� �����̴�.
            m_Offset.x = i * m_Size.x;

            CUIImage::Render(hDC);
        }
    }
}

void CNumberWidget::Render(const Vector2& Pos, HDC hDC)
{
    if (m_vecNumber.empty())
    {
        m_FrameIndex = 0;
        CUIImage::Render(Pos, hDC);
    }
    else
    {
        size_t Size = m_vecNumber.size();

        for (size_t i = 0; i < Size; i++)
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
                m_FrameIndex = m_vecNumber[i];
            else
                m_FrameIndex = 0;

            m_Offset.x = i * m_Size.x;

            CUIImage::Render(Pos, hDC);

        }
    }
}

CNumberWidget* CNumberWidget::Clone()
{
    return new CNumberWidget(*this);
}