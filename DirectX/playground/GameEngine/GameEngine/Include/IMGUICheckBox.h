#pragma once

#include "IMGUIWidget.h"

class CIMGUICheckBox :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUICheckBox();
	virtual ~CIMGUICheckBox();

protected:
	std::vector<std::string> m_vecItem;
	std::vector<std::string> m_vecItemUTF8;
	std::vector<std::function<void()>> m_SelectCallbacks;
	std::vector<std::function<void()>> m_DeSelectCallbacks;
	std::vector<bool> m_vecCheck;
	bool m_SameLine;

protected:
	virtual bool Init();
	virtual void Render();

public:
	inline void SetSameLine(bool val)
	{
		m_SameLine = val;
	}

	inline void SetVal(int index, bool val)
	{
		if (index >= m_vecCheck.size())
			return;

		m_vecCheck[index] = val;
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
		m_DeSelectCallbacks.push_back(nullptr);
		m_vecCheck.push_back(false);
	}

	inline void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
		m_vecCheck.clear();
		m_SelectCallbacks.clear();
		m_DeSelectCallbacks.clear();
	}

public:
	template <typename T>
	void SetSelectCallback(T* obj, void(T::* func)(), int index)
	{
		m_SelectCallbacks[index] = std::bind(func, obj);
	}

	template <typename T>
	void SetDeSelectCallback(T* obj, void(T::* func)(), int index)
	{
		m_DeSelectCallbacks[index] = std::bind(func, obj);
	}
};

