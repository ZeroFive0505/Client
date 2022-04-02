#include "IMGUIListBox.h"

CIMGUIListBox::CIMGUIListBox() :
	m_Sort(false),
	m_Select(false),
	m_SelectIndex(-1),
	m_PageItemCount(3)
{
}

CIMGUIListBox::~CIMGUIListBox()
{
}

bool CIMGUIListBox::Init()
{
	return true;
}

void CIMGUIListBox::Render()
{
	ImVec2 size = ImVec2(m_Size.x, m_PageItemCount * ImGui::GetTextLineHeightWithSpacing());

	if (ImGui::BeginListBox(m_Name.c_str(), size))
	{
		size_t itemCount = m_vecItemUTF8.size();

		for (size_t i = 0; i < itemCount; i++)
		{
			// m_Select변수는 전에 선택한 아이템의 인덱스가 있거나
			// 만약 리스트박스로 새롭게 다른 아이템을 선택할 경우
			// 참이 되어있을 것

			m_Select = m_SelectIndex == i;

			if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), m_Select))
			{
				// 만약 새롭게 선택된 아이템이 전에 선택된 아이템과 다를 경우 바뀐 아이템의 콜백 함수를 호출한다.
				if (m_SelectIndex != i && m_SelectCallback)
					m_SelectCallback((int)i, m_vecItem[i].c_str());

				// 그리고 인덱스 갱신
				m_SelectIndex = (int)i;
			}

			if (m_Select)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}
}
