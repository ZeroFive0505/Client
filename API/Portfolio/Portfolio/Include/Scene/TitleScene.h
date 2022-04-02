#pragma once

#include "../GameInfo.h"
#include "Scene.h"
#include "../Resource/Texture.h"

/// <summary>
/// 타이틀 화면 씬
/// </summary>
class CTitleScene :
	public CScene
{
	friend class CSceneManager;

private:
	CTitleScene();
	virtual ~CTitleScene();

private:
	enum class EMenuState
	{
		RACE,
		TOOL,
		MODE7,
		EXIT
	};

	// 타이틀 UI 윈도우
	class CTitleUI* m_TitleUI;

	// 레이스 메뉴 윈도우
	class CRaceMenuUI* m_RaceMenuUI;

	// 타이틀씬에서 무한 스크롤 배경
	class CScrollMap* m_ScrollMap;

	// 현재 메뉴 상태
	EMenuState m_CurrentMenu;
	
	// 몇 cc를 선택했는지
	ECCOption m_CCSelect;

	// 커서의 위치
	Vector2 m_CursorPos;

	bool m_Start;
	
	// 메뉴 팝업
	bool m_ShowMenu;

	// 레이스 메뉴 팝업
	bool m_ShowRaceMenu;

	// 메뉴에서 드라이버의 애니메이션을 위해서 이용
	float m_DriverFrequency[(int)EMenuPlayer::END];
	float m_CurrentTime;

public:
	virtual bool Init() override;
	virtual bool Update(float deltaTime) override;
	virtual bool Render(HDC hDC) override;

private:
	void LoadSound();

private:
	void MoveCursorUp(float deltaTime);
	void MoveCursorDown(float deltaTime);
	void Next(float deltaTime);
	void Back(float deltaTime);
};

