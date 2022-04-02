#include "IMGUILabel.h"

CIMGUILabel::CIMGUILabel() :
    m_Align(0.0f, 0.0f)
{
}

CIMGUILabel::~CIMGUILabel()
{
}

bool CIMGUILabel::Init()
{
    return true;
}

void CIMGUILabel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_Align);

    // ��ư�� ��� ���¿� ���� ���� �����ؼ� ���̸� ���� �����.
    ImGui::PushStyleColor(ImGuiCol_Button, m_Color.Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Color.Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_Color.Value);

    ImGui::Button(m_Name.c_str(), m_Size);

    // ������ 3�� Ǫ�������� 3�� ������.
    ImGui::PopStyleColor(3);
    // ��Ÿ�� ������ ������.
    ImGui::PopStyleVar();
    
}
