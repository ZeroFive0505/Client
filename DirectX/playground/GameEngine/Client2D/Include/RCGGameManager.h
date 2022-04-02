#pragma once

#include "Object/RCGEnemy.h"
#include "Object/LockEventTrigger.h"

class CRCGGameManager
{
	friend class CClientManager;
protected:
	CRCGGameManager();
	~CRCGGameManager();

private:
	int m_Health;
	int m_SpecialGuage;

	std::list<CRCGEnemy*> m_Enemies;
	std::vector<Vector3> m_vecSpawnPoints;
	CLockEventTrigger* m_Event;

public:
	inline void AddEnemy(CRCGEnemy* enemy)
	{
		m_Enemies.push_back(enemy);
	}

	inline void DeleteEnemy(CRCGEnemy* enemy)
	{
		auto iter = std::find(m_Enemies.begin(), m_Enemies.end(), enemy);

		if (iter != m_Enemies.end())
			m_Enemies.erase(iter);
	}

	inline void ClearEnemies()
	{
		m_Enemies.clear();
	}

	inline void AddSpawnPoint(const Vector3& point)
	{
		m_vecSpawnPoints.push_back(point);
	}

	inline void ClearSpawnPoint()
	{
		m_vecSpawnPoints.clear();
	}

public:
	inline void SetHealth(int health)
	{
		m_Health = health;
	}

	inline void SetSpecialGuage(int guage)
	{
		m_SpecialGuage = guage;
	}

	inline void SetEventTrigger(CLockEventTrigger* trigger)
	{
		m_Event = trigger;
	}

public:
	inline int GetHealth() const
	{
		return m_Health;
	}

	inline int GetSpecialGuage() const
	{
		return m_SpecialGuage;
	}

	inline Vector3 GetRandomSpanwPoint() const
	{
		int index = rand() % (int)m_vecSpawnPoints.size();

		return m_vecSpawnPoints[index];
	}

	inline CLockEventTrigger* GetCurrentEvent() const
	{
		return m_Event;
	}

	inline const std::list<CRCGEnemy*>& GetEnemies() const
	{
		return m_Enemies;
	}

	inline bool CanSpawnEnemy() const
	{
		return (int)m_Enemies.size() < 5;
	}

public:
	void LoadSounds();

private:
	void LoadKyokoSounds();
	void LoadMisuzuSounds();
	void LoadSFX();
};

