#pragma once

#include "GameInfo.h"

// IMGUI 윈도우들을 관리할 매니저 클래스
class CIMGUIManager
{
private:
	ImGuiContext* m_Context;
	std::unordered_map<std::string, class CIMGUIWindow*> m_mapWindow;
	std::unordered_map<std::string, ImFont*> m_mapFont;
	ImFont* m_CurrentFont;

public:
	bool Init(HWND hWnd);
	void Update(float deltaTime);
	void Render();
	class CIMGUIWindow* FindIMGUIWindow(const std::string& name);

public:
	bool AddFont(const std::string& name, const char* fileName, float size, bool korean = false, int overH = 2, int overV = 1,
		float spacing = 1.0f, const std::string& pathName = FONT_PATH);
	bool AddFontFullPath(const std::string& name, const char* fullPath, float size, bool korean = false, int overH = 2, int overV = 1,
		float spacing = 1.0f);
	void SetCurrentFont(const std::string& name);
	ImFont* FindFont(const std::string& name);
	void SetCurrentFont();
	void ResetCurrentFont();

public:
	template <typename T>
	T* AddWindow(const std::string& name)
	{
		T* window = (T*)FindIMGUIWindow(name);

		if (window)
			return window;

		window = new T;

		window->SetName(name);

		if (!window->Init())
		{
			SAFE_DELETE(window);
			return nullptr;
		}

		m_mapWindow.insert(std::make_pair(name, window));

		return window;
	}

	DECLARE_SINGLE(CIMGUIManager)
};

