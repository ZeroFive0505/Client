#include <iostream>

/*
* ���� ����� : �ϵ��ũ�� ������ ����ų� �ִ� ������ �����͸� ���ö� ����Ѵ�.
* 
* C��� ��� : fopen �Լ��� �̿��ؼ� ó���Ѵ�.
* C++ ��� : ifstream, ofstream�� �̿��ؼ� ó���Ѵ�.
*/

class CPlayer
{

};

void CreatePlayer(CPlayer** ppPlayer)
{
	*ppPlayer = new CPlayer;
}

int main()
{
	FILE* FileStraem = nullptr;

	/*
	* 3�� ���ڿ��� Mode�� ����.
	* r : ������ �о�´�.
	* w : ������ �����.
	* a : ���Ͽ� �����Ͽ� �̾�⸦ �Ѵ�.
	* r+ : ������ �����ϸ� �ش� ������ �ϰ� ���� �����ϰ� ����. ������ ������ ���� ��ȯ
	* w+ : ������ �����ϸ� �ش� ������ �а� ���� �����ϰ� ����. �Ǿ��� ������ ���� ������ش�.
	* a+ : ������ �����ϸ� �ش� ������ ������ �а� ���Ⱑ �� �� �����ϰ� ����. ������ ������ ���� �����.
	* 
	* t : �ؽ�Ʈ ����
	* b : ���̳ʸ� ����
	* rt : �ؽ�Ʈ������ �о�´�.
	* rb : ���̳ʸ������� �о�´�.
	* wt : �ؽ�Ʈ������ �����.
	* wb : ���̳ʸ� ������ �����.
	* at
	* ab
	*/
	fopen_s(&FileStraem, "Test.txt", "wt");

	if (FileStraem != nullptr)
	{
		// fputc : ���Ͽ� ���� �ϳ��� �߰��Ѵ�.
		fputc('A', FileStraem);
		fputc('A', FileStraem);
		fputc('A', FileStraem);

		fputs("Hello World.\n", FileStraem);
		fputs("Hello World.\n", FileStraem);
		fputs("Hello World.\n", FileStraem);

		int Number = 100;
		int Number1 = 200;
		int Number2 = 300;

		// %d : ������ ������ ����Ѵ�.
		// %s : ���ڿ��� ������ ����Ѵ�.
		// %f : �Ǽ��� ������ ����Ѵ�.
		fprintf(FileStraem, "Number %d.\n", Number);
		fprintf(FileStraem, "Number1 %d.\n", Number1);
		fprintf(FileStraem, "Number2 %d.\n", Number2);

		// ���� ��Ʈ���� �������� �ݵ�� �ݾ���� �Ѵ�.
		fclose(FileStraem);
	}

	fopen_s(&FileStraem, "Test.txt", "rt");

	if (FileStraem != nullptr)
	{
		// 1������ : ���� ��Ʈ���� ����.
		// 2������ : 3�����ڿ� ������ ���������κ��� �� ����Ʈ�� �̵��� �������� �����Ѵ�.
		// 3������ : �������� �����ش�.
		// SEEK_CUR : ���� ���� �������� ��ġ�̴�.
		// SEEK_END : ������ ���� �� ��ġ�� �̵��Ѵ�.
		// SEEK_SET : ������ ���� ��ġ�� �̵��Ѵ�.
		fseek(FileStraem, 0, SEEK_END);

		// ftell : ���� ����Ŀ���� ��ġ�� ���´�.
		int FileSize = ftell(FileStraem);

		fseek(FileStraem, 0, SEEK_SET);

		std::cout << "File Size : " << FileSize << "\n";

		char c = fgetc(FileStraem);

		std::cout << c << "\n";

		char Line[128] = {};
		// fgets �Լ��� \n�� ������ �Ǹ� �ű������ �о���� �ȴ�.
		fgets(Line, 127, FileStraem);
		
		std::cout << Line << "\n";

		fgets(Line, 127, FileStraem);

		std::cout << Line << "\n";

		fgets(Line, 127, FileStraem);

		std::cout << Line << "\n";

		fclose(FileStraem);
	}

	//CPlayer* pPlayer = nullptr;
	//CreatePlayer(&pPlayer);

	//delete pPlayer;

	return 0;
}