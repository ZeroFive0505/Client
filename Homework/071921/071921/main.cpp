#include <iostream>
#include <time.h>

enum class BINGO
{
	EMPTY,
	CHECK,
	COMPLETE
};

const int HEIGHT = 5;
const int WIDTH = 5;
const int N_SHUFFLE = 100;

const int WINCONDITION = 3;

void InitBingo(int board[][WIDTH], BINGO* checker);

void Shuffle(int deck[], int nSize);

void PrintBingoBoard(int board[][WIDTH], BINGO* checker, int currentBingo, bool playerTurn = true);

int Input(BINGO* player, BINGO* com, bool playerTurn = true);

void BingoCheck(int board[][WIDTH], BINGO* checker, 
	int input, int* currnetBingo, bool* leftDiagnol, bool* rightDiagnol);

int main()
{
	srand((unsigned int)time(0));
	rand();

	int playerBingo[HEIGHT][WIDTH];
	int comBingo[HEIGHT][WIDTH];
	BINGO player[HEIGHT * WIDTH + 1];
	BINGO com[HEIGHT * WIDTH + 1];

	InitBingo(playerBingo, player);
	InitBingo(comBingo, com);

	int input;

	int playerBingoCnt = 0;
	int comBingoCnt = 0;
	
	
	bool pLeftDiagnol = false;
	bool pRightDiagnol = false;

	bool cLeftDiagnol = false;
	bool cRightDiagnol = false;

	while (true)
	{
		system("cls");

		PrintBingoBoard(playerBingo, player, playerBingoCnt, true);
		PrintBingoBoard(comBingo, com, comBingoCnt, false);

		int playerInput = Input(player, com, true);
		BingoCheck(playerBingo, player, playerInput, &playerBingoCnt, &pLeftDiagnol, &pRightDiagnol);
		BingoCheck(comBingo, com, playerInput, &comBingoCnt, &cLeftDiagnol, &cRightDiagnol);


		int comInput = Input(player, com, false);
		BingoCheck(playerBingo, player, comInput, &playerBingoCnt, &pLeftDiagnol, &pRightDiagnol);
		BingoCheck(comBingo, com, comInput, &comBingoCnt, &cLeftDiagnol, &cRightDiagnol);

		PrintBingoBoard(playerBingo, player, playerBingoCnt, true);
		PrintBingoBoard(comBingo, com, comBingoCnt, false);

		if (playerBingoCnt >= WINCONDITION)
		{
			std::cout << "Player win!\n";
			break;
		}

		if (comBingoCnt >= WINCONDITION)
		{
			std::cout << "Computer win!\n";
			break;
		}

		system("pause");
	}

	return 0;
}

void InitBingo(int board[][WIDTH], BINGO* checker)
{
	int deck[WIDTH * HEIGHT];

	for (int i = 0; i < WIDTH * HEIGHT; i++)
		deck[i] = i + 1;
	
	for (int i = 0; i < WIDTH * HEIGHT + 1; i++)
		checker[i] = BINGO::EMPTY;

	Shuffle(deck, WIDTH * HEIGHT);

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			board[i][j] = deck[i * WIDTH + j];
		}
	}
}

void Shuffle(int deck[], int nSize)
{
	if (nSize < 2)
		return;

	for (int i = 0; i < N_SHUFFLE; i++)
	{
		int idx1 = rand() % nSize;
		int idx2 = rand() % nSize;

		int temp = deck[idx1];

		deck[idx1] = deck[idx2];
		deck[idx2] = temp;
	}
}

void PrintBingoBoard(int board[][WIDTH], BINGO* checker, int currentBingo, bool playerTurn)
{

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			switch (checker[board[i][j]])
			{
			case BINGO::EMPTY:
				std::cout << board[i][j] << "\t";
				break;
			case BINGO::CHECK:
				std::cout << "*" << "\t";
				break;
			case BINGO::COMPLETE:
				std::cout << "#" << "\t";
				break;
			}
		}

		std::cout << "\n";
	}

	if (playerTurn)
		std::cout << "Player current bingo : " << currentBingo << "\n";
	else
		std::cout << "Com current bingo : " << currentBingo << "\n";
}

int Input(BINGO* player, BINGO* com, bool playerTurn)
{
	int input;
	if (playerTurn)
	{

		do
		{
			std::cout << "Player input : ";

			std::cin >> input;

			if (input < 1 || input > WIDTH * HEIGHT)
				std::cout << "Invalid input range.\n";
			else
			{
				if (player[input] == BINGO::CHECK || player[input] == BINGO::COMPLETE)
					std::cout << "That number had already been taken.\n";
			}

		} while (input < 1 || input > WIDTH * HEIGHT || player[input] != BINGO::EMPTY);

		player[input] = BINGO::CHECK;
		com[input] = BINGO::CHECK;
	}
	else
	{
		int cnt = 0;
		int temp[HEIGHT * WIDTH] = {};

		for (int i = 1; i < HEIGHT * WIDTH + 1; i++)
		{
			if (com[i] == BINGO::EMPTY)
				temp[cnt++] = i;
		}

		Shuffle(temp, cnt);

		input = temp[rand() % cnt];


		com[input] = BINGO::CHECK;
		player[input] = BINGO::CHECK;

		std::cout << "Computer's choice : " << input << "\n";
	}

	return input;
}

void BingoCheck(int board[][WIDTH], BINGO* checker,
	int input, int* currnetBingo, bool* leftDiagnol, bool* rightDiagnol)
{
	int y = -1;
	int x = -1;

	bool found = false;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (board[i][j] == input)
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

	int cnt1 = 0;
	int cnt2 = 0;

	for (int i = 0; i < WIDTH; i++)
	{
		// 가로
		if (checker[board[y][i]] == BINGO::CHECK || checker[board[y][i]] == BINGO::COMPLETE)
			cnt1++;

		// 세로
		if (checker[board[i][x]] == BINGO::CHECK || checker[board[i][x]] == BINGO::COMPLETE)
			cnt2++;
	}

	if (cnt1 == WIDTH)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			checker[board[y][i]] = BINGO::COMPLETE;
		}

		(*currnetBingo)++;
	}

	if (cnt2 == HEIGHT)
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			checker[board[i][x]] = BINGO::COMPLETE;
		}

		(*currnetBingo)++;
	}

	cnt1 = 0;
	cnt2 = 0;

	if (!(*leftDiagnol) && y == x)
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			if (checker[board[i][i]] == BINGO::CHECK 
				|| checker[board[i][i]] == BINGO::COMPLETE)
				cnt1++;
		}

		if (cnt1 == HEIGHT)
		{
			for (int i = 0; i < HEIGHT; i++)
			{
				checker[board[i][i]] = BINGO::COMPLETE;
			}

			(*currnetBingo)++;
			*leftDiagnol = true;
		}
	}

	if (!(*rightDiagnol) && (y + x == WIDTH - 1))
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			if (checker[board[i][WIDTH - i - 1]] == BINGO::CHECK 
				|| checker[board[i][WIDTH - i - 1]] == BINGO::COMPLETE)
				cnt2++;
		}

		if (cnt2 == HEIGHT)
		{
			for (int i = 0; i < HEIGHT; i++)
			{
				checker[board[i][WIDTH - i - 1]] = BINGO::COMPLETE;
			}

			(*currnetBingo)++;
			*rightDiagnol = true;
		}
	}
}