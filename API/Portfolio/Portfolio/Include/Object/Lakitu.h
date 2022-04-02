#pragma once


#include "../Resource/Texture.h"

#include "ObjectInfo.h"

/// <summary>
/// ±è¼öÇÑ¹«
/// </summary>
class CLakitu
{
public:
	CLakitu();
	virtual ~CLakitu();

private:
	CSharedPtr<CTexture> m_Finish[3];
	CSharedPtr<CTexture> m_WrongDir[2];

	CSharedPtr<CTexture> m_LapTexture[4];
	CSharedPtr<CTexture> m_Start[2];
	CSharedPtr<CTexture> m_Lights[4];

	CSharedPtr<CTexture> m_Catch;

	CSharedPtr<CTexture> m_Shadow;

	CSharedPtr<CTexture> m_LakituLab;

	CSharedPtr<CTexture> m_CurrentLakitu;
	CSharedPtr<CTexture> m_CurrentLight;
	CSharedPtr<CTexture> m_CurrentLap;

	class CRaceScene* m_Scene;

	class CDriver* m_Driver;

	const int m_FinishAnimIndex[4] = { 0, 1, 2, 1 };

	int m_TextIdx;

	float m_Scale = 0;

	enum class ELakituState
	{
		START,
		WRONG_DIR,
		GOING_UP,
		LAP,
		FINISH,
		SLEEP,
		PICKUP,
		END
	};

	int m_AnimationPriorities[(int)ELakituState::END] = { 0 };

	ELakituState m_State;

	int m_Laps;

	int m_Light;

	float m_ScreenTime;
	float m_WrongTime;
	float m_fElapsedTime;
	float m_NextFrameTime;
	float m_FrameTime;

	bool m_OnPosition;
	bool m_Right;

	bool m_Started;

	bool m_SoundStarted;

	bool m_DrawShadow;

	int m_CurrentAnimationPriority;
public:
	bool Init(class CRaceScene* scene);


public:
	void ShowRaceStart();
	void ShowCurrentLap(int numLap);
	void ShowRaceFinish();

	void WarningWrongDir(bool wrongDir);
	void PickUpDriver(CDriver* driver);

	void ShowUntil(float seconds, float deltaTime);

	void Sleep();

	bool HasRaceStarted();
	bool IsSleeping();

	void Update(float deltaTime);
	void Render(HDC hDC);

	void Reset();
};

