#pragma once

#include "Scene.h"
#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "../Object/ObjectInfo.h"

const float UI_FADEIN_TIME = 1.5f;
const float UI_FADEOUT_TIME = 1.5f;

/// <summary>
/// 컵과 코스의 선택 씬
/// </summary>
class CSelectScene :
	public CScene
{
	friend class CSceneManager;

private:
	CSelectScene();
	virtual ~CSelectScene();

	enum class EUIState
	{
		DRIVER,
		COURSE
	};

private:
	// 드라이버 선택 윈도우 창
	class CDriverSelectUI* m_DriverSelectUI;
	// 컵 선택 윈도우 창
	class CCourseSelectUI* m_CourseSelectUI;

	// 버섯 컵의 경우 보여줄 윈도우
	class CCourseWindow* m_MushroomWindow;

	// 플라워 컵의 경우 보여줄 윈도우
	class CCourseWindow* m_FlowerWindow;

	// 스타 컵의 경우
	class CCourseWindow* m_StarWindow;

	// 스페셜 컵의 경우
	class CCourseWindow* m_SpecialWindow;

	// 플레이어가 드라이버 선택할시에 움직일 커서의 위치들
	Vector2 m_PlayerOnePos[(int)EMenuPlayer::END] =
	{
		Vector2(98.0f, 128.0f),
		Vector2(195.0f, 128.0f),
		Vector2(290.0f, 128.0f),
		Vector2(386.0f, 128.0f),
		Vector2(98.0f, 255.0f),
		Vector2(194.0f, 255.0f),
		Vector2(289.0f, 255.0f),
		Vector2(387.0f, 255.0f)
	};

	// 확인
	CSharedPtr<CTexture> m_Confirm;

	// 1p 커서
	CSharedPtr<CTexture> m_1P;

	// 현재 드라이버
	EMenuPlayer m_CurrentDriver;

	// 현재 UI의 상태 (드라이버 상태인지 컵 상태인지)
	EUIState m_UIState;

	// 현재 선택된 컵
	ECupSelect m_CurrentCup;

	// 현재 CC 
	ECCOption m_CurrentCCOption;

	// 페이드 인, 아웃을 위한 좌표
	Vector2 m_FadeLT;
	Vector2 m_FadeRB;

	// 현재 커서의 좌표
	Vector2 m_CursorPos;

	float m_fElapsedTime;

	int m_CourseIndex;

	bool m_DriverSelected;
	bool m_CupSelected;
	bool m_FadeOut;
	bool m_FadeIn;

public:
	virtual bool Init() override;
	virtual bool Update(float deltaTime) override;
	virtual bool Render(HDC hDC) override;

public:
	inline void SetCurrentCC(ECCOption option)
	{
		m_CurrentCCOption = option;
	}

	void SetCCText(ECCOption option);

private:
	bool m_Start;
	void LoadSound();

	void Up(float deltaTime);
	void Down(float deltaTime);
	void Left(float deltaTime);
	void Right(float deltaTime);
	void Next(float deltaTime);
	void Back(float deltaTime);
};

