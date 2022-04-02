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

    // 버튼의 모든 상태에 같은 색을 지정해서 차이를 없게 만든다.
    ImGui::PushStyleColor(ImGuiCol_Button, m_Color.Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Color.Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_Color.Value);

    ImGui::Button(m_Name.c_str(), m_Size);

    // 색깔은 3번 푸시했으니 3번 빼낸다.
    ImGui::PopStyleColor(3);
    // 스타일 변수도 빼낸다.
    ImGui::PopStyleVar();
    
}
