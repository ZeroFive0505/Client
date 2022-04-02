#pragma once

#include "GameInfo.h"

// UI의 가장 기본이 되는 베이스 윈도우 클래스 여러 위젯을 가지고 있는다.
class CIMGUIWindow
{
public:
	CIMGUIWindow();
	virtual ~CIMGUIWindow();

protected:
	std::string m_Name;
	bool m_Open;
	int m_WindowFlag;
	std::vector<class CIMGUIWidget*> m_vecWidget;

protected:
	std::string m_PopupTitle;
	bool m_ModelPopup;
	std::vector<class CIMGUIWidget*> m_vecPopupWidget;

public:
	inline void SetName(const std::string& name)
	{
		m_Name = name;
	}

	inline void AddWindowFlag(ImGuiWindowFlags_ flag)
	{
		m_WindowFlag |= flag;
	}

	inline void EnableModalPopup()
	{
		m_ModelPopup = true;
	}

	inline void SetPopupWindowTitle(const std::string& title)
	{
		m_PopupTitle = title;
	}

public:
	inline void Open()
	{
		m_Open = true;
	}

	inline void Close()
	{
		m_Open = false;
	}

	void ClosePopup();
	class CIMGUIWidget* FindWidget(const std::string& name);

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

public:
	template <typename T>
	T* AddWidget(const std::string& name, float width = 100.0f, float height = 100.0f)
	{
		T* widget = new T;

		widget->SetName(name);
		widget->SetOwner(this);
		widget->SetSize(width, height);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}

		m_vecWidget.push_back(widget);

		return widget;
	}

	template <typename T>
	T* AddPopupWidget(const std::string& name, float width = 100.0f, float height = 100.0f)
	{
		T* widget = new T;

		widget->SetName(name);
		widget->SetOwner(this);
		widget->SetSize(width, height);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}

		m_vecPopupWidget.push_back(widget);
		
		return widget;
	}
};

