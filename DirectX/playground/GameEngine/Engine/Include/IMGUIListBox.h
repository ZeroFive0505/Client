#pragma once

#include "IMGUIWidget.h"

class CIMGUIListBox :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIListBox();
	virtual ~CIMGUIListBox();

protected:
	std::vector<std::string> m_vecItem;
	std::vector<std::string> m_vecItemUTF8;
	// 선택된 항목이 있는지
	bool m_Select;
	// 현재 선택된 항목의 인덱스
	int m_SelectIndex;
	// 리스트박스에서 최대로 보이는 아이템의 갯수
	// 만약 넘어갈 경우 스크롤바를 통해서 다른 아이템을 확인 가능
	int m_PageItemCount;
	// 정렬
	bool m_Sort;
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

	bool CheckItem(const char* item)
	{
		size_t size = m_vecItem.size();

		for (size_t i = 0; i < size; i++)
		{
			if (m_vecItem[i] == item)
				return true;
		}

		return false;
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

			m_vecItemUTF8.erase(iter);
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

