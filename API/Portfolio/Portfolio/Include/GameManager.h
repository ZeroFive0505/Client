#pragma once

#include "GameInfo.h"

/// <summary>
/// 게임 매니저클래스로 모든 싱글톤을 관리한다.
/// </summary>
class CGameManager
{
private:
	CGameManager();
	~CGameManager();

private:
	// 게임 루프
	static bool m_Loop;
	// 고유 번호
	HINSTANCE m_hInst;
	// 핸들러
	HWND m_hWnd;
	// 프론트 버퍼
	HDC m_hDC;
	// 백 버퍼
	HDC m_hBackDC;
	HBITMAP m_hBackBmp;
	HBITMAP m_hPrevBackBmp;
	Resolution m_RS;
	class CTimer* m_Timer;
	float m_TimeScale;

	HBRUSH m_hGreenBrush;
	HBRUSH m_hRedBrush;
	HBRUSH m_hYellowBrush;
	HBRUSH m_hDarkerYellowBrush;
	HBRUSH m_hBlackBrush;
	HBRUSH m_hOrangeBrush;


	HPEN m_hGreenPen;
	HPEN m_hRedPen;

	bool m_EditorMode;
	bool m_ShowGrid;

public:
	inline bool GetShowGrid() const
	{
		return m_ShowGrid;
	}

	inline Resolution GetResolution() const
	{
		return m_RS;
	}

	inline HPEN GetGreenPen() const
	{
		return m_hGreenPen;
	}

	inline HPEN GetRedPen() const
	{
		return m_hRedPen;
	}

	inline HBRUSH GetGreenBrush() const
	{
		return m_hGreenBrush;
	}

	inline HBRUSH GetRedBrush() const
	{
		return m_hRedBrush;
	}

	inline HBRUSH GetYellowBrush() const
	{
		return m_hYellowBrush;
	}

	inline HBRUSH GetDarkerYellowBrush() const
	{
		return m_hDarkerYellowBrush;
	}

	inline HBRUSH GetBlackBrush() const
	{
		return m_hBlackBrush;
	}

	inline HBRUSH GetOrangeBrush() const
	{
		return m_hOrangeBrush;
	}

	inline HDC GetWindowDC() const
	{
		return m_hDC;
	}

	inline HWND GetWindowHandle() const
	{
		return m_hWnd;
	}


	inline HINSTANCE GetWindowInstance() const
	{
		return m_hInst;
	}

	inline bool IsEditorMode() const
	{
		return m_EditorMode;
	}

	inline CTimer* GetTimer() const
	{
		return m_Timer;
	}

public:
	inline void SetEditorMode(bool val)
	{
		m_EditorMode = val;
	}

	inline void SetTimeScale(float timeScale)
	{
		m_TimeScale = timeScale;
	}

	inline void SetShowGrid(bool val)
	{
		m_ShowGrid = val;
	}

public:
	float GetDeltaTime() const;
	void Exit();

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);
	bool Collision(float deltaTime);
	void Render(float deltaTime);


private:
	ATOM Register();
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static CGameManager* m_Inst;

public:
	inline static CGameManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CGameManager;

		return m_Inst;
	}

	inline static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

