#pragma once

#include "GameInfo.h"
#include "Timer.h"

class CEngine
{
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	sResolution m_RS;
	static bool m_Loop;
	float m_ClearColors[4];
	class CTimer* m_Timer;
	bool m_Start;
	bool m_Play;
	Engine_Space m_Space;
	int m_ShowCursorCount;
	Mouse_State m_MouseState;
	CSharedPtr<class CWidgetWindow> m_MouseWidget[(int)Mouse_State::MAX];
	class CGlobalConstantBuffer* m_GlobalCBuffer;
	float m_GlobalAccTime;
	CSharedPtr<class CTexture> m_GlobalNoiseTexture;

	bool m_SceneChangeStart;

	float m_ElapsedTime;

private:
	bool m_Slow;
	float m_SlowTime;
	float m_SlowDuration;
	float m_SlowScale;

private:
	float* m_NoiseSeed1D;
	float* m_PerlinNoise1D;
	int m_PerlinOutPutSize1D;

	int m_PerlinNoiseWidth;
	int m_PerlinNoiseHeight;
	float* m_NoiseSeed2D;
	float* m_PerlinNoise2D;

	int m_OctaveCount;
	float m_ScalingBias;

public:
	void PerlinNoise1D();
	void PerlinNoise2D();

public:
	inline bool IsSlowMotion() const
	{
		return m_Slow;
	}

	inline void SetSlowMotion(float duration, float scale)
	{
		m_Slow = true;
		m_SlowTime = 0.0f;
		m_SlowDuration = duration;
	}

public:
	inline float GetNoise1D(int x) const
	{
		x %= m_PerlinNoiseWidth;

		return m_PerlinNoise1D[x];
	}


	inline float GetNoise2D(int x, int y) const
	{
		x %= m_PerlinNoiseWidth;
		y %= m_PerlinNoiseHeight;

		return m_PerlinNoise2D[y * m_PerlinNoiseWidth + x];
	}

	inline int GetPerlinNoise2DWidth() const
	{
		return m_PerlinNoiseWidth;
	}

	inline int GetPerlinNoise2DHeight() const
	{
		return m_PerlinNoiseHeight;
	}

	inline int GetPerlinNoise1DSize() const
	{
		return m_PerlinOutPutSize1D;
	}

public:
	inline Engine_Space GetEngineSpace() const
	{
		return m_Space;
	}

	inline void SetEngineSpace(Engine_Space space)
	{
		m_Space = space;
	}

	inline bool IsPlay() const
	{
		return m_Play;
	}

	inline void SetPlay(bool play)
	{
		m_Play = play;
	}

	void SetMouseState(Mouse_State state);

public:
	inline HWND GetWindowHandle() const
	{
		return m_hWnd;
	}

	inline HINSTANCE GetWindowInstance() const
	{
		return m_hInst;
	}

	inline sResolution GetResolution() const
	{
		return m_RS;
	}

	inline class CWidgetWindow* GetMouseWidget() const
	{
		return m_MouseWidget[(int)m_MouseState];
	}

	inline void Exit()
	{
		DestroyWindow(m_hWnd);
	}

	inline float GetFPS() const
	{
		return m_Timer->GetFPS();
	}

	inline float GetDeltaTime() const
	{
		return m_Timer->GetDeltaTime();
	}

public:
	bool Init(HINSTANCE hInst, const TCHAR* name, unsigned int width,
		const int height, int iconID, bool windowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int width, unsigned int height, bool windowMode = true);

	int Run();

	void Logic();

private:
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);
	bool Render(float deltaTime);

private:
	ATOM Register(const TCHAR* name, int iconID);
	BOOL Create(const TCHAR* name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	template <typename T>
	T* CreateMouse(Mouse_State state, const std::string& name)
	{
		T* window = new T;

		window->SetName(name);

		if (!window->Init())
		{
			SAFE_RELEASE(window);
			return nullptr;
		}

		m_MouseWidget[(int)state] = window;

		return window;
	}

	DECLARE_SINGLE(CEngine);
};