#include <iostream>

int main()
{
	const int NROWS_1 = 5;
	const int NROWS_2 = 7;
	std::cout << "1 - 1\n";
	for (int i = 0; i < NROWS_1; i++)
	{
		for (int j = 0; j < NROWS_1 - i; j++)
		{
			std::cout << "*";
		}
		std::cout << "\n";
	}

	std::cout << "\n\n";

	std::cout << "1 - 2\n";
	for (int i = 0; i < NROWS_1; i++)
	{
		for (int j = i + 1; j < NROWS_1; j++)
			std::cout << " ";

		for (int j = 0; j <= i; j++)
			std::cout << "*";

		std::cout << "\n";
	}

	std::cout << "\n\n";

	std::cout << "1 - 3\n";
	for (int i = 0; i < NROWS_1; i++)
	{
		for (int j = 0; j < i; j++)
			std::cout << " ";

		for (int j = i; j < NROWS_1; j++)
			std::cout << "*";

		std::cout << "\n";
	}

	std::cout << "\n\n";

	std::cout << "1 - 4\n";
	for (int i = 0; i < NROWS_2; i++)
	{
		if (i <= NROWS_2 / 2)
		{
			for (int j = i; j < NROWS_2 / 2; j++)
				std::cout << " ";

			for (int j = 0; j < i * 2 + 1; j++)
				std::cout << "*";

			std::cout << "\n";
		}
		else
		{
			for (int j = NROWS_2 / 2; j < i; j++)
				std::cout << " ";

			for (int j = 0; j < NROWS_2 - (i * 2 + 1 - NROWS_2); j++)
				std::cout << "*";

			std::cout << "\n";
		}
	}

	return 0;
}