#include "Button.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"
#include "../Input.h"

CButton::CButton()
{
    m_ButtonState = EButton_State::Normal;
}

CButton::CButton(const CButton& Widget) :
    CUIWidget(Widget)
{
    memcpy(m_FrameData, Widget.m_FrameData, sizeof(AnimationFrameData) * (int)EButton_State::End);
    m_ButtonSound[0] = Widget.m_ButtonSound[0];
    m_ButtonSound[1] = Widget.m_ButtonSound[1];
}

CButton::~CButton()
{
}

void CButton::SetTexture(const std::string& Name)
{
    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();

        for (int i = 0; i < (int)EButton_State::End; i++)
        {
            m_FrameData[i].Size = m_Size;
        }
    }
}

void CButton::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
    m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();

        for (int i = 0; i < (int)EButton_State::End; i++)
        {
            m_FrameData[i].Size = m_Size;
        }
    }
}

void CButton::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
    m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();

        for (int i = 0; i < (int)EButton_State::End; i++)
        {
            m_FrameData[i].Size = m_Size;
        }
    }
}

void CButton::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
    m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();

        for (int i = 0; i < (int)EButton_State::End; i++)
        {
            m_FrameData[i].Size.x = (float)m_Texture->GetWidth(i);
            m_FrameData[i].Size.y = (float)m_Texture->GetHeight(i);
        }
    }
}

void CButton::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
    if (m_Texture)
        m_Texture->SetColorKey(r, g, b, Index);
}

void CButton::SetFrameData(EButton_State State, const Vector2& Start, const Vector2& Size)
{
    m_FrameData[(int)State].StartPos = Start;
    m_FrameData[(int)State].Size = Size;

    m_Size = Size;
}

void CButton::SetMouseOnSound(const std::string& Name)
{
    m_ButtonSound[0] = m_Scene->GetSceneResource()->FindSound(Name);
}

void CButton::SetClickSound(const std::string& Name)
{
    m_ButtonSound[1] = m_Scene->GetSceneResource()->FindSound(Name);
}

bool CButton::Init()
{
    return true;
}

void CButton::Update(float DeltaTime)
{
    if (m_ButtonState != EButton_State::Disable)
    {
        // ���� ��ư�� ��Ȱ���� �ƴϰ� ���� ���콺�� �ö� �ִٸ�
        if (m_MouseHoverd)
        {
            // ���콺�� �����ٸ� Ŭ�� ����
            if (CInput::GetInst()->GetMouseDown())
            {
                m_ButtonState = EButton_State::Click;
            }
            // ���� ���°� Ŭ�������̰� ���콺 ��ư�� ��� ����
            else if (m_ButtonState == EButton_State::Click && CInput::GetInst()->GetMouseUp())
            {
                // ���� ���� �ݹ� �Լ��� �ִٸ� ȣ���Ѵ�.
                if (m_ButtonClickCallback)
                    m_ButtonClickCallback();

                if (m_ButtonSound[1])
                    m_ButtonSound[1]->Play();

                // �� ���� ���콺 �� ���·� �ٲ۴�.
                m_ButtonState = EButton_State::MouseOn;
            }
            // ��� ���콺�� �������ִٸ�
            else if (m_ButtonState == EButton_State::Click &&
                CInput::GetInst()->GetMousePush())
            {
                // Ŭ�����¸� �����Ѵ�.
                m_ButtonState = EButton_State::Click;
            }
            else
            {
                m_ButtonState = EButton_State::MouseOn;

                if (m_ButtonMouseOnCallback)
                    m_ButtonMouseOnCallback();
            }
        }
        else
            m_ButtonState = EButton_State::Normal;
    }

    m_Size = m_FrameData[(int)m_ButtonState].Size;
}

void CButton::PostUpdate(float DeltaTime)
{
}

void CButton::Collision(float DeltaTime)
{
}

void CButton::Render(HDC hDC)
{
    if (m_Texture)
    {
        Vector2 Pos = m_Pos + m_Owner->GetPos();

        if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
        {
            m_Texture->Render(hDC, Pos, m_FrameData[(int)m_ButtonState].StartPos, m_FrameData[(int)m_ButtonState].Size);
        }
        else
        {
            m_Texture->Render(hDC, Pos, Vector2(0.0f, 0.0f), m_Size, (int)m_ButtonState);
        }
    }
}

void CButton::Render(const Vector2& Pos, HDC hDC)
{
    if (m_Texture)
    {
        if (m_Texture->GetTextureType() == ETexture_Type::Atlas)
        {
            m_Texture->Render(hDC, Pos, m_FrameData[(int)m_ButtonState].StartPos, m_FrameData[(int)m_ButtonState].Size);
        }
        else
        {
            m_Texture->Render(hDC, Pos, Vector2(0.0f, 0.0f), m_Size, (int)m_ButtonState);
        }
    }
}

CButton* CButton::Clone()
{
    return new CButton(*this);
}

void CButton::CollisionMouseHoveredCallback(float DeltaTime)
{
    CUIWidget::CollisionMouseHoveredCallback(DeltaTime);

    if (m_ButtonSound[0])
        m_ButtonSound[0]->Play();
}

void CButton::CollisionMouseReleaseCallback(float DeltaTime)
{
    CUIWidget::CollisionMouseReleaseCallback(DeltaTime);
}
