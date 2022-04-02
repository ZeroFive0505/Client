
#include <iostream>
#include <time.h>

int main()
{
    // �̰� �״�� ���� warning�� ���ܼ� unsigned int�� ����ȯ�� ���ش�.
    srand((unsigned int)time(0));
    rand();

    /*
    1 ~ 45 ������ ������ ������ 6���� ���ڸ� ���;� �Ѵ�.
    ��, ���ڵ��� �ߺ��Ǹ� �ȵȴ�.

    �̰�� ���� 1 ~ 45���� ���ڸ� ���ʴ�� �־��� �迭�� ����� �ش� �迭�� ���� �־��� �Ŀ�
    �迭�� ������ ���� �����ָ� �ȴ�.
    */
    int Number[45] = {};

    for (int i = 0; i < 45; ++i)
    {
        Number[i] = i + 1;
    }

    // �迭�� �ִ� ������ �����ش�.
    for (int i = 0; i < 100; ++i)
    {
        // 0 ~ 44 ������ ������ �ε��� 2���� ������ �� 2���� ��ġ�� �ִ� ����
        // ���� �ٲ��ش�.
        int Index1 = rand() % 45;
        int Index2 = rand() % 45;

        // ���� �ٲ��ش�.
        int Temp = Number[Index1];
        Number[Index1] = Number[Index2];
        Number[Index2] = Temp;
    }

    // ��÷��ȣ 6���� �޾ƿ´�.
    int Result[6] = {};

    std::cout << "��÷��ȣ : ";
    for (int i = 0; i < 6; ++i)
    {
        Result[i] = Number[i];
    }

    // ���� ��÷��ȣ�� �����Ѵ�.
    for (int Src = 0; Src < 5; ++Src)
    {
        for (int Dest = Src + 1; Dest < 6; ++Dest)
        {
            if (Result[Src] > Result[Dest])
            {
                int Temp = Result[Src];
                Result[Src] = Result[Dest];
                Result[Dest] = Temp;
            }
        }
    }

    for (int i = 0; i < 6; ++i)
    {
        std::cout << Result[i] << "\t";
    }
    std::cout << std::endl;

    int Grade[6] = {};

    for (int Game = 0; Game < 1000; ++Game)
    {
        // �迭�� �ִ� ������ �����ش�.
        for (int i = 0; i < 100; ++i)
        {
            // 0 ~ 44 ������ ������ �ε��� 2���� ������ �� 2���� ��ġ�� �ִ� ����
            // ���� �ٲ��ش�.
            int Index1 = rand() % 45;
            int Index2 = rand() % 45;

            // ���� �ٲ��ش�.
            int Temp = Number[Index1];
            Number[Index1] = Number[Index2];
            Number[Index2] = Temp;
        }

        for (int Src = 0; Src < 5; ++Src)
        {
            for (int Dest = Src + 1; Dest < 6; ++Dest)
            {
                if (Number[Src] > Number[Dest])
                {
                    int Temp = Number[Src];
                    Number[Src] = Number[Dest];
                    Number[Dest] = Temp;
                }
            }
        }

        for (int i = 0; i < 6; ++i)
        {
            // \t : Tab ���ִ� ����̴�.
            std::cout << Number[i] << "\t";
        }

        std::cout << "/ " << Number[6] << " : ";

        // ��÷��ȣ�� ���Ͽ� ��� �������� �Ǵ��Ѵ�.
        int Pair = 0;

        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                // i��°�� ���� ��÷��ȣ 6���� ������ �ִ����� �Ǵ��Ѵ�.
                if (Number[i] == Result[j])
                {
                    ++Pair;

                    // ���� ��ȣ�� ���� �ʿ䰡 ���⶧���� break�� ���� j for���� ����������.
                    break;
                }
            }
        }

        switch (Pair)
        {
        case 6:
            std::cout << "1��" << std::endl;
            ++Grade[0];
            break;
        case 5:
        {
            // case �ȿ��� ������ ����� ���ÿ� �ʱ�ȭ �� ��� ������ �߻��ϹǷ�
            // �ڵ������� �����ش�.
            bool    Second = false;
            for (int i = 0; i < 6; ++i)
            {
                if (Number[6] == Result[i])
                {
                    Second = true;
                    break;
                }
            }

            if (Second)
            {
                ++Grade[1];
                std::cout << "2��" << std::endl;
            }

            else
            {
                ++Grade[2];
                std::cout << "3��" << std::endl;
            }
        }
        break;
        case 4:
            ++Grade[3];
            std::cout << "4��" << std::endl;
            break;
        case 3:
            ++Grade[4];
            std::cout << "5��" << std::endl;
            break;
        default:
            ++Grade[5];
            std::cout << "��" << std::endl;
            break;
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        std::cout << i + 1 << "�� : " << Grade[i] << std::endl;
    }

    std::cout << "�� : " << Grade[5] << std::endl;


    //int Number[6] = {};

    //for (int i = 0; i < 6; ++i)
    //{
    //    Number[i] = rand() % 45 + 1;

    //    bool    Acc = false;

    //    // �ߺ��� ���ڰ� �ִ��� üũ�Ѵ�.
    //    for (int j = 0; j < i; ++j)
    //    {
    //        if (Number[i] == Number[j])
    //        {
    //            Acc = true;
    //         
    //            break;
    //        }
    //    }

    //    if (Acc)
    //    {
    //        --i;
    //        continue;
    //    }

    //    std::cout << Number[i] << std::endl;
    //}

    return 0;
}
