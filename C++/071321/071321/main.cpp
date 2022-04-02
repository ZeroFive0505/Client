
#include <iostream>

int main()
{
    /*
    1 - 1
    *****
    ****
    ***
    **
    *
    */
    std::cout << "1 - 1" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 5; j > i; --j)
        {
            std::cout << "*";
        }

        std::cout << std::endl;
    }

    /*
    1 - 2
        *
       **
      ***
     ****
    *****
    */
    std::cout << "1 - 2" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 4; j > i; --j)
        {
            std::cout << " ";
        }

        for (int j = 0; j <= i; ++j)
        {
            std::cout << "*";
        }

        std::cout << std::endl;
    }

    /*
    1 - 3
    *****
     ****
      ***
       **
        *
    */
    std::cout << "1 - 3" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            std::cout << " ";
        }

        for (int j = 5; j > i; --j)
        {
            std::cout << "*";
        }

        std::cout << std::endl;
    }

    /*
    1 - 4
       *
      ***
     *****
    *******
     *****
      ***
       *
    공백 : 3, 2, 1, 0, 1, 2, 3
    별 : 1, 3, 5, 7, 5, 3, 1

    줄번호 0, 6
    줄번호 1, 5
    줄번호 2, 4
    */
    std::cout << "1 - 4" << std::endl;

    int Line = 7;
    int LineNumber = 0;

    for (int i = 0; i < 7; ++i)
    {
        if (i <= 3)
            LineNumber = i;

        else
            LineNumber = 6 - i;

        for (int j = LineNumber; j < 3; ++j)
        {
            std::cout << " ";
        }

        for (int j = 0; j < LineNumber * 2 + 1; ++j)
        {
            std::cout << "*";
        }

        std::cout << std::endl;
    }

    return 0;
}
