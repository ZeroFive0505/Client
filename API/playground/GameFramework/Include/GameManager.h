#pragma once

#include "GameInfo.h"
/// <summary>
/// 게임매니저 클래스는 싱글톤으로 게임을 관리한다. 게임매니저 클래스는 생성되고 초기화될때
/// 다른 매니저(싱글톤)들도 생성하고 소멸할때는 다른 매니저들도 같이 소멸을 하게된다.
/// </summary>
class CGameManager
{
	// 싱들톤이기에 생성자와 소멸자는 private으로 외부에서 맘대로 생성이 불가능하게 함.
private:
	CGameManager();
	~CGameManager();

private:
	// m_Loop가 true일 동안 계속해서 반복해서 렌더링한다.
	static bool	m_Loop;
	// 윈도우 인스턴스
	HINSTANCE	m_hInst;
	// 윈도우 핸들러
	HWND		m_hWnd;
	// 디바이스 컨텍스트
	HDC			m_hDC;
	// 백 버퍼 디바이스 컨텍스트
	HDC m_hBackDC;
	HBITMAP m_hBackBmp;
	HBITMAP m_hPrevBackBmp;
	// 해상도 조절을 위한 변수
	Resolution	m_RS;
	// DeltaTime과 FPS 계산을 위한 타이머 클래스
	class CTimer* m_Timer;
	// 현재 게임의 시간 비율이 어떻게 되는지 설정
	float m_TimeScale;

	HBRUSH m_hGreenBrush;
	HBRUSH m_hRedBrush;
	HBRUSH m_hYellowBrush;

	HPEN m_hGreenPen;
	HPEN m_hRedPen;

	bool m_EditorMode;

public:
	Resolution GetResolution() const
	{
		return m_RS;
	}

	HPEN GetGreenPen() const
	{
		return m_hGreenPen;
	}

	HPEN GetRedPen() const
	{
		return m_hRedPen;
	}

	HBRUSH GetGreenBrush() const
	{
		return m_hGreenBrush;
	}

	HBRUSH GetRedBrush() const
	{
		return m_hRedBrush;
	}

	HBRUSH GetYellowBrush() const
	{
		return m_hYellowBrush;
	}

	HDC GetWindowDC() const
	{
		return m_hDC;
	}

	HWND GetWindowHandle() const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance() const
	{
		return m_hInst;
	}

	bool IsEditorMode() const
	{
		return m_EditorMode;
	}

public:
	void SetEditorMode(bool Mode)
	{
		m_EditorMode = Mode;
	}

	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}

public:
	float GetDeltaTime() const;
	void Exit();

public:
	bool Init(HINSTANCE hInst);
	int Run();


private:
	void Logic();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(float DeltaTime);


private:
	ATOM Register();
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	static CGameManager* m_Inst;

public:
	static CGameManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CGameManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}

public:
	class CTimer* GetTimer() const
	{
		return m_Timer;
	}
};

