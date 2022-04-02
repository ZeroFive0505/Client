#include <iostream>
#include <time.h>

enum class BINGO
{
	EMPTY,
	CHECK,
	COMPLETE
};

int main()
{
	const int HEIGHT = 5;
	const int WIDTH = 5;
	const int SHUFFLEAMOUNT = 100;
	int bingo[HEIGHT][WIDTH];
	int deck[HEIGHT * WIDTH];
	BINGO checker[HEIGHT * WIDTH + 1];

	for (int i = 0; i < HEIGHT * WIDTH; i++)
	{
		deck[i] = i + 1;
	}

	for (int i = 0; i < HEIGHT * WIDTH + 1; i++)
	{
		checker[i] = BINGO::EMPTY;
	}

	srand((unsigned int)time(0));
	rand();

	for (int i = 0; i < SHUFFLEAMOUNT; i++)
	{
		int idx1 = rand() % (WIDTH * HEIGHT);
		int idx2 = rand() % (WIDTH * HEIGHT);

		int temp = deck[idx1];
		deck[idx1] = deck[idx2];
		deck[idx2] = temp;
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			bingo[i][j] = deck[i * WIDTH + j];
		}
	}

	int input;

	int bingoCount = 0;
	bool leftDiagnol = false;
	bool rightDiagnol = false;
	bool gameLoop = true;
	while (gameLoop)
	{

		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				if (checker[bingo[i][j]] == BINGO::CHECK)
					std::cout << "*" << "\t";
				else if (checker[bingo[i][j]] == BINGO::COMPLETE)
					std::cout << "#" << "\t";
				else
					std::cout << bingo[i][j] << "\t";
			}

			std::cout << "\n";
		}

		std::cout << "현재 빙고 수 : " << bingoCount << "\n";

		do
		{
			std::cout << "Input : ";
			std::cin >> input;

			if (input < 1 || input > 25)
				std::cout << "유효하지 않은 범위.\n";
			else
			{
				if (checker[input] == BINGO::CHECK || checker[input] == BINGO::COMPLETE)
					std::cout << "이미 입력한 숫자.\n";
			}

		} while (input < 1 || input > 25 || checker[input] != BINGO::EMPTY);

		checker[input] = BINGO::CHECK;

		int y = -1;
		int x = -1;
		bool found = false;

		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				if (bingo[i][j] == input)
				{
					y = i;
					x = j;
					found = true;
					break;
				}
			}

			if (found)
				break;
		}

		if (y == -1 || x == -1)
		{
			std::cout << "Error\n";
			break;
		}

		// 해당 좌표에서 가로 빙고 체크
		int cnt = 0;
		for (int i = 0; i < WIDTH; i++)
		{
			if (checker[bingo[y][i]] == BINGO::CHECK || checker[bingo[y][i]] == BINGO::COMPLETE)
				cnt++;
		}

		if (cnt == WIDTH)
		{
			for (int i = 0; i < WIDTH; i++)
			{
				checker[bingo[y][i]] = BINGO::COMPLETE;
			}

			std::cout << "BINGO!\n";
			bingoCount++;
		}

		// 해당 좌표에서 세로 빙고 체크
		cnt = 0;
		for (int i = 0; i < HEIGHT; i++)
		{
			if (checker[bingo[i][x]] == BINGO::CHECK || checker[bingo[i][x]] == BINGO::COMPLETE)
				cnt++;
		}

		if (cnt == HEIGHT)
		{
			for (int i = 0; i < HEIGHT; i++)
			{
				checker[bingo[i][x]] = BINGO::COMPLETE;
			}

			std::cout << "BINGO!\n";

			bingoCount++;
		}

		// 대각선 체크
		if (!leftDiagnol && y == x)
		{
			cnt = 0;
			for (int i = 0; i < HEIGHT; i++)
			{
				if (checker[bingo[i][i]] == BINGO::CHECK || checker[bingo[i][i]] == BINGO::COMPLETE)
					cnt++;
			}

			if (cnt == HEIGHT)
			{
				for (int i = 0; i < HEIGHT; i++)
				{
					checker[bingo[i][i]] = BINGO::COMPLETE;
				}

				std::cout << "BINGO!\n";
				leftDiagnol = true;
				bingoCount++;
			}
		}

		if (!rightDiagnol && (y + x == WIDTH - 1))
		{
			cnt = 0;
			for (int i = 0; i < HEIGHT; i++)
			{
				if (checker[bingo[i][WIDTH - i - 1]] == BINGO::CHECK || checker[bingo[i][WIDTH - i - 1]] == BINGO::COMPLETE)
					cnt++;
			}

			if (cnt == HEIGHT)
			{
				for (int i = 0; i < HEIGHT; i++)
				{
					checker[bingo[i][WIDTH - i - 1]] = BINGO::COMPLETE;
				}

				std::cout << "BINGO!\n";
				rightDiagnol = true;
				bingoCount++;
			}
		}

		if (bingoCount == 2 * WIDTH + 2)
		{
			std::cout << "게임 끝\n";
			gameLoop = false;
		}
	}

	return 0;
}