
#include <iostream>
#include <time.h>

/*
210714 숙제
빙고 만들어오기
1 ~ 25 까지의 숫자를 골고루 섞어서
5 x 5로 출력되게 해온다.
예)
1   2   3   4   5
6   7   8   9   10
11  12  13  14  15
16  17  18  19  20
21  22  23  24  25

이렇게 출력하되 숫자는 섞어준다.

플레이어는 숫자 1개를 입력한다.
입력하면 해당 숫자는 *로 출력이 되게 만들어보자.
예를 들어 14를 입력했다면
1   2   3   4   5
6   7   8   9   10
11  12  13  *  15
16  17  18  19  20
21  22  23  24  25
*/

/*
1 ~ 9 사이의 랜덤한 값 3개를 얻어오나.
값은 중복되면 안된다.
플레이어는 3개의 숫자를 입력하여 숫자를 맞추는 게임이다.
만약 숫자가 같고 자리수까지 같으면 Strike, 자리수가 다르면 Ball을 출력한다.
같은게 없다면 Out을 출력한다.
맞춰야할 값 : 7 2 8
입력 : 1 3 4
같은게 없으므로 OUt 출력

입력 : 7 5 6
7은 숫자도 같고 자리도 같으므로 Strike 5, 6은 같은게 없으므로 최종 1Strike 0Ball이 된다.
입력 : 7 8 2
7은 Strike , 8, 2는 숫자는 같지만 자리가 다르므로 Ball이 되어 1Strike 2Ball이 된다.
모두 맞추면 게임이 종료된다.
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

        // 입력한 숫자를 맞춰야할 숫자와 비교한다.
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
            std::cout << "모두 맞췄습니다." << std::endl;
            break;
        }

        else
            std::cout << Strike << " Strike " << Ball << " Ball" << std::endl;
    }

    return 0;
}
