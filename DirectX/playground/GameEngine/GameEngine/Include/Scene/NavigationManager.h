#pragma once

#include "../GameInfo.h"
#include "../Component/TileMapComponent.h"
#include "../ThreadQueue.h"
#include "NavigationThread.h"

class CNavigationManager
{
	friend class CScene;

private:
	CNavigationManager();
	~CNavigationManager();

private:
	class CScene* m_Scene;
	std::vector<CNavigationThread*> m_vecNavigationThread;
	CSharedPtr<CTileMapComponent> m_NavData;
	CThreadQueue<sNavResultData> m_ResultQueue;

public:
	void SetNavData(CTileMapComponent* navData);
	void AddNavResult(const sNavResultData& navData);

	inline CTileMapComponent* GetNavData() const
	{
		return m_NavData;
	}

public:
	void Start();
	bool Init();
	void Update(float deltaTime);

public:
	template <typename T>
	bool FindPath(T* obj, void(T::* func)(const std::list<Vector3>&),
		const Vector3& start, const Vector3& end)
	{
		if (m_vecNavigationThread.empty())
			return false;

		int count = m_vecNavigationThread[0]->GetWorkCount();
		int workerIndex = 0;

		size_t size = m_vecNavigationThread.size();

		for (size_t i = 1; i < size; i++)
		{
			if (count > m_vecNavigationThread[i]->GetWorkCount())
			{
				count = m_vecNavigationThread[i]->GetWorkCount();
				workerIndex = (int)i;
			}
		}

		m_vecNavigationThread[workerIndex]->AddWork<T>(obj, func, start, end);

		return true;
	}
};

