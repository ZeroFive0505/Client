
#include <iostream>
#include <time.h>

int main()
{
    // 이거 그대로 쓰면 warning이 생겨서 unsigned int로 형변환을 해준다.
    srand((unsigned int)time(0));
    rand();

    /*
    1 ~ 45 까지의 숫자중 랜덤한 6개의 숫자를 얻어와야 한다.
    단, 숫자들은 중복되면 안된다.

    이경우 먼저 1 ~ 45까지 숫자를 차례대로 넣어줄 배열을 만들고 해당 배열에 값을 넣어준 후에
    배열의 값들을 골고루 섞어주면 된다.
    */
    int Number[45] = {};

    for (int i = 0; i < 45; ++i)
    {
        Number[i] = i + 1;
    }

    // 배열에 있는 값들을 섞어준다.
    for (int i = 0; i < 100; ++i)
    {
        // 0 ~ 44 사이의 랜덤한 인덱스 2개를 얻어오고 그 2개의 위치에 있는 값을
        // 서로 바꿔준다.
        int Index1 = rand() % 45;
        int Index2 = rand() % 45;

        // 값을 바꿔준다.
        int Temp = Number[Index1];
        Number[Index1] = Number[Index2];
        Number[Index2] = Temp;
    }

    // 당첨번호 6개를 받아온다.
    int Result[6] = {};

    std::cout << "당첨번호 : ";
    for (int i = 0; i < 6; ++i)
    {
        Result[i] = Number[i];
    }

    // 얻어온 당첨번호를 정렬한다.
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
        // 배열에 있는 값들을 섞어준다.
        for (int i = 0; i < 100; ++i)
        {
            // 0 ~ 44 사이의 랜덤한 인덱스 2개를 얻어오고 그 2개의 위치에 있는 값을
            // 서로 바꿔준다.
            int Index1 = rand() % 45;
            int Index2 = rand() % 45;

            // 값을 바꿔준다.
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
            // \t : Tab 해주는 기능이다.
            std::cout << Number[i] << "\t";
        }

        std::cout << "/ " << Number[6] << " : ";

        // 당첨번호와 비교하여 몇개가 같은지를 판단한다.
        int Pair = 0;

        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                // i번째의 값과 당첨번호 6개중 같은게 있는지를 판단한다.
                if (Number[i] == Result[j])
                {
                    ++Pair;

                    // 뒤의 번호와 비교할 필요가 없기때문에 break를 통해 j for문을 빠져나간다.
                    break;
                }
            }
        }

        switch (Pair)
        {
        case 6:
            std::cout << "1등" << std::endl;
            ++Grade[0];
            break;
        case 5:
        {
            // case 안에서 변수를 선언과 동시에 초기화 할 경우 에러가 발생하므로
            // 코드블록으로 묶어준다.
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
                std::cout << "2등" << std::endl;
            }

            else
            {
                ++Grade[2];
                std::cout << "3등" << std::endl;
            }
        }
        break;
        case 4:
            ++Grade[3];
            std::cout << "4등" << std::endl;
            break;
        case 3:
            ++Grade[4];
            std::cout << "5등" << std::endl;
            break;
        default:
            ++Grade[5];
            std::cout << "꽝" << std::endl;
            break;
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        std::cout << i + 1 << "등 : " << Grade[i] << std::endl;
    }

    std::cout << "꽝 : " << Grade[5] << std::endl;


    //int Number[6] = {};

    //for (int i = 0; i < 6; ++i)
    //{
    //    Number[i] = rand() % 45 + 1;

    //    bool    Acc = false;

    //    // 중복된 숫자가 있는지 체크한다.
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
