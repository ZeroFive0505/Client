
#include <iostream>
#include <time.h>

void SetNumber(int* pArray)
{
    for (int i = 0; i < 25; ++i)
    {
        pArray[i] = i + 1;
    }
}

void Shuffle(int* pArray)
{
    for (int i = 0; i < 100; ++i)
    {
        int Index1 = rand() % 25;
        int Index2 = rand() % 25;

        int Temp = pArray[Index1];
        pArray[Index1] = pArray[Index2];
        pArray[Index2] = Temp;
    }
}

void Init(int* pPlayer, int* pAI)
{
    SetNumber(pPlayer);
    SetNumber(pAI);

    // 섞어준다.
    Shuffle(pPlayer);
    Shuffle(pAI);
}

void OutputNumber(int* pArray, const char* pName, int BingoCount)
{
    std::cout << "====================== " << pName << " ======================" << std::endl << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            // INT_MAX 는 int타입으로 표현할 수 있는 최대값이 상수로 정해져 있는 것이다.
            if (pArray[i * 5 + j] == INT_MAX)
                std::cout << "*\t";

            else
                std::cout << pArray[i * 5 + j] << "\t";
        }

        // 가로 한줄이 출력이 완료되었다면 개행한다.
        std::cout << std::endl;
    }

    std::cout << pName << " Bingo : " << BingoCount << std::endl;
    std::cout << std::endl;
}

bool ChangeNumber(int* pArray, int Number)
{
    for (int i = 0; i < 25; ++i)
    {
        if (pArray[i] == Number)
        {
            pArray[i] = INT_MAX;
            return true;
        }
    }

    // 여기 온 경우라면 이미 입력한 숫자를 다시 중복해서 입력한 것이므로
    // false를 리턴한다.
    return false;
}

int SelectAI(int* pAI)
{
    int SelectNumber[25] = {};
    int SelectCount = 0;

    // AI가 하나 선택한다. 단, 현재 입력이 안된 숫자중 하나를 랜덤하게 선택해야 한다.
    // 선택을 위해서 *이 아닌 일반 숫자들을 배열에 넣어준다.
    SelectCount = 0;
    for (int i = 0; i < 25; ++i)
    {
        if (pAI[i] != INT_MAX)
        {
            SelectNumber[SelectCount] = pAI[i];
            ++SelectCount;
        }
    }

    // 여기로 빠져나오면 SelectCount는 SelectNumber 배열에 채워준 값의 개수가 된다.
    int RandIndex = rand() % SelectCount;
    return SelectNumber[RandIndex];
}

int CheckBingoCount(int* pArray)
{
    // 빙고 줄 수를 체크한다.
    // 가로 줄을 체크한다.
    int Check1 = 0, Check2 = 0;
    // 체크 전에 빙고 수를 0으로 초기화 하고 모든 줄에 대해서 줄수를 다시 체크해주도록 한다.
    int Bingo = 0;

    for (int i = 0; i < 5; ++i)
    {
        // 한줄 체크하기 전에 0으로 초기화를 해주고 그 줄을 체크하도록 한다.
        Check1 = 0;

        // Check2는 세로 줄을 체크하기 위한 변수이다.
        Check2 = 0;

        for (int j = 0; j < 5; ++j)
        {
            if (pArray[i * 5 + j] == INT_MAX)
                ++Check1;

            if (pArray[j * 5 + i] == INT_MAX)
                ++Check2;
        }

        // 한 줄이 끝났을 경우 위에서 체크해놓은 Check1이 5라면 그 줄은 모두 *로 변환된 줄이다.
        if (Check1 == 5)
            ++Bingo;

        if (Check2 == 5)
            ++Bingo;
    }

    // 대각선 체크
    Check1 = 0;
    for (int i = 0; i < 25; i += 6)
    {
        if (pArray[i] == INT_MAX)
            ++Check1;
    }

    if (Check1 == 5)
        ++Bingo;

    Check1 = 0;
    for (int i = 4; i <= 20; i += 4)
    {
        if (pArray[i] == INT_MAX)
            ++Check1;
    }

    if (Check1 == 5)
        ++Bingo;

    return Bingo;
}

void Run(int* pPlayer, int* pAI)
{
    int Bingo = 0, AIBingo = 0;

    while (true)
    {
        system("cls");

        // 플레이어 번호판 출력
        OutputNumber(pPlayer, "Player", Bingo);

        // AI 번호판 출력
        OutputNumber(pAI, "AI", AIBingo);

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

        int Input = 0;

        std::cout << "Input Number(0 : Exit) : ";
        std::cin >> Input;

        if (Input == 0)
            break;

        // 1 ~ 25 사이의 숫자가 아니라면 다시 입력하게 한다.
        else if (Input < 0 || Input > 25)
            continue;

        // ChangeNumber 함수의 리턴값이 false라면 이미 입력된 숫자를 다시 입력한
        // 것이므로 숫자 자체를 다시 입력받도록 한다.
        if (ChangeNumber(pPlayer, Input) == false)
            continue;

        ChangeNumber(pAI, Input);

        Input = SelectAI(pAI);

        std::cout << "AISelect : " << Input << std::endl;

        //system("pause");

        // AI가 선택한 숫자로 플레이어의 숫자중 찾아서 *로 바꿔준다.
        ChangeNumber(pPlayer, Input);

        ChangeNumber(pAI, Input);

        Bingo = CheckBingoCount(pPlayer);
        AIBingo = CheckBingoCount(pAI);
    }
}

int main()
{
    srand((unsigned int)time(0));
    rand();

    int Number[25] = {};
    int AINumber[25] = {};

    Init(Number, AINumber);

    Run(Number, AINumber);

    return 0;
}
