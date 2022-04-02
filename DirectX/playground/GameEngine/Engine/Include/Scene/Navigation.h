#pragma once

#include "../Component/TileMapComponent.h"

enum class Node_Dir
{
	T,
	RT,
	R,
	RB,
	B,
	LB,
	L,
	LT,
	End
};

struct sNavNode
{
	sNavNode* parent;
	Nav_Node_Type nodeType;
	Tile_Type tileType;
	Vector3 pos;
	Vector3 size;
	Vector3 center;
	int indexX;
	int indexY;
	int index;
	float cost;
	float dist;
	float total;
	std::list<Node_Dir> searchDirList;

	sNavNode() :
		nodeType(Nav_Node_Type::None),
		tileType(Tile_Type::Normal),
		indexX(-1),
		indexY(-1),
		index(-1),
		cost(FLT_MAX),
		dist(FLT_MAX),
		total(FLT_MAX),
		parent(nullptr)
	{
		
	}
};

class CNavigation
{
	friend class CNavigationThread;

private:
	CNavigation();
	~CNavigation();

private:
	Tile_Shape m_NodeShape;
	std::vector<sNavNode*> m_vecNode;
	int m_CountX;
	int m_CountY;
	Vector3 m_TileSize;
	CSharedPtr<CTileMapComponent> m_TileMap;
	std::vector<sNavNode*> m_vecOpen;
	std::vector<sNavNode*> m_vecUseNode;

public:
	void CreateNavigationNode(class CTileMapComponent* tileMap);
	bool FindPath(const Vector3& start, const Vector3& end, std::list<Vector3>& path);

private:
	bool FindNode(sNavNode* node, sNavNode* endNode, const Vector3& end, std::list<Vector3>& path);
	sNavNode* GetCorner(Node_Dir dir, sNavNode* node, sNavNode* endNode, const Vector3& end);

private:
	sNavNode* GetRectNodeTop(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal = true);
	sNavNode* GetRectNodeRightTop(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRectNodeRight(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal = true);
	sNavNode* GetRectNodeRightBottom(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRectNodeBottom(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal = true);
	sNavNode* GetRectNodeLeftBottom(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRectNodeLeft(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal = true);
	sNavNode* GetRectNodeLeftTop(sNavNode* node, sNavNode* endNode, const Vector3& end);

private:
	sNavNode* GetRhombusNodeTop(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeRightTop(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeRight(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeRightBottom(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeBottom(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeLeftBottom(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeLeft(sNavNode* node, sNavNode* endNode, const Vector3& end);
	sNavNode* GetRhombusNodeLeftTop(sNavNode* node, sNavNode* endNode, const Vector3& end);

	void AddDir(Node_Dir dir, sNavNode* node);

private:
	inline static bool SortNode(const sNavNode* src, const sNavNode* dest)
	{
		return src->total > dest->total;
	}
};

