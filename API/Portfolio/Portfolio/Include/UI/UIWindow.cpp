
#include "UIWindow.h"
#include "UIWidget.h"

CUIWindow::CUIWindow() :
	m_Visibility(true),
	m_WidgetArray(nullptr),
	m_Scene(nullptr),
	m_ZOrder(0),
	m_WidgetCount(0),
	m_WidgetCapacity(4)
{
	m_WidgetArray = new CUIWidget * [m_WidgetCapacity];
}

CUIWindow::~CUIWindow()
{
	for (int i = 0; i < m_WidgetCount; i++)
	{
		SAFE_RELEASE(m_WidgetArray[i]);
	}

	SAFE_DELETE_ARRAY(m_WidgetArray);
}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Update(float deltaTime)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			m_WidgetCount--;

			continue;
		}
		else if (!m_WidgetArray[i]->GetVisibility())
		{
			i++;
			continue;
		}

		m_WidgetArray[i]->Update(deltaTime);
		i++;
	}
}

void CUIWindow::PostUpdate(float deltaTime)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			m_WidgetCount--;

			continue;
		}
		else if (!m_WidgetArray[i]->GetVisibility())
		{
			i++;
			continue;
		}

		m_WidgetArray[i]->PostUpdate(deltaTime);
		i++;
	}
}

void CUIWindow::Render(HDC hDC)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; j++)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			m_WidgetCount--;

			continue;
		}
		else if (!m_WidgetArray[i]->GetVisibility())
		{
			i++;
			continue;
		}

		m_WidgetArray[i]->Render(hDC);
		i++;
	}
}

int CUIWindow::SortZOrder(const void* src, const void* dest)
{
	CUIWidget* srcObj = *(CUIWidget**)src;
	CUIWidget* destObj = *(CUIWidget**)dest;

	int srcZ = srcObj->GetZOrder();
	int destZ = destObj->GetZOrder();

	if (srcZ > destZ)
		return -1;
	else if (srcZ < destZ)
		return 1;

	return 0;
}
