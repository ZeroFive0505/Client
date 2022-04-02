#include "IMGUIWindow.h"
#include "IMGUIWidget.h"
#include "IMGUIManager.h"

CIMGUIWindow::CIMGUIWindow() :
	m_Open(true),
	m_WindowFlag(ImGuiWindowFlags_NoCollapse),
	m_ModelPopup(false)
{
}

CIMGUIWindow::~CIMGUIWindow()
{
	size_t size = m_vecPopupWidget.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecPopupWidget[i]);
	}

	m_vecPopupWidget.clear();

	size = m_vecWidget.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}

	m_vecWidget.clear();
}


void CIMGUIWindow::ClosePopup()
{
	ImGui::CloseCurrentPopup();

	size_t size = m_vecPopupWidget.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecPopupWidget[i]);
	}

	m_vecPopupWidget.clear();
}

CIMGUIWidget* CIMGUIWindow::FindWidget(const std::string& name)
{
	size_t size = m_vecWidget.size();

	for (size_t i = 0; i < size; i++)
	{
		if (m_vecWidget[i]->m_Name == name)
			return m_vecWidget[i];
		else if (m_vecWidget[i]->IsHidden())
		{
			std::string hiddenName = "##" + name;

			if (m_vecWidget[i]->m_Name == hiddenName)
				return m_vecWidget[i];
		}
	}

	return nullptr;
}

bool CIMGUIWindow::Init()
{
	return true;
}

void CIMGUIWindow::Update(float deltaTime)
{
	if (!m_Open)
		return;

	if (!ImGui::Begin(m_Name.c_str(), &m_Open, m_WindowFlag))
		m_Open = false;

	CIMGUIManager::GetInst()->SetCurrentFont();

	size_t size = m_vecWidget.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecWidget[i]->Render();
	}

	CIMGUIManager::GetInst()->ResetCurrentFont();

	ImGui::End();
}
