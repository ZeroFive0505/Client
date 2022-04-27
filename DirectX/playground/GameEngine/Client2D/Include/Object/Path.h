#pragma once

#include "GameObject/GameObject.h"
#include "Component/StaticMeshComponent.h"
#include "../Object/Line.h"

class CPath
{
public:
	CPath();
	CPath(const CPath* obj);
	virtual ~CPath();

private:
	std::vector<CSharedPtr<CLine>> m_vecPathLine;
	std::vector<Vector2> m_vecPoints;
	float m_Radius;

public:
	bool Init();

public:
	inline float GetPathRadius() const
	{
		return m_Radius;
	}

	inline const std::vector<Vector2>& GetVecPoints() const
	{
		return m_vecPoints;
	}

	inline Vector2 GetPoint(int index) const
	{
		return m_vecPoints[index];
	}
};

