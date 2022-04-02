#include "Viewport.h"
#include "../Input.h"

CViewport::CViewport()
{
}

CViewport::~CViewport()
{
}

void CViewport::Start()
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Start();
	}
}

bool CViewport::Init()
{
	return true;
}

void CViewport::Update(float deltaTime)
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Update(deltaTime);
		iter++;
	}
}

void CViewport::PostUpdate(float deltaTime)
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			iter++;
			continue;
		}
		
		(*iter)->PostUpdate(deltaTime);
		iter++;
	}
}

bool CViewport::CollisionMouse()
{
	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	// 출력 우선순위가 높은 위젯부터 충돌처리를 시작한다.
	if (m_WindowList.size() >= 2)
		m_WindowList.sort(CViewport::SortWindow);

	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->IsEnable())
			continue;

		if ((*iter)->CollisionMouse(mousePos))
			return true;
	}

	iter = m_WindowList.begin();
	iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}

		iter++;
	}

	return false;
}

void CViewport::Collision()
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->IsEnable())
			continue;
	}

	iter = m_WindowList.begin();
	iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}

		iter++;
	}
}

void CViewport::Render()
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Render();
		iter++;
	}
}

bool CViewport::SortWindow(const CSharedPtr<CWidgetWindow> src, const CSharedPtr<CWidgetWindow> dest)
{
	return src->GetZOrder() > dest->GetZOrder();
}
