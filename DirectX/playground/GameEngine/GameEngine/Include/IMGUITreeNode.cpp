#include "IMGUITreeNode.h"

CIMGUITreeNode::CIMGUITreeNode() :
	m_Flag(ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth),
	m_AlignLabelWithXPos(false),
	m_DragAndDrop(false),
	m_SelectionMask(0),
	m_SelectedNode(-1)
{
}

CIMGUITreeNode::~CIMGUITreeNode()
{
}

bool CIMGUITreeNode::Init()
{
	return true;
}

void CIMGUITreeNode::Render()
{
	if (m_AlignLabelWithXPos)
		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());


	size_t size = m_vecItemUTF8.size();

	if (ImGui::TreeNode(m_Name.c_str()))
	{
		if (m_AlignLabelWithXPos)
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

		m_SelectionMask = 0;
		m_SelectedNode = -1;

		size_t size = m_vecItemUTF8.size();

		for (size_t i = 0; i < size; i++)
		{
			// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
			// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
			ImGuiTreeNodeFlags node_flags = m_Flag;
			const bool is_selected = (m_SelectionMask & (1 << i)) != 0;

			std::string currentNode = m_vecItemUTF8[i];

			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;


			if (!m_vecChildUTF8[currentNode].empty())
			{
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, currentNode.c_str());
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					m_SelectedNode = (int)i;
				if (m_DragAndDrop && ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
					ImGui::Text("This is a drag and drop source");
					ImGui::EndDragDropSource();
				}

				if (node_open)
				{
					size_t childCount = m_vecChildUTF8[currentNode].size();

					for (size_t j = 0; j < childCount; j++)
					{
						ImGui::BulletText(m_vecChildUTF8[currentNode][j].c_str());
					}
					ImGui::TreePop();
				}
			}
			else
			{
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, currentNode.c_str());
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					m_SelectedNode = (int)i;
				if (m_DragAndDrop && ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
					ImGui::Text("This is a drag and drop source");
					ImGui::EndDragDropSource();
				}
			}
		}

		if (m_SelectedNode != -1)
		{
			// Update selection state
			// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
			if (ImGui::GetIO().KeyCtrl)
				m_SelectionMask ^= (1 << m_SelectedNode);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
				m_SelectionMask = (1 << m_SelectedNode);           // Click to single-select
		}
		if (m_AlignLabelWithXPos)
			ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::TreePop();
	}

}
