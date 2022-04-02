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
			// m_Select������ ���� ������ �������� �ε����� �ְų�
			// ���� ����Ʈ�ڽ��� ���Ӱ� �ٸ� �������� ������ ���
			// ���� �Ǿ����� ��

			m_Select = m_SelectIndex == i;

			if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), m_Select))
			{
				// ���� ���Ӱ� ���õ� �������� ���� ���õ� �����۰� �ٸ� ��� �ٲ� �������� �ݹ� �Լ��� ȣ���Ѵ�.
				if (m_SelectIndex != i && m_SelectCallback)
					m_SelectCallback((int)i, m_vecItem[i].c_str());

				// �׸��� �ε��� ����
				m_SelectIndex = (int)i;
			}

			if (m_Select)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}
}
