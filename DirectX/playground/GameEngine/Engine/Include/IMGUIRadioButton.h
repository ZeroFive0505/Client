#pragma once

#include "IMGUIWidget.h"

class CIMGUIRadioButton :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIRadioButton();
	virtual ~CIMGUIRadioButton();

protected:
	std::vector<std::string> m_vecItem;
	std::vector<std::string> m_vecItemUTF8;
	std::vector<std::function<void()>> m_SelectCallbacks;
	bool m_SameLine;
	int m_Mode;

public:
	virtual bool Init();
	virtual void Render();

public:
	inline void SetSameLine(bool val)
	{
		m_SameLine = val;
	}

	void AddItem(const std::string& item)
	{
		m_vecItem.push_back(item);

		wchar_t wItem[1024] = {};
		char itemUTF8[1024] = {};

		int length = MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wItem, length);

		length = WideCharToMultiByte(CP_UTF8, 0, wItem, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, wItem, -1, itemUTF8, length, 0, 0);

	
		m_vecItemUTF8.push_back(itemUTF8);

		m_SelectCallbacks.push_back(nullptr);
	}

	inline void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
		m_SelectCallbacks.clear();
	}

public:
	template <typename T>
	void SetSelectCallback(T* obj, void(T::* func)(), int index)
	{
		m_SelectCallbacks[index] = std::bind(func, obj);
	}
};

