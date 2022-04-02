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

    // 이전에 사용하던 노드들이 있을수도 있으므로 길을 찾기전에 전에 사용했던 노드들의 데이터를 초기화한다.
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

    // 스타트 지점부터 시작한다.
    startNode->nodeType = Nav_Node_Type::Open;
    startNode->cost = 0.0f;
    startNode->dist = startNode->center.Distance(end);
    startNode->total = startNode->dist;

    // 첫 시작 노드는 8방향으로 탐색을 시작한다.
    for (int i = 0; i < (int)Node_Dir::End; i++)
    {
        startNode->searchDirList.push_back((Node_Dir)i);
    }

    m_vecOpen.push_back(startNode);

    m_vecUseNode.push_back(startNode);

    // 열린 목록이 있다면 무한 반복
    while (!m_vecOpen.empty())
    {
        sNavNode* node = m_vecOpen.back();
        m_vecOpen.pop_back();

        // 방문했으므로 닫힌 목록으로 
        node->nodeType = Nav_Node_Type::Close;

        // 8방향으로 타일을 검사해서 코너를 열린 목록에 넣어준다.
        if (FindNode(node, endNode, end, path))
            break;

        // 열린 목록을 정렬한다. 이 때 내림차순으로 정렬하여 가장 뒤에 가중치가 낮은 노드가 위치하도록 한다.
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

        // 찾은 노드가 도착 노드라면 경로를 만들어준다.
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

            // 시작점은 제외한다.
            path.pop_front();

            return true;
        }

        // 이동 비용을 구해준다.
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


        // 찾아준 노드가 이미 열린목록에 들어가 있을 경우 비용을 비교하여 좀 더 작은 비용의 경로로 교체한다.

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
    // 위로 이동할때는 노드를 한칸씩 위로 이동을 시키면서 해당 노드의 오른쪽이 막혀있고 오른쪽 위가 뚫려있거나
    // 왼쪽이 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
    int indexY = node->indexY;

    while (true)
    {
        // 현재 노드에서 한칸 위로 이동한다.
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

        // 오른쪽을 확인한다.
        int cornerX = node->indexX + 1;
        int cornerY = indexY;
        
        if (cornerX < m_CountX && cornerY + 1 < m_CountY)
        {
            // 만약 오른쪽이 벽이고 오른쪽 위가 뚫린상태라면 코너 발견
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        // 이번엔 왼쪽을 확인한다.
        cornerX = node->indexX - 1;
        cornerY = indexY;

        if (cornerX >= 0 && cornerY + 1 < m_CountY)
        {
            // 왼쪽이 막혀있고, 왼쪽 위가 뚫린 상태
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
    // 오른쪽 위로 이동할때는 노드를 한칸씩 오른쪽 위로 이동일 시키면서 해동 노드의 왼쪽이 막혀있고 왼쪽위가 뚫려있거나
    // 아래쪽이 막혀있고 오른쪽 아래가 뚫려 있으면 해당 노드는 코너가 된다.
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
            // 왼쪽이 막혀있고 왼쪽위가 뚫려있는 경우
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
            // 아래가 막혀있고 오른쪽 아래가 뚫려있는 경우
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }

        // 오른쪽 위 대각선을 체크시에 만약 현재 노드가 코너가 아니라면 오른쪽 방향과 위쪽 방향을 체크하여 코너의 유무를 확인한다.
        sNavNode* findNode = GetRectNodeTop(checkNode, endNode, end);

        // 만약 코너를 찾았다면 현재 노드를 코너로 체크한다.
        if (findNode)
            return checkNode;

        findNode = GetRectNodeRight(checkNode, endNode, end);

        if (findNode)
            return checkNode;
    }
}

sNavNode* CNavigation::GetRectNodeRight(sNavNode* node, sNavNode* endNode, const Vector3& end, bool diagonal)
{
    // 오른쪽으로 이동할때는 가로가 1씩 증가하며 위가 막혀있고 오른쪽 위는 지나갈 수 있거나
    // 아래가 막혀있고 오른쪽 아래를 지나갈 수 있는 경우 코너
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
            // 위쪽노드가 벽이고 오른쪽 위가 뚫려있을 경우
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
            // 아래가 벽이고 아래 오른쪽이 뚫려있을 경우
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
    // 오른쪽 아래로 이동할때는 노드를 한칸씩 오른쪽 아래로 이동을 시키면서 해당 노드의 왼쪽이 막혀있고 왼쪽 아래가 뚫려있거나
    // 위쪽이 막혀있고 오른쪽 위가 뚫려있으면 해당 노드는 코너가 된다.

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
            // 윈쪽이 막혀있고 왼쪽 아래가 뚫려있을 경우
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
            // 위와 오른쪽 위가 막혀있을 경우
            if ((m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Wall || 
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::SideWall ||
                m_vecNode[cornerY * m_CountX + cornerX]->tileType == Tile_Type::Pilar) &&
                m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileType == Tile_Type::Normal)
            {
                return checkNode;
            }
        }


        // 오른쪽 아래 대각선 체크시 만약 현 노드가 코너가 아니라면 오른쪽 방향과 아래쪽 방향을 체크하여
        // 코너가 있는지를 판단한다.
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
    // 아래로 이동할때는 노드를 한칸씩 아래로 이동을 시키면서 해당 노드의 오른쪽이 막혀있고 오른쪽 아래가 뚫려있거나
    // 왼쪽이 막혀있거나 왼쪽 아래가 뚫려 있으면 코너

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
            // 만약 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우
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
            // 만약 왼쪽이 막혀있고 왼쪽아래가 뚫려 있을경우
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
    // 왼쪽 아래로 이동할때는 노드를 한칸씩 왼쪽 아래로 이동을 시키면서 해당 노드의 오른쪽이 막혀있고 오른쪽 아래가 뚫려있거나
    // 위쪽이 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.

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
            // 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우
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
            // 위가 막혀있고 왼쪽위가 뚫려있을 경우
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
    // 왼쪽으로 이동할때는 가로가 1씩 감소하며 위가 막혀있고 윈쪽 위는 갈 수 있거나
    // 아래가 막혀있고 왼쪽 아래는 갈 수 있을 경우 코너가 된다.

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
            // 위가 막혀있고 왼쪽 위가 뚫려있을경우
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
            // 아래쪽이 막혀있고 왼쪽아래가 뚫려 있을경우
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
    // 왼쪽 위로 이동할때는 노드를 한칸씩 왼쪽 위로 이동을 시키면서 해동 노드의 오른쪽이 막혀있고 오른쪽 위가 뚫려있거나
    // 아래쪽이 막혀있고 왼쪽 아래가 뚫려 있으면 해당 노드는 코너가 된다.

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
        // 마름모 타일의 경우 바로 위 타일의 경우 2 증가한 위치
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

        // y가 홀수일 경우 x는 1 감소한다.
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
        // y가 홀수일 경우 x는 1증가
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

    // 진행방향을 유지하며 탐색을 확장한다.
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
