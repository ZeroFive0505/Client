#include "IMGUIRadioButton.h"

CIMGUIRadioButton::CIMGUIRadioButton() :
	m_Mode(0),
	m_SameLine(false)
{
}

CIMGUIRadioButton::~CIMGUIRadioButton()
{
}

bool CIMGUIRadioButton::Init()
{
	return true;
}

void CIMGUIRadioButton::Render()
{
	size_t itemCount = m_vecItemUTF8.size();

	ImGui::Text(m_Name.c_str());

	for (size_t i = 0; i < itemCount; i++)
	{
		if (ImGui::RadioButton(m_vecItemUTF8[i].c_str(), i == m_Mode))
		{
			if (i == m_Mode)
				continue;

			if (m_SelectCallbacks[i])
				m_SelectCallbacks[i]();

			m_Mode = (int)i;
		}

		if (m_SameLine && i < itemCount - 1)
			ImGui::SameLine();
	}
}
