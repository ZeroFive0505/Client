#include <iostream>
#include <time.h>
/*
* 함수 : 같은 코드를 여러번 동작시킬 경우 함수로 미리 만들어두고 해당 함수를 원하는 곳에서 호출만 하면 해당
* 코드가 동작될 수 있게 만들어주는 기능이다.
* 
* 함수의 형태
* 반환타입 함수이름(인자)
* {
*	
* }
* 
* 의 형태로 구성이 된다.
* 
* 반환타입 : 이 함수가 특정 코드를 수행하고 어떤 결과를 호출한 곳으로 전달할 필요가 있다면
* 반홭타입을 이용해서 전달할 값의 타입을 지정하고 반환해주면 된다.
* 정수를 반환해야 한다면 반환타입을 int로 하고 return 원하는 값; 을 통해서 해당 함수의
* 어느 곳에서든 값을 반환하게끔 만들어준다.
* 
* 실수를 반환해야 한다면 반환 타입을 float으로 지정한다.
* 
* 함수이름 : 이 함수가 동작할 기능에 맞는 정확한 이름을 작성해준다.
* 
* 인자 : 있어도 되고 없어도 된다. 만약 이 함수를 사용할때 이 함수에 데이터를 전달할 필요가 있을 경우
* 인자를 이용해서 함수로 데이터를 전달하게 된다.
* 
* 인자는 해당 함수 내에서만 사용할 수 있는 변수이다.
* 인자는 해당 함수가 호출이 되면 그때 메모리에 공간이 만들어지게 되고 해당 함수가 종료되면 메모리에서 제거된다.
* 
* 메모리가 제거되는 방법은 함수호출규약에 의해서 제거가 된다.
* 
* 함수 호출규약 검색해서 확인.
* 
* 반환타입을 이용해서 함수의 결과를 반환하는 형태를 Call By Value
* 함수의 인자에 포인터 타입을 이용해서 포인터 변수를 역참조하여 결과를 반환하는 형태를 Call By Address
*/

int Add(int num1, int num2)
{
	return num1 + num2;
}

// 반환타입에 void를 사용하면 반환값을 return 안해도 된다.
// return은 이 함수를 종료시키고 이 함수의 수행된 결가값을 반환할때 사용한다.
// void 타입의 반환타입을 갖는 함수에서는 return; 을 통해서 원할때 이 함수를 종료시킬 수도 있다.
void Output()
{
	std::cout << "Output Function" << "\n";
}

void Output1(int num)
{
	std::cout << num << "\n";
}


void SetArray(int* pArray)
{
	for (int i = 0; i < 10; i++)
		pArray[i] = i + 1;
}

void OutputArray(int* pArray)
{
	for (int i = 0; i < 10; i++)
		std::cout << pArray[i] << " ";
	std::cout << "\n";
}

/*
* 함수의 오버로딩 : 같은 이름으로 여러개의 함수를 만들어줄 수 있는 기능이다.
* 단, 이름이 같다면 반드시 인자의 개수 혹은 타입이 달라야 한다.
* 반환타입은 오버로딩에 영향이 없다.
*/

void ChangeNumber(int number)
{
	number = 9999;
}

void ChangeNumber(float number)
{
	number = 3.14f;
}

void ChangeNumber(int* number)
{
	*number = 9999;
}

void ChangeNumber(int num1, int num2)
{
	num1 = 100;
	num2 = 200;
}

//int ChangeNumber(int num)
//{
//	num = 30;
//	return num;
//}

void SetNumber(int* pNumber)
{
	for (int i = 0; i < 25; i++)
	{
		pNumber[i] = i + 1;
	}
}

void Shuffle(int* pNumber)
{
	for (int i = 0; i < 100; i++)
	{
		int idx1 = rand() % 25;
		int idx2 = rand() % 25;

		int temp = pNumber[idx1];
		pNumber[idx1] = pNumber[idx2];
		pNumber[idx2] = temp;
	}
}

void OutputBingo(int* pNumber)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			std::cout << pNumber[i * 5 + j] << "\t";
		}
		std::cout << "\n";
	}

	std::cout << "\n";
}

int main()
{
	// 함수를 호출해준다. 함수를 호출할때 Add함수는 인자로 2개의 정수를 받으므로
	// 반드시 2개의 정수를 넣어주어야한다.
	// Add함수에서 반환한 값을 num에 넣어준다.
	int num = Add(10, 20);
	int num1 = 102;
	srand(time(0));

	std::cout << num << "\n";
	std::cout << Add(50, 130) << "\n";
	std::cout << Add(100, 100) << "\n";

	Output();

	Output1(900);

	ChangeNumber(num);

	ChangeNumber(num, num1);

	std::cout << "Number : " << num << "\n";

	ChangeNumber(&num);

	std::cout << "Number : " << num << "\n";

	int arr[10] = {};

	SetArray(arr);

	OutputArray(arr);

	int myBingo[25] = {};
	int aiBingo[25] = {};
	
	SetNumber(myBingo);
	SetNumber(aiBingo);

	Shuffle(myBingo);
	Shuffle(aiBingo);

	OutputBingo(myBingo);
	OutputBingo(aiBingo);

	return 0;
}