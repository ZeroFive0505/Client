#include "NavigationManager.h"

CNavigationManager::CNavigationManager()
{
}

CNavigationManager::~CNavigationManager()
{
    size_t size = m_vecNavigationThread.size();

    for (size_t i = 0; i < size; i++)
    {
        SAFE_DELETE(m_vecNavigationThread[i]);
    }

    m_vecNavigationThread.clear();
}

void CNavigationManager::SetNavData(CTileMapComponent* navData)
{
    m_NavData = navData;

    if (!m_vecNavigationThread.empty())
    {
        size_t size = m_vecNavigationThread.size();

        for (size_t i = 0; i < size; i++)
        {
            SAFE_DELETE(m_vecNavigationThread[i]);
        }

        m_vecNavigationThread.clear();
    }

    if (m_NavData)
    {
        for (int i = 0; i < 4; i++)
        {
            char name[256] = {};
            sprintf_s(name, "Thread %d", i + 1);
            CNavigationThread* thread = CThread::CreateThread<CNavigationThread>(name);

            thread->m_NavManager = this;
            thread->CreateNavigationNode(m_NavData);

            thread->Start();

            m_vecNavigationThread.push_back(thread);
        }
    }
}

void CNavigationManager::AddNavResult(const sNavResultData& navData)
{
    m_ResultQueue.push(navData);
}

void CNavigationManager::Start()
{
}

bool CNavigationManager::Init()
{
    return true;
}

void CNavigationManager::Update(float deltaTime)
{
    if (!m_ResultQueue.empty())
    {
        sNavResultData result = m_ResultQueue.front();
        m_ResultQueue.pop();

        result.callBack(result.path);
    }
}
