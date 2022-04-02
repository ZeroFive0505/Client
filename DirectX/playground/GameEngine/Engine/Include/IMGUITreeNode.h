#pragma once

#include "IMGUIWidget.h"

class CIMGUITreeNode :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUITreeNode();
	virtual ~CIMGUITreeNode();

protected:
	ImGuiTreeNodeFlags m_Flag;
	bool m_AlignLabelWithXPos;
	bool m_DragAndDrop;
	int m_SelectionMask;
	int m_SelectedNode;

	std::vector<std::string> m_vecItem;
	std::vector<std::string> m_vecItemUTF8;

	std::unordered_map<std::string, std::vector<std::string>> m_vecChild;
	std::unordered_map<std::string, std::vector<std::string>> m_vecChildUTF8;

public:
	inline void AddTreeNodeFlag(ImGuiTreeNodeFlags flag)
	{
		m_Flag |= flag;
	}

	inline void SetAlignLabelIwthXPos(bool val)
	{
		m_AlignLabelWithXPos = val;
	}

	inline void SetDragAndDrop(bool val)
	{
		m_DragAndDrop = val;
	}

public:
	void AddTreeNode(const std::string& item)
	{
		m_vecItem.push_back(item);

		m_vecChild[item];

		wchar_t wItem[1024] = {};
		char itemUTF8[1024] = {};

		int length = MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wItem, length);

		length = WideCharToMultiByte(CP_UTF8, 0, wItem, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, wItem, -1, itemUTF8, length, 0, 0);

		m_vecItemUTF8.push_back(itemUTF8);

		m_vecChildUTF8[itemUTF8];
	}

	void AddChild(const std::string& parent, const std::string& item)
	{
		if (m_vecChild.find(parent) == m_vecChild.end() ||
			m_vecChildUTF8.find(parent) == m_vecChildUTF8.end())
			return;

		m_vecChild[parent].push_back(item);

		wchar_t wItem[1024] = {};
		char itemUTF8[1024] = {};

		int length = MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wItem, length);

		length = WideCharToMultiByte(CP_UTF8, 0, wItem, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, wItem, -1, itemUTF8, length, 0, 0);

		m_vecChildUTF8[itemUTF8].push_back(item);
	}

public:
	virtual bool Init();
	virtual void Render();
};

