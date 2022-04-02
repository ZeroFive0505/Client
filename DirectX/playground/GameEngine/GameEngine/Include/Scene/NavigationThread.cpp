#include "NavigationThread.h"
#include "Navigation.h"
#include "NavigationManager.h"

CNavigationThread::CNavigationThread()
{
	SetLoop(true);

	m_Navigation = new CNavigation;

	m_ExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CNavigationThread::~CNavigationThread()
{
	m_Loop = false;

	WaitForSingleObject(m_ExitEvent, INFINITE);

	CloseHandle(m_ExitEvent);

	SAFE_DELETE(m_Navigation);
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* tileMap)
{
	m_Navigation->CreateNavigationNode(tileMap);
}

void CNavigationThread::Run()
{
	while (m_Loop)
	{
		if (!m_WorkQueue.empty())
		{
			sNavWorkData work = m_WorkQueue.front();
			m_WorkQueue.pop();

			sNavResultData result;

			m_Navigation->FindPath(work.start, work.end, result.path);

			result.callBack = work.callBack;

			m_NavManager->AddNavResult(result);
		}
	}

	SetEvent(m_ExitEvent);
}
