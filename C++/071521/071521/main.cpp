
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

    10��¥���� �迭�� *�� �ƴ� ����
    �־��ش�.
    1, 2, 3, 5, 6, 7, 10

    �� 7���� ���� 7���� ������ �ε����� ���ؼ� �� ���� �����ϰ� ���ش�.
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
                // INT_MAX �� intŸ������ ǥ���� �� �ִ� �ִ밪�� ����� ������ �ִ� ���̴�.
                if (Number[i * 5 + j] == INT_MAX)
                    std::cout << "*\t";

                else
                    std::cout << Number[i * 5 + j] << "\t";
            }

            // ���� ������ ����� �Ϸ�Ǿ��ٸ� �����Ѵ�.
            std::cout << std::endl;
        }

        std::cout << "Bingo : " << Bingo << std::endl;
        std::cout << std::endl;

        std::cout << "====================== AI ======================" << std::endl << std::endl;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                // INT_MAX �� intŸ������ ǥ���� �� �ִ� �ִ밪�� ����� ������ �ִ� ���̴�.
                if (AINumber[i * 5 + j] == INT_MAX)
                    std::cout << "*\t";

                else
                    std::cout << AINumber[i * 5 + j] << "\t";
            }

            // ���� ������ ����� �Ϸ�Ǿ��ٸ� �����Ѵ�.
            std::cout << std::endl;
        }

        int Input = 0;
        std::cout << "AIBingo : " << AIBingo << std::endl;

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

        std::cout << "Input Number(0 : Exit) : ";
        std::cin >> Input;

        if (Input == 0)
            break;

        // 1 ~ 25 ������ ���ڰ� �ƴ϶�� �ٽ� �Է��ϰ� �Ѵ�.
        else if (Input < 0 || Input > 25)
            continue;

        // �Է¹��� ���ڸ� ã�Ƽ� *�� �ٲ��ش�.
        // ��, �̹� *�� �ٲ����� ���ڸ� �ٽ� �Է��ϸ�
        // �ٽ� ó������ �Է¹޵��� �Ѵ�.
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

        // ���� ��ã�Ҵٸ� �ٽ� �Է¹ް� �Ѵ�.
        if (Find == false)
            continue;

        // AI�� ������ڿ��� �÷��̾ �Է��� ���� ã�Ƽ� *�� �ٲ��ش�.
        for (int i = 0; i < 25; ++i)
        {
            if (AINumber[i] == Input)
            {
                AINumber[i] = INT_MAX;
                break;
            }
        }

        // AI�� �ϳ� �����Ѵ�. ��, ���� �Է��� �ȵ� ������ �ϳ��� �����ϰ� �����ؾ� �Ѵ�.
        // ������ ���ؼ� *�� �ƴ� �Ϲ� ���ڵ��� �迭�� �־��ش�.
        SelectCount = 0;
        for (int i = 0; i < 25; ++i)
        {
            if (AINumber[i] != INT_MAX)
            {
                SelectNumber[SelectCount] = AINumber[i];
                ++SelectCount;
            }
        }

        // ����� ���������� SelectCount�� SelectNumber �迭�� ä���� ���� ������ �ȴ�.
        int RandIndex = rand() % SelectCount;
        Input = SelectNumber[RandIndex];

        std::cout << "AISelect : " << Input << std::endl;

        //system("pause");

        // AI�� ������ ���ڷ� �÷��̾��� ������ ã�Ƽ� *�� �ٲ��ش�.
        for (int i = 0; i < 25; ++i)
        {
            if (Number[i] == Input)
            {
                Number[i] = INT_MAX;
                break;
            }
        }

        // AI�� ������ ���ڷ� AI�� ������ ã�Ƽ� *�� �ٲ��ش�.
        for (int i = 0; i < 25; ++i)
        {
            if (AINumber[i] == Input)
            {
                AINumber[i] = INT_MAX;
                break;
            }
        }


        // ���� �� ���� üũ�Ѵ�.
        // ���� ���� üũ�Ѵ�.
        int Check1 = 0, Check2 = 0;
        int AICheck1 = 0, AICheck2 = 0;

        // üũ ���� ���� ���� 0���� �ʱ�ȭ �ϰ� ��� �ٿ� ���ؼ� �ټ��� �ٽ� üũ���ֵ��� �Ѵ�.
        Bingo = 0;
        AIBingo = 0;

        for (int i = 0; i < 5; ++i)
        {
            // ���� üũ�ϱ� ���� 0���� �ʱ�ȭ�� ���ְ� �� ���� üũ�ϵ��� �Ѵ�.
            Check1 = 0;

            // Check2�� ���� ���� üũ�ϱ� ���� �����̴�.
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

            // �� ���� ������ ��� ������ üũ�س��� Check1�� 5��� �� ���� ��� *�� ��ȯ�� ���̴�.
            if (Check1 == 5)
                ++Bingo;

            if (Check2 == 5)
                ++Bingo;

            if (AICheck1 == 5)
                ++AIBingo;

            if (AICheck2 == 5)
                ++AIBingo;
        }

        // �밢�� üũ
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
