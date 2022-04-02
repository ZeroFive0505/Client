#pragma once

#include "../Thread.h"
#include "../ThreadQueue.h"

class CNavigationThread :
	public CThread
{
	friend class CNavigationManager;

public:
	CNavigationThread();
	virtual ~CNavigationThread();

private:
	class CNavigationManager* m_NavManager;
	CThreadQueue<sNavWorkData> m_WorkQueue;
	class CNavigation* m_Navigation;
	HANDLE m_ExitEvent;

public:
	inline int GetWorkCount()
	{
		return m_WorkQueue.size();
	}

	void CreateNavigationNode(class CTileMapComponent* tileMap);

public:
	virtual void Run();

public:
	template <typename T>
	void AddWork(T* obj, void(T::* func)(const std::list<Vector3>&),
		const Vector3& start, const Vector3& end)
	{
		sNavWorkData data;
		data.callBack = std::bind(func, obj, std::placeholders::_1);
		data.start = start;
		data.end = end;

		m_WorkQueue.push(data);
	}
};

