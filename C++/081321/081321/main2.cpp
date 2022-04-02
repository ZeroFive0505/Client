#include <iostream>

enum class EJob
{
	None,
	Knight,
	Archer,
	Magicion
};

struct Player
{
	char Name[32];
	EJob job;
	int Attack;
	int Armor;
};

int main()
{
	Player player;

	strcpy_s(player.Name, "Hello World");
	player.job = EJob::Archer;
	player.Attack = 8;
	player.Armor = 16;

	FILE* FileStraem;

	fopen_s(&FileStraem, "Test1.bin", "wb");

	if (FileStraem)
	{
		int Number = 100;
		int Array[10];
		for (int i = 0; i < 10; i++)
			Array[i] = i + 1;
		// 1번인자 : 저장하고자 하는 변수의 주소가 들어온다.
		// 2번인자 : 저장하고자 하는 타입의 크기를 지정한다.
		// 3번인자 : 저장하고자 하는 개수를 지정한다.
		// 2번인자 * 3번인자의 바이트 수만큼을 1번인자의 지정한 주소로부터
		// 저장하게 된다.
		// 4번인자 : 파일스트림이 들어간다.
		fwrite(&Number, sizeof(int), 1, FileStraem);
		fwrite(Array, sizeof(int), 10, FileStraem);
		fwrite(&player, sizeof(player), 1, FileStraem);


		fclose(FileStraem);
	}

	Player player1 = {};
	int Number1 = 0;
	int Array1[10] = {};

	fopen_s(&FileStraem, "Test1.bin", "rb");

	if (FileStraem != nullptr)
	{
		fread(&Number1, sizeof(int), 1, FileStraem);
		fread(Array1, sizeof(int), 10, FileStraem);
		fread(&player1, sizeof(Player), 1, FileStraem);

		std::cout << "Number1 : " << Number1 << "\n";

		for (int i = 0; i < 10; i++)
			std::cout << i << " : " << Array1[i] << "\n";

		std::cout << "Name : " << player1.Name << "\n";
		std::cout << "Job : ";

		switch (player1.job)
		{
		case EJob::Archer:
			std::cout << "Archer\n";
			break;
		case EJob::Knight:
			std::cout << "Knight\n";
			break;
		case EJob::Magicion:
			std::cout << "Magicion\n";
			break;
		}

		fclose(FileStraem);
	}

	return 0;
}