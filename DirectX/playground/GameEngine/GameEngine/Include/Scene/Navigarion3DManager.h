#pragma once

#include "../GameInfo.h"
#include "../Component/LandScape.h"

class CNavigarion3DManager
{
	friend class CScene;

private:
	CNavigarion3DManager();
	~CNavigarion3DManager();

private:
	class CScene* m_Scene;
	CSharedPtr<CLandScape> m_NavData;

public:
	void SetNavData(CLandScape* navData);
	void AddNavResult(const sNavResultData& navData);

public:
	float GetLandScapeHeight(const Vector3& pos);

public:
	void Start();
	bool Init();
	void Update(float deltaTime);
};

