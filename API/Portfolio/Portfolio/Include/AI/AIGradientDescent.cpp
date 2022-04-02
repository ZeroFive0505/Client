#include "AIGradientDescent.h"
#include "../Map/Tile.h"

int CAIGradientDescent::MAX_DISTANCE_BEHIND[(int)ECCOption::END] = { 100, 100, 75, 50 };
float CAIGradientDescent::MIN_PROB_BEHIND[(int)ECCOption::END] = { 0.6f, 0.7f, 0.75f, 0.8f };
int CAIGradientDescent::MAX_DISTANCE_AHEAD[(int)ECCOption::END] = { 100, 200, 300, 400 };
float CAIGradientDescent::MIN_PROB_AHEAD[(int)ECCOption::END] = { 0.45f, 0.45f, 0.45f, 0.45f };
int CAIGradientDescent::GradientMatrix[MAP_TILES_HEIGHT][MAP_TILES_WIDTH] = {};
int CAIGradientDescent::PositionMatrix[MAP_TILES_HEIGHT][MAP_TILES_WIDTH] = {};
int CAIGradientDescent::GRADIENT_LAP_CHECK = 0;
int CAIGradientDescent::MAX_POSITION_MATRIX = 0;


const std::array<Vector2, 8> CAIGradientDescent::EightNeibours =
{
    Vector2(0.0f, -1.0f), Vector2(-1.0f, 0.0f), Vector2(0.0f, 1.0f),
    Vector2(1.0f, 0.0f), Vector2(1.0f, -1.0f), Vector2(1.0f, 1.0f),
    Vector2(-1.0f, 1.0f), Vector2(-1.0f, -1.0f)
};


// 타일의 가중치를 반환한다.
int CAIGradientDescent::WeightTile(const ETileProperty tileType)
{
    switch (tileType)
    {
    case ETileProperty::ACCEL_PANEL:
    case ETileProperty::OTHER:
        return 1;
    case ETileProperty::TRACK:
    case ETileProperty::SPECIAL_13H:
        return 10;
    case ETileProperty::RAMP:
    case ETileProperty::RAMP_HORIZONTAL:
    case ETileProperty::RAMP_VERTICAL:
        return 50;
    case ETileProperty::OIL_SLICK:
    case ETileProperty::SLOW:
    case ETileProperty::SLOW_2:
        return 100;
    case ETileProperty::BLOCK:
    case ETileProperty::HAZARD:
        return 500000;
    default:
        return 0;
    }
}

void CAIGradientDescent::SetGradient(const std::vector<CTile*>& vecTiles, const Vector2& goalStartPos, const Vector2& goalSize, const std::string& positionsPath,
    const std::string& gradientPath)
{

    std::ifstream inGradient(gradientPath);
    std::ifstream inPosition(positionsPath);

    // 파일이 존재하면 파일을 로딩해서 설정한다.
    if (inGradient.good() && inPosition.good())
    {
        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                inGradient >> GradientMatrix[row][col];
                inPosition >> PositionMatrix[row][col];
            }
        }

        inPosition >> GRADIENT_LAP_CHECK;
        MAX_POSITION_MATRIX = GRADIENT_LAP_CHECK + 10;
    }
    else
    {
        // 파일이 존재하지 않을 경우 일단 초기화를 한다.
        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                GradientMatrix[row][col] = -2;
                PositionMatrix[row][col] = -1;
            }
        }


        // 먼저 벽을 확인한다.
        std::vector<std::vector<int>> wallPenalty(MAP_TILES_HEIGHT, std::vector<int>(MAP_TILES_WIDTH, 0));
        std::vector<Vector2> wallPenaltyFrontier;
        unsigned int numRows = MAP_TILES_HEIGHT;
        unsigned int numCols = MAP_TILES_WIDTH;

        for (unsigned int row = 0; row < numRows; row++)
        {
            for (unsigned int col = 0; col < numCols; col++)
            {
                // 만약 벽이거나 맵의 가장 가장자리의 경우 벽으로 처리한다.
                if (vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::BLOCK || row == 0 ||
                    row == numRows - 1 || col == 0 || col == numCols - 1)
                {
                    // 변화량 매트릭스에는 -1을 설정
                    GradientMatrix[row][col] = -1;
                    // 벽 패널티는 최대로
                    wallPenalty[row][col] = WALL_PENALTY_MAX;
                    // 좌표를 집어넣는다.
                    wallPenaltyFrontier.push_back(Vector2(col, row));
                }
                // 또는 느려지는 경우거나 트랙의 바깥일 경우도 벽과 마찬가지의 패널티
                else if (vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::SLOW ||
                    vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::SLOW_2 ||
                    vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::HAZARD)
                {
                    wallPenalty[row][col] = WALL_PENALTY_MAX;
                    wallPenaltyFrontier.push_back(Vector2(col, row));
                }
                else
                {
                    // 그외에는 패널티는 존재하지 않는다.
                    wallPenalty[row][col] = 0;
                }
            }
        }


        // 벽에 가까이 가면 추가되는 패널티
        int currentPenalty = WALL_PENALTY_MAX;
        // 총 3번 반복하며 주변의 벽을 찾는다.
        for (int pen = 0; pen < WALL_PENALTY_ITERS; pen++)
        {
            // 다음 벽 좌표의 후보
            std::vector<Vector2> nextWallPenaltyFrontier;

            // 벽의 좌표를 기록한 벡터를 순회한다.
            for (const auto& point : wallPenaltyFrontier)
            {
                // 좌표를 얻어오고
                int row = (int)point.y;
                int col = (int)point.x;

                // 패널티 값을 조회한다.
                int penalty = wallPenalty[row][col];

                // 만약 현재 패널티가 벽 패널티랑 동일하지 않다면
                // 건너뛴다.
                if (penalty != currentPenalty)
                    continue;

                // 아닐경우 좌표기준으로 8개의 이웃한 곳을 조회한다.
                for (const auto& delta : EightNeibours)
                {
                    // - 값을 없애기 위해 unsigned를 이용
                    unsigned int prow = row + delta.y;
                    unsigned int pcol = col + delta.x;

                    // 만약 범위안이고 벽 패널티에는 아무값도 기록이 되지 않았다면
                    if (prow < numRows && pcol < numCols &&
                        wallPenalty[prow][pcol] == 0)
                    {
                        // 완전한 패널티를 주지는 않고 어느정도 감소한 패널티를 준다.
                        wallPenalty[prow][pcol] = penalty / WALL_PENALTY_FACTOR;
                        // 다시 좌표를 기록한다.
                        nextWallPenaltyFrontier.push_back(Vector2(pcol, prow));
                    }
                }
            }

            // 다음 벽 근처로 이동한다.
            wallPenaltyFrontier = nextWallPenaltyFrontier;
            // 패널티는 다시 줄어든다.
            currentPenalty /= WALL_PENALTY_FACTOR;
        }


        // 골인 지점의 크기와 좌표를 통해
        // 골인 지점의 영역을 구한다.
        struct GoalRect
        {
            int x;
            int y;
            int width;
            int height;
        };

        GoalRect goalRect = {};

        goalRect.x = (goalStartPos.x / MAP_ASSETS_HEIGHT) * MAP_TILES_HEIGHT;
        goalRect.y = (goalStartPos.y / MAP_ASSETS_WIDTH) * MAP_TILES_WIDTH;
        goalRect.width = (goalSize.x / MAP_ASSETS_WIDTH) * MAP_TILES_WIDTH;
        goalRect.height = (goalSize.y / MAP_ASSETS_HEIGHT) * MAP_TILES_HEIGHT;


        // x, y, position, gradient
        using WeightTuple = std::tuple<int, int, int, int>;
        std::vector<WeightTuple> frontier;
        Vector2 lapCheckPos;

        // 골인 영역을 순회한다.
        for (int irow = 0; irow < goalRect.height; irow++)
        {
            for (int icol = 0; icol < goalRect.width; icol++)
            {
                // 정확한 영역의 좌표를 구한뒤에
                int row = goalRect.y + irow;
                int col = goalRect.x + icol;

                // 먼저 골인지점은 위치와 변화량은 0부터 시작한다.
                GradientMatrix[row][col] = 0;
                PositionMatrix[row][col] = 0;

                // 시작지점 바로 아래는 스타팅 시점으로 설정한다.
                if (vecTiles[(row + 1) * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::TRACK)
                {
                    int initWeight = WeightTile(ETileProperty::TRACK) + wallPenalty[row + 1][col];
                    GradientMatrix[row + 1][col] = initWeight;
                    PositionMatrix[row + 1][col] = 1;

                    frontier.push_back(WeightTuple(col, row + 1, initWeight, 1));
                }

                // 골인지점 바로 아래는 랩 체크 좌표로 설정한다.
                if (vecTiles[(row - 1) * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::TRACK)
                {
                    lapCheckPos = Vector2(col, row - 1);
                }
            }
        }

        // 경계선을 기준으로 계속 다음 경계선으로 넘어간다.
        while (!frontier.empty())
        {
            // 다음 경계선 후보
            std::vector<WeightTuple> nextFrontier;

            // 경계선을 순회한다.
            for (const WeightTuple& point : frontier)
            {
                // 8개의 근처 위치를 조사한다.
                for (const Vector2& delta : EightNeibours)
                {
                    int row = std::get<1>(point);
                    int col = std::get<0>(point);

                    if (vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::SPECIAL_13H)
                        col -= 13;

                    // 위치의 오프셋을 더한다.
                    row += delta.y;
                    col += delta.x;

                    // 가중치는 0 : x, 1 : y, 2 : grad, 3 : pos
                    // 다음 가중치는 grad + 타일 가중치 + 벽 충돌치
                    int weight = std::get<2>(point) + WeightTile(vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty()) + wallPenalty[row][col];

                    // 위치는 한 값 늘어난다.
                    int position = std::get<3>(point) + 1;

                    // 만약 그래디언트 값이 -2로 초기화되지 않았다면 또는 현제 가중치보다 더 크다면
                    if (GradientMatrix[row][col] == -2 || GradientMatrix[row][col] > weight)
                    {
                        // 가중치를 낮춘다.(더 좋은 곳을 찾았기에)
                        GradientMatrix[row][col] = weight;
                        // 위치도 새로 기록한다.
                        PositionMatrix[row][col] = position;

                        // 다음 경계선을 찾는다.
                        bool found = false;

                        // 다음 후보 경계선을 순회한다.
                        for (auto& element : nextFrontier)
                        {
                            // 만약 후보 경계선이 현재 좌표와 같다면
                            if (std::get<0>(element) == col &&
                                std::get<1>(element) == row)
                            {
                                // 값을 갱신한다.
                                std::get<2>(element) = weight;
                                std::get<3>(element) = position;

                                found = true;

                                break;
                            }
                        }

                        // 업데이트를 하지 못했으므로 새로운 후보를 등록한다.
                        if (!found)
                        {
                            nextFrontier.push_back(WeightTuple(col, row, weight, position));
                        }
                    }
                }
            }

            frontier = nextFrontier;
        }


        // 현재 위치 좌표에서 위로 10 올라가 체크 포인트를 갱신한다.
        GRADIENT_LAP_CHECK = PositionMatrix[(int)lapCheckPos.y][(int)lapCheckPos.x] - 10;
        // 랩을 체크하는 곳은 가장 큰 포지션 값을 가진다. 거기다 10을 더해서 제일 큰 값으로 만든다.
        MAX_POSITION_MATRIX = GRADIENT_LAP_CHECK + 10;

        // 파일로 출력
        std::ofstream out(gradientPath);

        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                out << GradientMatrix[row][col] << " ";
            }

            out << "\n";
        }

        // 파일로 출력
        std::ofstream out2(positionsPath);

        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                out2 << PositionMatrix[row][col] << " ";
            }

            out2 << "\n";
        }

        // 마지막으로 랩을 체크할 포지션의 정보를 집어넣는다.
        out2 << GRADIENT_LAP_CHECK << "\n";
    }
}


int CAIGradientDescent::GetPositionValue(unsigned int col, unsigned int row)
{
    row = min(row, (unsigned int)MAP_TILES_HEIGHT - 1);
    col = min(col, (unsigned int)MAP_TILES_WIDTH - 1);

    return PositionMatrix[row][col];
}

int CAIGradientDescent::GetPositionValue(const Vector2& pos)
{
    return GetPositionValue(int(pos.x * MAP_TILES_WIDTH), int(pos.y * MAP_TILES_HEIGHT));
}

Vector2 CAIGradientDescent::GetNextDirection(const Vector2& pos)
{
    int row = pos.y * MAP_TILES_HEIGHT;
    int col = pos.x * MAP_TILES_WIDTH;
    Vector2 bestDirection(0.0f, 0.0f);
    int bestWeight = GradientMatrix[row][col];

    // 변화량이 0인 지점은 즉 시작점이라는 예기
    if (bestWeight == 0)
    {
        // 따라서 그냥 직진으로 올라간다.
        Vector2 up = Vector2(0.0f, -1.0f / MAP_TILES_HEIGHT);

        return up;
    }

    // 8개의 인접한 공간을 순회한다.
    auto bestIter = EightNeibours.begin();
    int bestValue = -1;
    while (bestValue == -1)
    {
        bestValue = GradientMatrix[row + (int)bestIter->y][col + (int)bestIter->x];
        bestIter++;
    }

    auto iter = --bestIter;

    while (iter != EightNeibours.end())
    {
        // 후보 값을 찾아온다.
        int candValue = GradientMatrix[row + (int)iter->y][col + (int)iter->x];

        // 현재 값보다 더 작은 값이고, 벽이 아니며 0이 아니거나 또는 벽근처가 아니라면 새롭게 등록
        if (bestValue > candValue && candValue != -1 &&
            (candValue != 0 || (int)iter->y != 1))
        {
            bestValue = candValue;
            bestIter = iter;
        }

        iter++;
    }

    Vector2 dir = Vector2(bestIter->x / (float)MAP_TILES_WIDTH, bestIter->y / (float)MAP_TILES_HEIGHT);

    return dir;
}