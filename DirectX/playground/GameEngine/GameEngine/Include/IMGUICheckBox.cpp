#include "IMGUICheckBox.h"

CIMGUICheckBox::CIMGUICheckBox() :
	m_SameLine(false)
{
}

CIMGUICheckBox::~CIMGUICheckBox()
{
}

bool CIMGUICheckBox::Init()
{
	return true;
}

void CIMGUICheckBox::Render()
{
	size_t itemCount = m_vecItemUTF8.size();

	ImGui::Text(m_Name.c_str());

	for (size_t i = 0; i < itemCount; i++)
	{
		bool v = m_vecCheck[i];

		if (ImGui::Checkbox(m_vecItemUTF8[i].c_str(), &v))
		{
			m_vecCheck[i] = v;

			if (m_vecCheck[i] && m_SelectCallbacks[i])
				m_SelectCallbacks[i]();
			else if (!m_vecCheck[i] && m_DeSelectCallbacks[i])
				m_DeSelectCallbacks[i]();

		}

		if (m_SameLine && i < itemCount - 1)
			ImGui::SameLine();
	}
}
