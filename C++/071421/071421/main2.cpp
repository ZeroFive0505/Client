
#include <iostream>
#include <time.h>

/*
210714 ����
���� ��������
1 ~ 25 ������ ���ڸ� ���� ���
5 x 5�� ��µǰ� �ؿ´�.
��)
1   2   3   4   5
6   7   8   9   10
11  12  13  14  15
16  17  18  19  20
21  22  23  24  25

�̷��� ����ϵ� ���ڴ� �����ش�.

�÷��̾�� ���� 1���� �Է��Ѵ�.
�Է��ϸ� �ش� ���ڴ� *�� ����� �ǰ� ������.
���� ��� 14�� �Է��ߴٸ�
1   2   3   4   5
6   7   8   9   10
11  12  13  *  15
16  17  18  19  20
21  22  23  24  25
*/

/*
1 ~ 9 ������ ������ �� 3���� ������.
���� �ߺ��Ǹ� �ȵȴ�.
�÷��̾�� 3���� ���ڸ� �Է��Ͽ� ���ڸ� ���ߴ� �����̴�.
���� ���ڰ� ���� �ڸ������� ������ Strike, �ڸ����� �ٸ��� Ball�� ����Ѵ�.
������ ���ٸ� Out�� ����Ѵ�.
������� �� : 7 2 8
�Է� : 1 3 4
������ �����Ƿ� OUt ���

�Է� : 7 5 6
7�� ���ڵ� ���� �ڸ��� �����Ƿ� Strike 5, 6�� ������ �����Ƿ� ���� 1Strike 0Ball�� �ȴ�.
�Է� : 7 8 2
7�� Strike , 8, 2�� ���ڴ� ������ �ڸ��� �ٸ��Ƿ� Ball�� �Ǿ� 1Strike 2Ball�� �ȴ�.
��� ���߸� ������ ����ȴ�.
*/

int main()
{
    srand((unsigned int)time(0));
    rand();

    int Number[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int i = 0; i < 100; ++i)
    {
        int Index1 = rand() % 9;
        int Index2 = rand() % 9;

        int Temp = Number[Index1];
        Number[Index1] = Number[Index2];
        Number[Index2] = Temp;
    }

    std::cout << Number[0] << ", " << Number[1] << ", " << Number[2] << std::endl;

    int Input[3];

    while (true)
    {
        std::cout << "Input Number(0 : Exit) : ";

        bool    Exit = false;

        for (int i = 0; i < 3; ++i)
        {
            std::cin >> Input[i];

            if (Input[i] == 0)
            {
                Exit = true;
                break;
            }
        }

        if (Exit)
            break;

        int Strike = 0, Ball = 0;

        // �Է��� ���ڸ� ������� ���ڿ� ���Ѵ�.
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (Number[i] == Input[j])
                {
                    if (i == j)
                        ++Strike;

                    else
                        ++Ball;

                    break;
                }
            }
        }

        if (Strike == 0 && Ball == 0)
            std::cout << "Out" << std::endl;

        else if (Strike == 3)
        {
            std::cout << "��� ������ϴ�." << std::endl;
            break;
        }

        else
            std::cout << Strike << " Strike " << Ball << " Ball" << std::endl;
    }

    return 0;
}
