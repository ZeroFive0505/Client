#include <iostream>

/*
* 1 - 1
* ***** 0
* ****  1
* ***   2
* **    3
* *     4
* 
* 1 - 2
*     * 0
*    ** 1
*   *** 2
*  **** 3
* ***** 4
* 
* 1 - 3
* ***** 0
*  **** 1
*   *** 2
*    ** 3
*     * 4
* 
* 
* 1 - 4
*    *     0   빈칸 3 별 1
*   ***    1   빈칸 2 별 3
*  *****   2   빈칸 1 별 5
* *******  3   빈칸 0 별 7
*  *****   4   빈칸 1 별 5
*   ***    5   빈칸 2 별 3
*    *     6   빈칸 3 별 1
*/

int main()
{
	/*
	* 반복문: 어떤 코드를 반복해서 동작시켜주고자 할 때 사용하는 문법이 반복문이다.
	* 반복문은 3가지 종류가 지원된다.
	* for, while, do while 3가지가 있다.
	* for(초기값; 조건식; 증감값) 
	* {
	*	반복시키고자 하는 코드
	* }
	* 
	* 초기값: for문에 진입할때 처음 1번만 동작이 되는 구문이다.
	* for문에서 사용할 값을 초기화 시켜주는 용도로 사용된다.
	* 
	* 조건식: 이 for문이 동작되는동안 계속해서 체크되는 구문이다.
	* 조건식이 true가 되면 for문이 동작되고 false가 되면 for문을 빠져나가게 된다.
	* 
	* 증감값: 이 for문이 동작되는 동안 계속해서 처리된다.
	* for문에서 사용하는 값을 증가 혹은 감소를 시켜줄 경우 사용할 수 있다.
	* 
	* for문의 실행순서 : 초기값 -> 조건식(true일 경우) -> 코드 -> 증감값 -> 조건식(true일 경우)
	* -> 코드 -> 증감값 -> 조건식 -> 코드 -> 증감값 -> 조건식... -> 조건식(false일 경우) for 문 종료
	*/

	// 초기값에서 변수를 선언할 경우 해당 변수는 이 for문 안에서만 사용이 가능하고 for문을 빠져나가면 사용이 불가
	//for (int i = 0; i < 10; i++)
	//{
	//	std::cout << "i : " << i << "\n";
	//}

	int num = 0;

	//for (int i = 0, j = 0; i < 100 && j == 0; i += 2, num+=100)
	//{
	//	std::cout << "i : " << i << "\n";
	//	std::cout << "j : " << j << "\n";
	//}

	//std::cout << "num : " << num << "\n";

	//for (int i = 1; i < 100; i++)
	//{
	//	// if문을 이용해서 3의 배수인지를 체크하여 3의 배수일 경우 출력하는 기능을 작성한다.
	//	if (i % 3 == 0 && i % 5 == 0)
	//		std::cout << "i : " << i << "\n";
	//}

	//for (int i = 0; i < 100; i++)
	//{
	//	if (i == 10)
	//		continue; // for문의 증감값으로 이동시킨다.
	//	else if (i == 80) 
	//		break; // for문을 종료시킨다.

	//	std::cout << "i : " << i << std::endl;
	//}

	//// for문을 이용하여 구구단 2단을 출력해보자...
	//for (int i = 1; i < 10; i++)
	//{
	//	std::cout << "2 * " << i << " : " << 2 * i << "\n";
	//}

	/*
	* 중첩 for문: for문 안에 또 도란 for문이 들어가는 형태
	*/

	/*for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (j == 3)
				break;

			std::cout << "i : " << i << ", j : " << j << "\n";
		}
	}*/


	// 중첩 for문을 활용해서 구구단을 2 - 9단까지 출력할 수 있는 for문을 작성한다.
	for (int i = 2; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			std::cout << i << " * " << j << " : " << i * j << "\n";
		}
	}

	/*
	* while문: 반북문의 한 종류이다.
	* while(조건식)
	* {
	*		반복될 코드
	* }
	* 
	* 주로 무한루프를 돌리거나. 특정 조건이 체크될때까지 계속해서 반복되어야 할 경우
	*/

	//while (true)
	//{
	//	// 여기에 break가 없다면 이 프로그램은 종료가 안되고 계속해서 반복하게 된다.
	//}

	//bool loop = true;

	//while (loop)
	//{
	//	// 여기에서 Loop를 false로 바꿔주면 while문이 종료된다.
	//	loop = false;
	//}

	/*
	* do while문: 반복문의 한 종류이다.
	* do
	* {
	*		반복할 코드
	* }while(조건식);
	* 
	* 처음 1번은 무조건 동작이 되고 그 뒤부터 조건식을 체크하여 true일 경우 동작되는 방식이다.
	*/

	/*
	* 신나는 별찍기
	* 
	* *
	* **
	* ***
	* ****
	* *****
	* 
	* 1. 5줄로 구성된다.
	* 2. *이 출력되는 개수가 1개씩 증가된다.
	* 3. *이 다 출력되면 한 줄을 마무리하기 위해 \n이 들어간다.
	*/

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			std::cout << "*";
		}
		std::cout << "\n";
	}

	/*
	*    *
	*   ***
	*  *****
	* *******
	* 1. 4줄이다.
	* 2. *은 각 줄마다 2개씩 증가하게된다.
	* 3. *이 다 출력되면 한 줄을 마무리하기 위해 \n이 들어간다.
	* 4. 각 줄별로 공백이 3, 2, 1, 0칸이 들어가게 된다.
	*/

	const int ROW = 10;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = i; j < ROW - 1; j++)
			std::cout << " ";

		// *은 1, 3, 5, 7로 출력해야한다.
		for (int j = 0; j < (i * 2) + 1; j++)
			std::cout << "*";

		std::cout << "\n";
	}

	return 0;
}