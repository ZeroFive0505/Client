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


// Ÿ���� ����ġ�� ��ȯ�Ѵ�.
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

    // ������ �����ϸ� ������ �ε��ؼ� �����Ѵ�.
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
        // ������ �������� ���� ��� �ϴ� �ʱ�ȭ�� �Ѵ�.
        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                GradientMatrix[row][col] = -2;
                PositionMatrix[row][col] = -1;
            }
        }


        // ���� ���� Ȯ���Ѵ�.
        std::vector<std::vector<int>> wallPenalty(MAP_TILES_HEIGHT, std::vector<int>(MAP_TILES_WIDTH, 0));
        std::vector<Vector2> wallPenaltyFrontier;
        unsigned int numRows = MAP_TILES_HEIGHT;
        unsigned int numCols = MAP_TILES_WIDTH;

        for (unsigned int row = 0; row < numRows; row++)
        {
            for (unsigned int col = 0; col < numCols; col++)
            {
                // ���� ���̰ų� ���� ���� �����ڸ��� ��� ������ ó���Ѵ�.
                if (vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::BLOCK || row == 0 ||
                    row == numRows - 1 || col == 0 || col == numCols - 1)
                {
                    // ��ȭ�� ��Ʈ�������� -1�� ����
                    GradientMatrix[row][col] = -1;
                    // �� �г�Ƽ�� �ִ��
                    wallPenalty[row][col] = WALL_PENALTY_MAX;
                    // ��ǥ�� ����ִ´�.
                    wallPenaltyFrontier.push_back(Vector2(col, row));
                }
                // �Ǵ� �������� ���ų� Ʈ���� �ٱ��� ��쵵 ���� ���������� �г�Ƽ
                else if (vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::SLOW ||
                    vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::SLOW_2 ||
                    vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::HAZARD)
                {
                    wallPenalty[row][col] = WALL_PENALTY_MAX;
                    wallPenaltyFrontier.push_back(Vector2(col, row));
                }
                else
                {
                    // �׿ܿ��� �г�Ƽ�� �������� �ʴ´�.
                    wallPenalty[row][col] = 0;
                }
            }
        }


        // ���� ������ ���� �߰��Ǵ� �г�Ƽ
        int currentPenalty = WALL_PENALTY_MAX;
        // �� 3�� �ݺ��ϸ� �ֺ��� ���� ã�´�.
        for (int pen = 0; pen < WALL_PENALTY_ITERS; pen++)
        {
            // ���� �� ��ǥ�� �ĺ�
            std::vector<Vector2> nextWallPenaltyFrontier;

            // ���� ��ǥ�� ����� ���͸� ��ȸ�Ѵ�.
            for (const auto& point : wallPenaltyFrontier)
            {
                // ��ǥ�� ������
                int row = (int)point.y;
                int col = (int)point.x;

                // �г�Ƽ ���� ��ȸ�Ѵ�.
                int penalty = wallPenalty[row][col];

                // ���� ���� �г�Ƽ�� �� �г�Ƽ�� �������� �ʴٸ�
                // �ǳʶڴ�.
                if (penalty != currentPenalty)
                    continue;

                // �ƴҰ�� ��ǥ�������� 8���� �̿��� ���� ��ȸ�Ѵ�.
                for (const auto& delta : EightNeibours)
                {
                    // - ���� ���ֱ� ���� unsigned�� �̿�
                    unsigned int prow = row + delta.y;
                    unsigned int pcol = col + delta.x;

                    // ���� �������̰� �� �г�Ƽ���� �ƹ����� ����� ���� �ʾҴٸ�
                    if (prow < numRows && pcol < numCols &&
                        wallPenalty[prow][pcol] == 0)
                    {
                        // ������ �г�Ƽ�� ������ �ʰ� ������� ������ �г�Ƽ�� �ش�.
                        wallPenalty[prow][pcol] = penalty / WALL_PENALTY_FACTOR;
                        // �ٽ� ��ǥ�� ����Ѵ�.
                        nextWallPenaltyFrontier.push_back(Vector2(pcol, prow));
                    }
                }
            }

            // ���� �� ��ó�� �̵��Ѵ�.
            wallPenaltyFrontier = nextWallPenaltyFrontier;
            // �г�Ƽ�� �ٽ� �پ���.
            currentPenalty /= WALL_PENALTY_FACTOR;
        }


        // ���� ������ ũ��� ��ǥ�� ����
        // ���� ������ ������ ���Ѵ�.
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

        // ���� ������ ��ȸ�Ѵ�.
        for (int irow = 0; irow < goalRect.height; irow++)
        {
            for (int icol = 0; icol < goalRect.width; icol++)
            {
                // ��Ȯ�� ������ ��ǥ�� ���ѵڿ�
                int row = goalRect.y + irow;
                int col = goalRect.x + icol;

                // ���� ���������� ��ġ�� ��ȭ���� 0���� �����Ѵ�.
                GradientMatrix[row][col] = 0;
                PositionMatrix[row][col] = 0;

                // �������� �ٷ� �Ʒ��� ��Ÿ�� �������� �����Ѵ�.
                if (vecTiles[(row + 1) * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::TRACK)
                {
                    int initWeight = WeightTile(ETileProperty::TRACK) + wallPenalty[row + 1][col];
                    GradientMatrix[row + 1][col] = initWeight;
                    PositionMatrix[row + 1][col] = 1;

                    frontier.push_back(WeightTuple(col, row + 1, initWeight, 1));
                }

                // �������� �ٷ� �Ʒ��� �� üũ ��ǥ�� �����Ѵ�.
                if (vecTiles[(row - 1) * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::TRACK)
                {
                    lapCheckPos = Vector2(col, row - 1);
                }
            }
        }

        // ��輱�� �������� ��� ���� ��輱���� �Ѿ��.
        while (!frontier.empty())
        {
            // ���� ��輱 �ĺ�
            std::vector<WeightTuple> nextFrontier;

            // ��輱�� ��ȸ�Ѵ�.
            for (const WeightTuple& point : frontier)
            {
                // 8���� ��ó ��ġ�� �����Ѵ�.
                for (const Vector2& delta : EightNeibours)
                {
                    int row = std::get<1>(point);
                    int col = std::get<0>(point);

                    if (vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty() == ETileProperty::SPECIAL_13H)
                        col -= 13;

                    // ��ġ�� �������� ���Ѵ�.
                    row += delta.y;
                    col += delta.x;

                    // ����ġ�� 0 : x, 1 : y, 2 : grad, 3 : pos
                    // ���� ����ġ�� grad + Ÿ�� ����ġ + �� �浹ġ
                    int weight = std::get<2>(point) + WeightTile(vecTiles[row * MAP_TILES_WIDTH + col]->GetTileProperty()) + wallPenalty[row][col];

                    // ��ġ�� �� �� �þ��.
                    int position = std::get<3>(point) + 1;

                    // ���� �׷����Ʈ ���� -2�� �ʱ�ȭ���� �ʾҴٸ� �Ǵ� ���� ����ġ���� �� ũ�ٸ�
                    if (GradientMatrix[row][col] == -2 || GradientMatrix[row][col] > weight)
                    {
                        // ����ġ�� �����.(�� ���� ���� ã�ұ⿡)
                        GradientMatrix[row][col] = weight;
                        // ��ġ�� ���� ����Ѵ�.
                        PositionMatrix[row][col] = position;

                        // ���� ��輱�� ã�´�.
                        bool found = false;

                        // ���� �ĺ� ��輱�� ��ȸ�Ѵ�.
                        for (auto& element : nextFrontier)
                        {
                            // ���� �ĺ� ��輱�� ���� ��ǥ�� ���ٸ�
                            if (std::get<0>(element) == col &&
                                std::get<1>(element) == row)
                            {
                                // ���� �����Ѵ�.
                                std::get<2>(element) = weight;
                                std::get<3>(element) = position;

                                found = true;

                                break;
                            }
                        }

                        // ������Ʈ�� ���� �������Ƿ� ���ο� �ĺ��� ����Ѵ�.
                        if (!found)
                        {
                            nextFrontier.push_back(WeightTuple(col, row, weight, position));
                        }
                    }
                }
            }

            frontier = nextFrontier;
        }


        // ���� ��ġ ��ǥ���� ���� 10 �ö� üũ ����Ʈ�� �����Ѵ�.
        GRADIENT_LAP_CHECK = PositionMatrix[(int)lapCheckPos.y][(int)lapCheckPos.x] - 10;
        // ���� üũ�ϴ� ���� ���� ū ������ ���� ������. �ű�� 10�� ���ؼ� ���� ū ������ �����.
        MAX_POSITION_MATRIX = GRADIENT_LAP_CHECK + 10;

        // ���Ϸ� ���
        std::ofstream out(gradientPath);

        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                out << GradientMatrix[row][col] << " ";
            }

            out << "\n";
        }

        // ���Ϸ� ���
        std::ofstream out2(positionsPath);

        for (unsigned int row = 0; row < MAP_TILES_HEIGHT; row++)
        {
            for (unsigned int col = 0; col < MAP_TILES_WIDTH; col++)
            {
                out2 << PositionMatrix[row][col] << " ";
            }

            out2 << "\n";
        }

        // ���������� ���� üũ�� �������� ������ ����ִ´�.
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

    // ��ȭ���� 0�� ������ �� �������̶�� ����
    if (bestWeight == 0)
    {
        // ���� �׳� �������� �ö󰣴�.
        Vector2 up = Vector2(0.0f, -1.0f / MAP_TILES_HEIGHT);

        return up;
    }

    // 8���� ������ ������ ��ȸ�Ѵ�.
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
        // �ĺ� ���� ã�ƿ´�.
        int candValue = GradientMatrix[row + (int)iter->y][col + (int)iter->x];

        // ���� ������ �� ���� ���̰�, ���� �ƴϸ� 0�� �ƴϰų� �Ǵ� ����ó�� �ƴ϶�� ���Ӱ� ���
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