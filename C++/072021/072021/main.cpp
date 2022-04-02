
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

    // �����ش�.
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
            // INT_MAX �� intŸ������ ǥ���� �� �ִ� �ִ밪�� ����� ������ �ִ� ���̴�.
            if (pArray[i * 5 + j] == INT_MAX)
                std::cout << "*\t";

            else
                std::cout << pArray[i * 5 + j] << "\t";
        }

        // ���� ������ ����� �Ϸ�Ǿ��ٸ� �����Ѵ�.
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

    // ���� �� ����� �̹� �Է��� ���ڸ� �ٽ� �ߺ��ؼ� �Է��� ���̹Ƿ�
    // false�� �����Ѵ�.
    return false;
}

int SelectAI(int* pAI)
{
    int SelectNumber[25] = {};
    int SelectCount = 0;

    // AI�� �ϳ� �����Ѵ�. ��, ���� �Է��� �ȵ� ������ �ϳ��� �����ϰ� �����ؾ� �Ѵ�.
    // ������ ���ؼ� *�� �ƴ� �Ϲ� ���ڵ��� �迭�� �־��ش�.
    SelectCount = 0;
    for (int i = 0; i < 25; ++i)
    {
        if (pAI[i] != INT_MAX)
        {
            SelectNumber[SelectCount] = pAI[i];
            ++SelectCount;
        }
    }

    // ����� ���������� SelectCount�� SelectNumber �迭�� ä���� ���� ������ �ȴ�.
    int RandIndex = rand() % SelectCount;
    return SelectNumber[RandIndex];
}

int CheckBingoCount(int* pArray)
{
    // ���� �� ���� üũ�Ѵ�.
    // ���� ���� üũ�Ѵ�.
    int Check1 = 0, Check2 = 0;
    // üũ ���� ���� ���� 0���� �ʱ�ȭ �ϰ� ��� �ٿ� ���ؼ� �ټ��� �ٽ� üũ���ֵ��� �Ѵ�.
    int Bingo = 0;

    for (int i = 0; i < 5; ++i)
    {
        // ���� üũ�ϱ� ���� 0���� �ʱ�ȭ�� ���ְ� �� ���� üũ�ϵ��� �Ѵ�.
        Check1 = 0;

        // Check2�� ���� ���� üũ�ϱ� ���� �����̴�.
        Check2 = 0;

        for (int j = 0; j < 5; ++j)
        {
            if (pArray[i * 5 + j] == INT_MAX)
                ++Check1;

            if (pArray[j * 5 + i] == INT_MAX)
                ++Check2;
        }

        // �� ���� ������ ��� ������ üũ�س��� Check1�� 5��� �� ���� ��� *�� ��ȯ�� ���̴�.
        if (Check1 == 5)
            ++Bingo;

        if (Check2 == 5)
            ++Bingo;
    }

    // �밢�� üũ
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

        // �÷��̾� ��ȣ�� ���
        OutputNumber(pPlayer, "Player", Bingo);

        // AI ��ȣ�� ���
        OutputNumber(pAI, "AI", AIBingo);

        if (Bingo >= 5)
        {
            std::cout << "Player �¸�" << std::endl;
            break;
        }

        else if (AIBingo >= 5)
        {
            std::cout << "AI �¸�" << std::endl;
            break;
        }

        int Input = 0;

        std::cout << "Input Number(0 : Exit) : ";
        std::cin >> Input;

        if (Input == 0)
            break;

        // 1 ~ 25 ������ ���ڰ� �ƴ϶�� �ٽ� �Է��ϰ� �Ѵ�.
        else if (Input < 0 || Input > 25)
            continue;

        // ChangeNumber �Լ��� ���ϰ��� false��� �̹� �Էµ� ���ڸ� �ٽ� �Է���
        // ���̹Ƿ� ���� ��ü�� �ٽ� �Է¹޵��� �Ѵ�.
        if (ChangeNumber(pPlayer, Input) == false)
            continue;

        ChangeNumber(pAI, Input);

        Input = SelectAI(pAI);

        std::cout << "AISelect : " << Input << std::endl;

        //system("pause");

        // AI�� ������ ���ڷ� �÷��̾��� ������ ã�Ƽ� *�� �ٲ��ش�.
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
