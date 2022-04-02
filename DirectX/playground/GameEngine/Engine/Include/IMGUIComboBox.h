#pragma once

#include "IMGUIWidget.h"

class CIMGUIComboBox :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIComboBox();
	virtual ~CIMGUIComboBox();

protected:
	std::vector<std::string> m_vecItem;
	std::vector<std::string> m_vecItemUTF8;
	std::string m_PrevViewName;
	bool m_Select;
	int m_SelectIndex;
	int m_PageItemCount;
	bool m_Sort;
	ImGuiComboFlags m_Flag;
	std::function<void(int, const char*)> m_SelectCallback;

public:
	inline const std::string& GetItem(int index) const
	{
		return m_vecItem[index];
	}

	inline const std::string& GetSelectItem() const
	{
		return m_vecItem[m_SelectIndex];
	}

	inline int GetSelectIndex() const
	{
		return m_SelectIndex;
	}

	inline int GetItemCount() const
	{
		return (int)m_vecItem.size();
	}

public:
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

		if (m_Sort)
		{
			std::sort(m_vecItem.begin(), m_vecItem.end());
			std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
		}
	}

	inline void SetPageItemCount(int count)
	{
		m_PageItemCount = count;
	}

	inline void DeleteItem(int index)
	{
		if (index >= (int)m_vecItem.size() || index >= (int)m_vecItemUTF8.size())
			return;

		{
			auto iter = m_vecItem.begin() + index;

			m_vecItem.erase(iter);
		}

		{
			auto iter = m_vecItemUTF8.begin() + index;

			m_vecItem.erase(iter);
		}
	}

	inline void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
	}

	inline void Sort(bool bSort)
	{
		m_Sort = bSort;

		if (m_Sort)
		{
			std::sort(m_vecItem.begin(), m_vecItem.end());
			std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
		}
	}

	inline void AddFlag(ImGuiComboFlags_ flag)
	{
		m_Flag |= flag;
	}

	inline void ResetSelectedIndex()
	{
		m_SelectIndex = -1;
	}
	
public:
	virtual bool Init();
	virtual void Render();
	
public:
	template <typename T>
	void SetSelectCallback(T* obj, void(T::* func)(int, const char*))
	{
		m_SelectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
};

