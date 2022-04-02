#include <iostream>

// time.h: �ð��� ���õ� ����� �������ִ� ��������̴�.
#include <time.h>

enum class SRP
{
	S = 1,
	R,
	P,
};

int main()
{
	// time(0)�� �ϸ� �ð��� ���� �� �ִµ� 1�ʰ� ������ 1�� ������ ���� ������ �ȴ�.
	// 1970�� 1�� 1�Ϻ��� 1�� �����ϴ� ��.
	// rand(): ������ ���� ���´�. 0 ~ 32767 ���̷� ������ �ȴ�.
	// rand()�� �̿��ؼ� ������ ���� ���ö� ���������� ������ ���� �� �ִ� ǥ�� �̿��ؼ� ���´ٰ�
	// �� �� �ִ�.
	// �׷��� ǥ�� � ǥ�� ����� ���̴� ��� �����Ѱ� �ƴ� ������ ǥ����
	// ������ ���� �����Ƿ� ��� ���� ���� ������ �Ǵ� ���̴�.
	// �׷��� ǥ�� �����Ҷ����� �ٸ� ǥ�� ������ �� �ֱ� ������־���Ѵ�.
	// rand()�� Ư���ϰ� ó������ �����ϴ� ������ ��µȴ�.. ���� ������ ���� ����.
	srand(time(0));
	rand();

	int player = 0;
	while (1)
	{
		// cls �ָܼ���� �Է��ؼ� â�� ��� �����ش�.
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

		// �ܼ�â�� �Ͻ� ����.
		system("pause");
	}

	// 0 ~ 32767
	// 0 ~ 100 ������ ������ ���´�.
	// 100 ~ 200 ������ ������ ���´�.
	// �ּ� ���ݷ� ~ �ִ� ���ݷ�
	// 100 ~ 400 ���� % (�ִ� - �ּ� + 1) + �ּ�.
	// Ȯ���� �Ҽ��� 2�ڸ����� ���غ���. 100.00
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