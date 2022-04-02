#include <iostream>

// time.h: 시간에 관련된 기능을 제공해주는 헤더파일이다.
#include <time.h>

enum class SRP
{
	S = 1,
	R,
	P,
};

int main()
{
	// time(0)을 하면 시간을 얻어올 수 있는데 1초가 지나면 1이 증가된 값을 얻어오게 된다.
	// 1970년 1월 1일부터 1씩 증가하는 값.
	// rand(): 랜덤한 값을 얻어온다. 0 ~ 32767 사이로 얻어오게 된다.
	// rand()를 이용해서 랜덤한 값을 얻어올때 내부적으로 랜덤한 값이 들어가 있는 표를 이용해서 얻어온다고
	// 볼 수 있다.
	// 그런데 표를 어떤 표를 사용할 것이다 라고 지정한게 아닌 고정된 표에서
	// 랜덤한 값을 얻어오므로 계속 같은 값이 나오게 되는 것이다.
	// 그래서 표를 실행할때마다 다른 표로 가져올 수 있기 만들어주어야한다.
	// rand()는 특이하게 처음값은 증가하는 순으로 출력된다.. 따라서 버리는 것이 좋다.
	srand(time(0));
	rand();

	int player = 0;
	while (1)
	{
		// cls 콘솔명령을 입력해서 창을 모두 지워준다.
		system("cls");
		std::cout << "1. Scissors" << "\n";
		std::cout << "2. Rock" << "\n";
		std::cout << "3. Paper" << "\n";
		std::cout << "4. Exit" << "\n";
		std::cout << "Your choice : ";
		std::cin >> player;

		if (player == 4)
			break;

		int ai = rand() % 3 + 1;
		SRP aiRSP = (SRP)ai;
		SRP playerRSP = (SRP)player;
		switch (aiRSP)
		{
		case SRP::R:
			std::cout << "AI : Rock \n" << "\n";
			break;
		case SRP::S:
			std::cout << "AI : Scissors \n" << "\n";
			break;
		case SRP::P:
			std::cout << "AI : Paper \n" << "\n";
			break;
		default:
			break;
		}
		
		switch (playerRSP)
		{
		case SRP::R:
			std::cout << "Player : Rock \n" << "\n";
			break;
		case SRP::S:
			std::cout << "Player : Scissors \n" << "\n";
			break;
		case SRP::P:
			std::cout << "Player : Paper \n" << "\n";
			break;
		default:
			break;
		}

		int result = player - ai;

		if (result == 1 || result == -2)
			std::cout << "Player win\n";
		else if (result == 0)
			std::cout << "Draw\n";
		else
			std::cout << "Lose\n";

		// 콘솔창을 일시 정지.
		system("pause");
	}

	// 0 ~ 32767
	// 0 ~ 100 사이의 난수를 얻어온다.
	// 100 ~ 200 사이의 난수를 얻어온다.
	// 최소 공격력 ~ 최대 공격력
	// 100 ~ 400 난수 % (최대 - 최소 + 1) + 최소.
	// 확률을 소수점 2자리까지 구해보자. 100.00
	/*std::cout << rand() % 10001 / (double)100 << "\n";
	std::cout << rand() % 101 + 100  << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";
	std::cout << rand() << "\n";*/

	/*__int64 start = time(0);

	int number = 1;

	for (int i = 0; i < 10000000; i++)
	{
		number *= -1;
	}

	__int64 end = time(0);
	
	std::cout << "Time : " << end - start << "\n";


	start = time(0);

	number = 1;

	for (int i = 0; i < 10000000; i++)
	{
		number = ~number + 1;
	}

	end = time(0);
	std::cout << "Time : " << end - start << "\n";*/


	return 0;
}