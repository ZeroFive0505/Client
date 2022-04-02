#include "Navigation.h"
#include "../Component/TileMapComponent.h"

CNavigation::CNavigation()
{
}

CNavigation::~CNavigation()
{
    size_t size = m_vecNode.size();

    for (size_t i = 0; i < size; i++)
    {
        SAFE_DELETE(m_vecNode[i]);
    }

    m_vecNode.clear();
}

void CNavigation::CreateNavigationNode(CTileMapComponent* tileMap)
{
    m_TileMap = tileMap;

    m_NodeShape = m_TileMap->GetTileShape();
    m_CountX = m_TileMap->GetTileCountX();
    m_CountY = m_TileMap->GetTileCountY();
    m_TileSize = m_TileMap->GetTileSize();

    int count = m_CountX * m_CountY;

    for (int i = 0; i < count; i++)
    {
        sNavNode* node = new sNavNode;

        node->tileType = m_TileMap->GetTile(i)->GetTileType();
        node->pos = m_TileMap->GetTile(i)->GetPos();
        node->size = m_TileSize;
        node->center = node->pos + node->size * 0.5f;
        node->indexX = m_TileMap->GetTile(i)->GetIndexX();
        node->indexY = m_TileMap->GetTile(i)->GetIndexY();
        node->index = m_TileMap->GetTile(i)->GetIndex();

        m_vecNode.push_back(node);
    }
}

bool CNavigation::FindPath(const Vector3& start, const Vector3& end, std::list<Vector3>& path)
{
    path.clear();

    int startIndex = m_TileMap->GetTileIndex(start);

    if (startIndex == -1)
        return false;

    int endIndex = m_TileMap->GetTileIndex(end);

    if (endIndex == -1)
        return false;

    // ������ ����ϴ� ������ �������� �����Ƿ� ���� ã������ ���� ����ߴ� ������ �����͸� �ʱ�ȭ�Ѵ�.
    size_t size = m_vecUseNode.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecUseNode[i]->nodeType = Nav_Node_Type::None;
        m_vecUseNode[i]->cost = FLT_MAX;
        m_vecUseNode[i]->dist = FLT_MAX;
        m_vecUseNode[i]->total = FLT_MAX;
        m_vecUseNode[i]->parent = nullptr;
        m_vecUseNode[i]->searchDirList.clear();
    }

    m_vecUseNode.clear();

    sNavNode* startNode = m_vecNode[startIndex];
    sNavNode* endNode = m_vecNode[endIndex];

    if (endNode->tileType == Tile_Type::Wall || 
        endNode->tileType == Tile_Type::SideWall ||
        endNode->tileType == Tile_Type::Pilar)
        return false;
    else if (startNode == endNode)
    {
        path.push_back(end);
        return true;
    }

    // ��ŸƮ �������� �����Ѵ�.
    startNode->nodeType = Nav_Node_Type::Open;
    startNode->cost = 0.0f;
    startNode->dist = startNode->center.Distance(end);
    startNode->total = startNode->dist;

    // ù ���� ���� 8�������� Ž���� �����Ѵ�.
    for (int i = 0; i < (int)Node_Dir::End; i++)
    {
        startNode->searchDirList.push_back((Node_Dir)i);
    }

    m_vecOpen.push_back(startNode);

    m_vecUseNode.push_back(startNode);

    // ���� ����� �ִٸ� ���� �ݺ�
    while (!m_vecOpen.empty())
    {
        sNavNode* node = m_vecOpen.back();
        m_vecOpen.pop_back();

        // �湮�����Ƿ� ���� ������� 
        node->nodeType = Nav_Node_Type::Close;

        // 8�������� Ÿ���� �˻��ؼ� �ڳʸ� ���� ��Ͽ� �־��ش�.
        if (FindNode(node, endNode, end, path))
            break;

        // ���� ����� �����Ѵ�. �� �� ������������ �����Ͽ� ���� �ڿ� ����ġ�� ���� ��尡 ��ġ�ϵ��� �Ѵ�.
        if (!m_vecOpen.empty())
            std::sort(m_vecOpen.begin(), m_vecOpen.end(), SortNode);
    }

    return !path.empty();
}

bool CNavigation::FindNode(sNavNode* node, sNavNode* endNode, const Vector3& end, std::list<Vector3>& path)
{
    auto iter = node->searchDirList.begin();
    auto iterEnd = node->searchDirList.end();

    for (; iter != iterEnd; iter++)
    {
        sNavNode* corner = GetCorner(*iter, node, endNode, end);

        if (!corner)
            continue;

        // ã�� ��尡 ���� ����� ��θ� ������ش�.
        if (corner == endNode)
        {
            m_vecUseNode.push_back(corner);
            path.push_front(end);

            sNavNode* cur = node;

            while (cur)
            {
                path.push_front(cur->center);
                cur = cur->parent;
            }

            // �������� �����Ѵ�.
            path.pop_front();

            return true;
        }

        // �̵� ����� �����ش�.
        float cost = 0.0f;

        if (m_NodeShape == Tile_Shape::Rect ||
            m_NodeShape == Tile_Shape::RectWire)
        {
            switch (*iter)
            {
            case Node_Dir::T:
            case Node_Dir::B:
                cost = node->cost + abs(node->center.y - corner->center.y);
                break;    
            case Node_Dir::R:
            case Node_Dir::L:
                cost = node->cost + abs(node->center.x - corner->center.x);
                break;
            case Node_Dir::RT:
            case Node_Dir::RB:
            case Node_Dir::LB:
            case Node_Dir::LT:
                cost = node->cost + node->center.Distance(corner->center);
                break;
            }
        }
        else
        {
            switch (*iter)
            {
            case Node_Dir::T:
            case Node_Dir::B:
                cost = node->cost + abs(node->center.y - corner->center.y);
                break;
            case Node_Dir::R:
            case Node_Dir::L:
                cost = node->cost + abs(node->center.x - corner->center.x);
                break;
            case Node_Dir::RT:
            case Node_Dir::RB:
            case Node_Dir::LB:
            case Node_Dir::LT:
                cost = node->cost + node->center.Distance(corner->center);
                break;
            }
        }


        // ã���� ��尡 �̹� ������Ͽ� �� ���� ��� ����� ���Ͽ� �� �� ���� ����� ��η� ��ü�Ѵ�.

        if (corner->nodeType == Nav_Node_Type::Open)
        {
            if (corner->cost > cost)
            {
                corner->cost = cost;
                corner->total = corner->cost + corner->dist;
                corner->parent = node;

                AddDir(*iter, corner);
            }
        }
        else
        {
            corner->nodeType = Nav_Node_Type::Open;
            corner->parent = node;
            corner->cost = cost;
            corner->dist = corner->center.Distance(end);
            corner->total = corner->cost + corner->dist;
            corner->parent = node;

            m_vecOpen.push_back(corner);

            m_vecUseNode.push_back(corner);

            AddDir(*iter, corner);
        }
    }

    return false;
}

sNavNode* CNavigation::GetCorner(Node_Dir dir, sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    switch (m_NodeShape)
    {
    case Tile_Shape::Rect:
    case Tile_Shape::RectWire:
    {
        switch (dir)
        {
        case Node_Dir::T:
            return GetRectNodeTop(node, endNode, end, false);
        case Node_Dir::RT:
            return GetRectNodeRightTop(node, endNode, end);
        case Node_Dir::R:
            return GetRectNodeRight(node, endNode, end, false);
        case Node_Dir::RB:
            return GetRectNodeRightBottom(node, endNode, end);
        case Node_Dir::B:
            return GetRectNodeBottom(node, endNode, end, false);
        case Node_Dir::LB:
            return GetRectNodeLeftBottom(node, endNode, end);
        case Node_Dir::L:
            return GetRectNodeLeft(node, endNode, end, false);
        case Node_Dir::LT:
            return GetRectNodeLeftTop(node, endNode, end);
        }
    }
        break;
    case Tile_Shape::Rhombus:
    {
        switch (dir)
        {
        case Node_Dir::T:
            return GetRhombusNodeTop(node, endNode, end);
        case Node_Dir::RT:
            return GetRhombusNodeRightTop(node, endNode, end);
        case Node_Dir::R:
            return GetRhombusNodeRight(node, endNode, end);
        case Node_Dir::RB:
            return GetRhombusNodeRightBottom(node, endNode, end);
        case Node_Dir::B:
            return GetRhombusNodeBottom(node, endNode, end);
        case Node_Dir::LB:
            return GetRhombusNodeLeftBottom(node, endNode, end);
        case Node_Dir::L:
            return GetRhombusNodeLeft(node, endNode, end);
        case Node_Dir::LT:
            return GetRhombusNodeLeftTop(node, endNode, end);
        }
    }
        break;
    }


    return nullptr;
}

sNavNode* CNavigation::GetRectNodeTop(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal)
{
    // ���� �̵��Ҷ��� ��带 ��ĭ�� ���� �̵��� ��Ű�鼭 �ش� ����� �������� �����ְ� ������ ���� �շ��ְų�
    // ������ �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
    int indexY = node->indexY;

    while (true)
    {
        // ���� ��忡�� ��ĭ ���� �̵��Ѵ�.
        indexY++;

        if (indexY >= m_CountY)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + node->indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall || 
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        // �������� Ȯ���Ѵ�.
        int cornerX = node->indexX + 1;
        int cornerY = indexY;
        
        if (cornerX < m_CountX && cornerY + 1 < m_CountY)
        {
            // ���� �������� ���̰� ������ ���� �ո����¶�� �ڳ� �߰�
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        // �̹��� ������ Ȯ���Ѵ�.
        cornerX = node->indexX - 1;
        cornerY = indexY;

        if (cornerX >= 0 && cornerY + 1 < m_CountY)
        {
            // ������ �����ְ�, ���� ���� �ո� ����
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }
}

sNavNode* CNavigation::GetRectNodeRightTop(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    // ������ ���� �̵��Ҷ��� ��带 ��ĭ�� ������ ���� �̵��� ��Ű�鼭 �ص� ����� ������ �����ְ� �������� �շ��ְų�
    // �Ʒ����� �����ְ� ������ �Ʒ��� �շ� ������ �ش� ���� �ڳʰ� �ȴ�.
    int indexY = node->indexY;
    int indexX = node->indexX;

    while (true)
    {
        indexX++;
        indexY++;

        if (indexY >= m_CountY || indexX >= m_CountX)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX - 1;
        int cornerY = indexY;

        if (cornerX >= 0 && cornerY + 1 < m_CountY)
        {
            // ������ �����ְ� �������� �շ��ִ� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;
        cornerY = indexY - 1;

        if (cornerY >= 0 && cornerX + 1 < m_CountX)
        {
            // �Ʒ��� �����ְ� ������ �Ʒ��� �շ��ִ� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        // ������ �� �밢���� üũ�ÿ� ���� ���� ��尡 �ڳʰ� �ƴ϶�� ������ ����� ���� ������ üũ�Ͽ� �ڳ��� ������ Ȯ���Ѵ�.
        sNavNode* findNode = GetRectNodeTop(checkNode, endNode, end);

        // ���� �ڳʸ� ã�Ҵٸ� ���� ��带 �ڳʷ� üũ�Ѵ�.
        if (findNode)
            return checkNode;

        findNode = GetRectNodeRight(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }
}

sNavNode* CNavigation::GetRectNodeRight(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal)
{
    // ���������� �̵��Ҷ��� ���ΰ� 1�� �����ϸ� ���� �����ְ� ������ ���� ������ �� �ְų�
    // �Ʒ��� �����ְ� ������ �Ʒ��� ������ �� �ִ� ��� �ڳ�
    int indexX = node->indexX;

    while (true)
    {
        indexX++;

        if (indexX >= m_CountX)
            return nullptr;

        sNavNode* checkNode = m_vecNode[node->indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall || 
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;
        int cornerY = node->indexY + 1;

        if (cornerY < m_CountY && cornerX + 1 < m_CountX)
        {
            // ���ʳ�尡 ���̰� ������ ���� �շ����� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;
        cornerY = node->indexY - 1;

        if (cornerY >= 0 && cornerX + 1 < m_CountX)
        {
            // �Ʒ��� ���̰� �Ʒ� �������� �շ����� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }


    return nullptr;
}

sNavNode* CNavigation::GetRectNodeRightBottom(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    // ������ �Ʒ��� �̵��Ҷ��� ��带 ��ĭ�� ������ �Ʒ��� �̵��� ��Ű�鼭 �ش� ����� ������ �����ְ� ���� �Ʒ��� �շ��ְų�
    // ������ �����ְ� ������ ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.

    int indexY = node->indexY;
    int indexX = node->indexX;

    while (true)
    {
        indexY--;
        indexX++;

        if (indexY < 0 || indexX >= m_CountX)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX - 1;
        int cornerY = indexY;

        if (cornerX >= 0 && cornerY - 1 >= 0)
        {
            // ������ �����ְ� ���� �Ʒ��� �շ����� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;
        cornerY = indexY + 1;

        if (cornerY < m_CountY && cornerX + 1 < m_CountX)
        {
            // ���� ������ ���� �������� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }


        // ������ �Ʒ� �밢�� üũ�� ���� �� ��尡 �ڳʰ� �ƴ϶�� ������ ����� �Ʒ��� ������ üũ�Ͽ�
        // �ڳʰ� �ִ����� �Ǵ��Ѵ�.
        sNavNode* findNode = GetRectNodeBottom(checkNode, endNode, end);

        if (findNode)
            return checkNode;

        findNode = GetRectNodeRight(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRectNodeBottom(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal)
{
    // �Ʒ��� �̵��Ҷ��� ��带 ��ĭ�� �Ʒ��� �̵��� ��Ű�鼭 �ش� ����� �������� �����ְ� ������ �Ʒ��� �շ��ְų�
    // ������ �����ְų� ���� �Ʒ��� �շ� ������ �ڳ�

    int indexY = node->indexY;

    while (true)
    {
        indexY--;

        if (indexY < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + node->indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = node->indexX + 1;
        int cornerY = indexY;

        if (cornerX < m_CountX && cornerY - 1 >= 0)
        {
            // ���� �������� �����ְ� ������ �Ʒ��� �շ����� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = node->indexX - 1;
        cornerY = indexY;

        if (cornerX >= 0 && cornerY - 1 >= 0)
        {
            // ���� ������ �����ְ� ���ʾƷ��� �շ� �������
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }


    return nullptr;
}

sNavNode* CNavigation::GetRectNodeLeftBottom(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    // ���� �Ʒ��� �̵��Ҷ��� ��带 ��ĭ�� ���� �Ʒ��� �̵��� ��Ű�鼭 �ش� ����� �������� �����ְ� ������ �Ʒ��� �շ��ְų�
    // ������ �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.

    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        indexX--;
        indexY--;

        if (indexX < 0 || indexY < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX + 1;
        int cornerY = indexY;

        if (cornerX < m_CountX && cornerY - 1 >= 0)
        {
            // �������� �����ְ� ������ �Ʒ��� �շ����� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;
        cornerY = indexY + 1;

        if (cornerY < m_CountY && cornerX - 1 >= 0)
        {
            // ���� �����ְ� �������� �շ����� ���
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        sNavNode* findNode = GetRectNodeLeft(checkNode, endNode, end);

        if (findNode)
            return checkNode;

        findNode = GetRectNodeBottom(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRectNodeLeft(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal)
{
    // �������� �̵��Ҷ��� ���ΰ� 1�� �����ϸ� ���� �����ְ� ���� ���� �� �� �ְų�
    // �Ʒ��� �����ְ� ���� �Ʒ��� �� �� ���� ��� �ڳʰ� �ȴ�.

    int indexX = node->indexX;

    while (true)
    {
        indexX--;

        if (indexX < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[node->indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;
        int cornerY = node->indexY + 1;

        if (cornerY < m_CountY && cornerX - 1 >= 0)
        {
            // ���� �����ְ� ���� ���� �շ��������
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;
        cornerY = node->indexY - 1;

		if (cornerY >= 0 && cornerX - 1 >= 0)
		{
            // �Ʒ����� �����ְ� ���ʾƷ��� �շ� �������
			if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
				m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileType == Tile_Type::Normal)
			{
				return checkNode;
			}
		}
       
    }

    return nullptr;
}

sNavNode* CNavigation::GetRectNodeLeftTop(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    // ���� ���� �̵��Ҷ��� ��带 ��ĭ�� ���� ���� �̵��� ��Ű�鼭 �ص� ����� �������� �����ְ� ������ ���� �շ��ְų�
    // �Ʒ����� �����ְ� ���� �Ʒ��� �շ� ������ �ش� ���� �ڳʰ� �ȴ�.

    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        indexX--;
        indexY++;

        if (indexX < 0 || indexY >= m_CountY)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX + 1;
        int cornerY = indexY;

        if (cornerX < m_CountX && cornerY + 1 < m_CountY)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;
        cornerY = indexY - 1;

        if (cornerY >= 0 && cornerX - 1 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        sNavNode* findNode = GetRectNodeTop(checkNode, endNode, end);

        if (findNode)
            return checkNode;

        findNode = GetRectNodeLeft(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeTop(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;
    
    while (true)
    {
        // ������ Ÿ���� ��� �ٷ� �� Ÿ���� ��� 2 ������ ��ġ
        indexY += 2;

        if (indexY >= m_CountY)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;

        // y�� Ȧ���� ��� x�� 1 �����Ѵ�.
        if (indexY % 2 == 0)
            cornerX = indexX - 1;

        int cornerY = indexY - 1;

        if (cornerY >= 0 && indexX - 1 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        cornerY = indexY - 1;

        if (indexX + 1 < m_CountX && cornerY >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        sNavNode* findNode = GetRhombusNodeLeftTop(checkNode, endNode, end);

        if (findNode)
            return checkNode;

        findNode = GetRhombusNodeRightTop(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeRightTop(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        // y�� Ȧ���� ��� x�� 1����
        if (indexY % 2 == 1)
            indexX++;

        indexY++;

        if (indexY >= m_CountY || indexX >= m_CountX)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;
        int cornerY = indexY + 1;

        if (indexY % 2 == 0)
            cornerX--;

        if (cornerX >= 0 && indexY + 2 < m_CountY)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY + 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        cornerY = indexY - 1;

        if (cornerX < m_CountX && cornerY >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeRight(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        indexX++;

        if (indexX >= m_CountX)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;

        if (indexY % 2 == 0)
            cornerX = indexX - 1;

        int cornerY = indexY + 1;

        if (indexY + 2 < m_CountY && cornerX >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY + 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 0)
            cornerX = indexX - 1;

        cornerY = indexY - 1;

        if (cornerX >= 0 && indexY - 2 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY - 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        sNavNode* findNode = GetRhombusNodeRightTop(checkNode, endNode, end);

        if (findNode)
            return checkNode;

        findNode = GetRhombusNodeRightBottom(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeRightBottom(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        if (indexY % 2 == 1)
            indexX++;

        indexY--;

        if (indexY < 0 || indexX >= m_CountX)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;
        int cornerY = indexY + 1;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        if (indexX + 1 < m_CountX && cornerY < m_CountY)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 0)
            cornerX = indexX - 1;

        cornerY = indexY - 1;

        if (cornerX >= 0 && indexY - 2 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY - 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeBottom(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        indexY -= 2;

        if (indexY < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;

        if (indexY % 2 == 0)
            cornerX = indexX - 1;

        int cornerY = indexY + 1;

        if (cornerY < m_CountY && indexX - 1 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        cornerY = indexY + 1;

        if (indexX + 1 < m_CountX && cornerY < m_CountY)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        sNavNode* findNode = GetRhombusNodeLeftBottom(checkNode, endNode, end);

        if (findNode)
            return checkNode;

        findNode = GetRhombusNodeRightBottom(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeLeftBottom(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        if (indexY % 2 == 0)
            indexX--;

        indexY--;

        if (indexY < 0 || indexX < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;
        int cornerY = indexY + 1;

        if (indexY % 2 == 0)
            cornerX--;

        if (indexX - 1 >= 0 && cornerY < m_CountY)
        {
            if((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        cornerY = indexY - 1;

        if (cornerX < m_CountX && indexY - 2 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY - 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeLeft(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        indexX--;

        if (indexX < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        int cornerY = indexY + 1;

        if (indexY + 2 < m_CountY && cornerX < m_CountX)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY + 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 1)
            cornerX = indexX + 1;

        cornerY = indexY - 1;

        if (cornerX < m_CountX && indexY - 2 >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY - 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        sNavNode* findNode = GetRhombusNodeLeftTop(checkNode, endNode, end);

        if (findNode)
            checkNode;

        findNode = GetRhombusNodeLeftBottom(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }

    return nullptr;
}

sNavNode* CNavigation::GetRhombusNodeLeftTop(sNavNode* node, sNavNode* endNode, const Vector3& end)
{
    int indexX = node->indexX;
    int indexY = node->indexY;

    while (true)
    {
        if (indexY % 2 == 0)
            indexX--;

        indexY++;

        if (indexY >= m_CountY || indexX < 0)
            return nullptr;

        sNavNode* checkNode = m_vecNode[indexY * m_CountX + indexX];

        if (checkNode == endNode)
            return checkNode;
        else if (checkNode->nodeType == Nav_Node_Type::Close)
            return nullptr;
        else if (checkNode->tileType == Tile_Type::Wall ||
            checkNode->tileType == Tile_Type::SideWall ||
            checkNode->tileType == Tile_Type::Pilar)
            return nullptr;

        int cornerX = indexX;
        int cornerY = indexY + 1;

        if (indexY % 2 == 1)
            cornerX++;

        if (cornerX < m_CountX && indexY + 2 < m_CountY)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(indexY + 2) * m_CountX + indexX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        cornerX = indexX;

        if (indexY % 2 == 0)
            cornerX = indexX - 1;

        cornerY = indexY - 1;

        if (indexX - 1 >= 0 && cornerY >= 0)
        {
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[indexY * m_CountX + (indexX - 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }
    }

    return nullptr;
}

void CNavigation::AddDir(Node_Dir dir, sNavNode* node)
{
    node->searchDirList.clear();

    // ��������� �����ϸ� Ž���� Ȯ���Ѵ�.
    switch (m_NodeShape)
    {
    case Tile_Shape::Rect:
    case Tile_Shape::RectWire:
        switch (dir)
        {
        case Node_Dir::T:
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::RT);
            break;
        case Node_Dir::RT:
            node->searchDirList.push_back(Node_Dir::RT);
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::R);
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::RB);
            break;
        case Node_Dir::R:
            node->searchDirList.push_back(Node_Dir::R);
            node->searchDirList.push_back(Node_Dir::RB);
            node->searchDirList.push_back(Node_Dir::RT);
            break;
        case Node_Dir::RB:
            node->searchDirList.push_back(Node_Dir::RB);
            node->searchDirList.push_back(Node_Dir::R);
            node->searchDirList.push_back(Node_Dir::RT);
            node->searchDirList.push_back(Node_Dir::B);
            node->searchDirList.push_back(Node_Dir::LB);
            break;
        case Node_Dir::B:
            node->searchDirList.push_back(Node_Dir::B);
            node->searchDirList.push_back(Node_Dir::LB);
            node->searchDirList.push_back(Node_Dir::RB);
            break;
        case Node_Dir::LB:
            node->searchDirList.push_back(Node_Dir::LB);
            node->searchDirList.push_back(Node_Dir::B);
            node->searchDirList.push_back(Node_Dir::RB);
            node->searchDirList.push_back(Node_Dir::L);
            node->searchDirList.push_back(Node_Dir::LT);
            break;
        case Node_Dir::L:
            node->searchDirList.push_back(Node_Dir::L);
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::LB);
            break;
        case Node_Dir::LT:
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::RT);
            node->searchDirList.push_back(Node_Dir::L);
            node->searchDirList.push_back(Node_Dir::LB);
            break;
        }
        break;
    case Tile_Shape::Rhombus:
        switch (dir)
        {
        case Node_Dir::T:
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::L);
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::R);
            node->searchDirList.push_back(Node_Dir::RT);
            break;
        case Node_Dir::RT:
            node->searchDirList.push_back(Node_Dir::RT);
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::R);
            break;
        case Node_Dir::R:
            node->searchDirList.push_back(Node_Dir::R);
            node->searchDirList.push_back(Node_Dir::RT);
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::RB);
            node->searchDirList.push_back(Node_Dir::B);
            break;
        case Node_Dir::RB:
            node->searchDirList.push_back(Node_Dir::RB);
            node->searchDirList.push_back(Node_Dir::B);
            node->searchDirList.push_back(Node_Dir::R);
            break;
        case Node_Dir::B:
            node->searchDirList.push_back(Node_Dir::B);
            node->searchDirList.push_back(Node_Dir::L);
            node->searchDirList.push_back(Node_Dir::LB);
            node->searchDirList.push_back(Node_Dir::R);
            node->searchDirList.push_back(Node_Dir::RB);
            break;
        case Node_Dir::LB:
            node->searchDirList.push_back(Node_Dir::LB);
            node->searchDirList.push_back(Node_Dir::B);
            node->searchDirList.push_back(Node_Dir::L);
            break;
        case Node_Dir::L:
            node->searchDirList.push_back(Node_Dir::L);
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::LB);
            node->searchDirList.push_back(Node_Dir::B);
            break;
        case Node_Dir::LT:
            node->searchDirList.push_back(Node_Dir::LT);
            node->searchDirList.push_back(Node_Dir::T);
            node->searchDirList.push_back(Node_Dir::L);
            break;
        }
        break;
    }
}
