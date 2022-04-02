
#include <iostream>
#include <time.h>

int main()
{
    srand((unsigned int)time(0));
    rand();

    int Number[25] = {};
    int AINumber[25] = {};

    for (int i = 0; i < 25; ++i)
    {
        Number[i] = i + 1;
        AINumber[i] = i + 1;
    }

    for (int i = 0; i < 100; ++i)
    {
        int Index1 = rand() % 25;
        int Index2 = rand() % 25;

        int Temp = Number[Index1];
        Number[Index1] = Number[Index2];
        Number[Index2] = Temp;

        Index1 = rand() % 25;
        Index2 = rand() % 25;

        Temp = AINumber[Index1];
        AINumber[Index1] = AINumber[Index2];
        AINumber[Index2] = Temp;
    }

    /*
    1, 2, 3, *, 5, 6, 7, *, *, 10

    10개짜리의 배열에 *이 아닌 수만
    넣어준다.
    1, 2, 3, 5, 6, 7, 10

    총 7개가 들어가고 7개중 랜덤한 인덱스를 구해서 그 값을 선택하게 해준다.
    */
    int SelectNumber[25] = {};
    int SelectCount = 0;

    int Bingo = 0, AIBingo = 0;

    while (true)
    {
        system("cls");

        std::cout << "====================== Player ======================" << std::endl << std::endl;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                // INT_MAX 는 int타입으로 표현할 수 있는 최대값이 상수로 정해져 있는 것이다.
                if (Number[i * 5 + j] == INT_MAX)
                    std::cout << "*\t";

                else
                    std::cout << Number[i * 5 + j] << "\t";
            }

            // 가로 한줄이 출력이 완료되었다면 개행한다.
            std::cout << std::endl;
        }

        std::cout << "Bingo : " << Bingo << std::endl;
        std::cout << std::endl;

        std::cout << "====================== AI ======================" << std::endl << std::endl;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                // INT_MAX 는 int타입으로 표현할 수 있는 최대값이 상수로 정해져 있는 것이다.
                if (AINumber[i * 5 + j] == INT_MAX)
                    std::cout << "*\t";

                else
                    std::cout << AINumber[i * 5 + j] << "\t";
            }

            // 가로 한줄이 출력이 완료되었다면 개행한다.
            std::cout << std::endl;
        }

        int Input = 0;
        std::cout << "AIBingo : " << AIBingo << std::endl;

        if (Bingo >= 5)
        {
            std::cout << "Player 승리" << std::endl;
            break;
        }

        else if (AIBingo >= 5)
        {
            std::cout << "AI 승리" << std::endl;
            break;
        }

        std::cout << "Input Number(0 : Exit) : ";
        std::cin >> Input;

        if (Input == 0)
            break;

        // 1 ~ 25 사이의 숫자가 아니라면 다시 입력하게 한다.
        else if (Input < 0 || Input > 25)
            continue;

        // 입력받은 숫자를 찾아서 *로 바꿔준다.
        // 단, 이미 *로 바뀌어버린 숫자를 다시 입력하면
        // 다시 처음부터 입력받도록 한다.
        bool    Find = false;

        for (int i = 0; i < 25; ++i)
        {
            if (Number[i] == Input)
            {
                Find = true;
                Number[i] = INT_MAX;
                break;
            }
        }

        // 만약 못찾았다면 다시 입력받게 한다.
        if (Find == false)
            continue;

        // AI의 빙고숫자에서 플레이어가 입력한 값을 찾아서 *로 바꿔준다.
        for (int i = 0; i < 25; ++i)
        {
            if (AINumber[i] == Input)
            {
                AINumber[i] = INT_MAX;
                break;
            }
        }

        // AI가 하나 선택한다. 단, 현재 입력이 안된 숫자중 하나를 랜덤하게 선택해야 한다.
        // 선택을 위해서 *이 아닌 일반 숫자들을 배열에 넣어준다.
        SelectCount = 0;
        for (int i = 0; i < 25; ++i)
        {
            if (AINumber[i] != INT_MAX)
            {
                SelectNumber[SelectCount] = AINumber[i];
                ++SelectCount;
            }
        }

        // 여기로 빠져나오면 SelectCount는 SelectNumber 배열에 채워준 값의 개수가 된다.
        int RandIndex = rand() % SelectCount;
        Input = SelectNumber[RandIndex];

        std::cout << "AISelect : " << Input << std::endl;

        //system("pause");

        // AI가 선택한 숫자로 플레이어의 숫자중 찾아서 *로 바꿔준다.
        for (int i = 0; i < 25; ++i)
        {
            if (Number[i] == Input)
            {
                Number[i] = INT_MAX;
                break;
            }
        }

        // AI가 선택한 숫자로 AI의 숫자중 찾아서 *로 바꿔준다.
        for (int i = 0; i < 25; ++i)
        {
            if (AINumber[i] == Input)
            {
                AINumber[i] = INT_MAX;
                break;
            }
        }


        // 빙고 줄 수를 체크한다.
        // 가로 줄을 체크한다.
        int Check1 = 0, Check2 = 0;
        int AICheck1 = 0, AICheck2 = 0;

        // 체크 전에 빙고 수를 0으로 초기화 하고 모든 줄에 대해서 줄수를 다시 체크해주도록 한다.
        Bingo = 0;
        AIBingo = 0;

        for (int i = 0; i < 5; ++i)
        {
            // 한줄 체크하기 전에 0으로 초기화를 해주고 그 줄을 체크하도록 한다.
            Check1 = 0;

            // Check2는 세로 줄을 체크하기 위한 변수이다.
            Check2 = 0;

            AICheck1 = 0;
            AICheck2 = 0;

            for (int j = 0; j < 5; ++j)
            {
                if (Number[i * 5 + j] == INT_MAX)
                    ++Check1;

                if (Number[j * 5 + i] == INT_MAX)
                    ++Check2;

                if (AINumber[i * 5 + j] == INT_MAX)
                    ++AICheck1;

                if (AINumber[j * 5 + i] == INT_MAX)
                    ++AICheck2;
            }

            // 한 줄이 끝났을 경우 위에서 체크해놓은 Check1이 5라면 그 줄은 모두 *로 변환된 줄이다.
            if (Check1 == 5)
                ++Bingo;

            if (Check2 == 5)
                ++Bingo;

            if (AICheck1 == 5)
                ++AIBingo;

            if (AICheck2 == 5)
                ++AIBingo;
        }

        // 대각선 체크
        Check1 = 0;
        AICheck1 = 0;
        for (int i = 0; i < 25; i += 6)
        {
            if (Number[i] == INT_MAX)
                ++Check1;

            if (AINumber[i] == INT_MAX)
                ++AICheck1;
        }

        if (Check1 == 5)
            ++Bingo;

        if (AICheck1 == 5)
            ++AIBingo;

        Check1 = 0;
        AICheck1 = 0;
        for (int i = 4; i <= 20; i += 4)
        {
            if (Number[i] == INT_MAX)
                ++Check1;

            if (AINumber[i] == INT_MAX)
                ++AICheck1;
        }

        if (Check1 == 5)
            ++Bingo;

        if (AICheck1 == 5)
            ++AIBingo;
    }

    return 0;
}
