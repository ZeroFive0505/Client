#include <iostream>

/*
* 파일 입출력 : 하드디스크에 파일을 만들거나 있는 파일의 데이터를 얻어올때 사용한다.
* 
* C언어 방삭 : fopen 함수를 이용해서 처리한다.
* C++ 방식 : ifstream, ofstream을 이용해서 처리한다.
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
	* 3번 인자에는 Mode가 들어간다.
	* r : 파일을 읽어온다.
	* w : 파일을 만든다.
	* a : 파일에 접근하여 이어쓰기를 한다.
	* r+ : 파일이 존재하면 해당 파일을 일고 쓰기 가능하게 연다. 파일이 없으면 에러 반환
	* w+ : 파일이 존재하면 해당 파일을 읽고 쓰기 가능하게 연다. 피알이 없으면 새로 만들어준다.
	* a+ : 파일이 존재하면 해당 파일의 끝부터 읽고 쓰기가 둘 다 가능하게 연다. 파일이 없으면 새로 만든다.
	* 
	* t : 텍스트 파일
	* b : 바이너리 파일
	* rt : 텍스트파일을 읽어온다.
	* rb : 바이너리파일을 읽어온다.
	* wt : 텍스트파일을 만든다.
	* wb : 바이너리 파일을 만든다.
	* at
	* ab
	*/
	fopen_s(&FileStraem, "Test.txt", "wt");

	if (FileStraem != nullptr)
	{
		// fputc : 파일에 문자 하나를 추가한다.
		fputc('A', FileStraem);
		fputc('A', FileStraem);
		fputc('A', FileStraem);

		fputs("Hello World.\n", FileStraem);
		fputs("Hello World.\n", FileStraem);
		fputs("Hello World.\n", FileStraem);

		int Number = 100;
		int Number1 = 200;
		int Number2 = 300;

		// %d : 정수를 받을때 사용한다.
		// %s : 문자열을 받을때 사용한다.
		// %f : 실수를 받을때 사용한다.
		fprintf(FileStraem, "Number %d.\n", Number);
		fprintf(FileStraem, "Number1 %d.\n", Number1);
		fprintf(FileStraem, "Number2 %d.\n", Number2);

		// 파일 스트림을 열었으면 반드시 닫아줘야 한다.
		fclose(FileStraem);
	}

	fopen_s(&FileStraem, "Test.txt", "rt");

	if (FileStraem != nullptr)
	{
		// 1번인자 : 파일 스트림이 들어간다.
		// 2번인자 : 3번인자에 지정된 기준점으로부터 몇 바이트를 이동할 것인지를 지정한다.
		// 3번인자 : 기준점을 정해준다.
		// SEEK_CUR : 현재 파일 포인터의 위치이다.
		// SEEK_END : 파일의 가장 끝 위치로 이동한다.
		// SEEK_SET : 파일의 시작 위치로 이동한다.
		fseek(FileStraem, 0, SEEK_END);

		// ftell : 현재 파일커서의 위치를 얻어온다.
		int FileSize = ftell(FileStraem);

		fseek(FileStraem, 0, SEEK_SET);

		std::cout << "File Size : " << FileSize << "\n";

		char c = fgetc(FileStraem);

		std::cout << c << "\n";

		char Line[128] = {};
		// fgets 함수는 \n을 만나게 되면 거기까지만 읽어오게 된다.
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