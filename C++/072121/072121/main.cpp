#include <iostream>

/*
* 함수에도 주소가 존재하기 때문에 해당 함수의 주소를 받아놓을 함수 포인터 변수를 선언하고
* 주소를 알고 있다면 함수 포인터 변수를 이용해서 해당 함수를 호출할 수 있다.
* 
* 형태
* 반환 타입(*포인터명)(인자 타입들);의 형태로 선언이 된다.
*/

struct Player
{
	char Name[32];
	int Attack;
	int Armor;
	int HP;
	int HPMax;
};

void Output()
{
	std::cout << "Output Function\n";
}

void Output1()
{
	std::cout << "Output1 Function\n";
}

int Add(int Number1, int Number2)
{
	return Number1 + Number2;
}

int Sub(int Number1, int Number2)
{
	return Number1 - Number2;
}

float Add1(float Number1, float Number2)
{
	return Number1 + Number2;
}

void ChangeNumber(int* pNumber)
{
	*pNumber = 300;
}

// 구조체는 메모리크기가 큰 구조체가 만들어질 수도 있다.
// 그렇기 때문에 단순하게 구조체를 인자로 넘길 경우
// 구조체의 모든 멤버들을 복사하게 되는 상황이 발생할 수 있다.
// 이 경우 구조체의 크기가 크면 클수록 성능이 떨어지게 된다.
void TestPlayer(Player player)
{

}

// 구조체의 값을 변경하는 것이 아니라면 const &를 이용해서
// 참조를 얻어오고 앞에 const가 붙어있기 때문에 대상의 값을 변경하는 것이 아닌
// 단순히 얻어오는 용도로만 사용할 수 있는데 당연히 참조를 하는 방식이기 때문에
// 새로운 Player를 생성해서 복사하는 것이 아닌 참조를 하게 되는 것이다.
// 그러므로 몇 백 바이트, 몇 천 바이트를 복사하게 될 경우 느려지게 되는데
// 단순 참조만 하게 되어 인자를 넘겨주는 속도가 향상될 수 있다.
void TestPlayer1(const Player& player)
{

}

int main()
{
	void (*pFunc)();

	// 함수 포인터 변수에 함수의 주소를 저장한다.
	pFunc = Output;
	
	// 저장된 주소의 함수를 호출한다.
	pFunc();

	pFunc = Output1;

	pFunc();

	int (*pFunc1)(int, int);

	pFunc1 = Add;

	std::cout << pFunc1(10, 20) << "\n";

	pFunc1 = Sub;

	std::cout << pFunc1(10, 20) << "\n";

	// pFunc1 = Add1;

	int (*pFunc2[2])(int, int);

	pFunc2[0] = Add;
	pFunc2[1] = Sub;

	for (int i = 0; i < 2; i++)
	{
		std::cout << pFunc2[i](30, 40) << "\n";;
	}

	int Number = 90;

	ChangeNumber(&Number);

	std::cout << Number << "\n";

	/*
	* 레퍼런스 : 다른 변수를 참조하는 변수를 만들 수 있다.
	* 레퍼런스는 반드시 선언과 동시에 참조할 변수를 지정해주어야 한다.
	* 그 이후에는 다른 변수를 넣더라도 참조가 아닌 해당 변수의 값을 레퍼런스가 참조하는
	* 변수에 대입해주는 역활을 한다.
	*/

	// NumberRef 레퍼런스는 Number 변수를 참조하게 되어 Number 변수의 값을 얻어와서
	// 사용하거나 값을 변경할 수 있다.
	int& NumberRef = Number;

	NumberRef = 1234;

	int Number1 = 9909;

	// 레퍼런스는 선언할때만 참조하는 변수를 지정하는 것이고 그 이후부터는
	// 변수의 값을 참조하는 대상에 대입해주는 역할만 한다.
	NumberRef = Number1;

	std::cout << Number << "\n";

	Player player = {};

	TestPlayer(player);

	TestPlayer1(player);

	/*
	* 동적할당 : 미리 메모리를 생성하는 것이 아닌 내가 원하는 시점에 메모리를
	* 생성해줄 수 있는 기능이다.
	* new, delete를 이용해서 동적할당에 관련된 기능등을 만들어나갈 수 있다.
	* 
	* 동적할당된 메모리는 힙 영역에 공간이 만들어진다.
	* 
	* 형태 new 생성하고자하는 타입; 을 해주게 되면 해당 타입의 크기만큼을
	* 힙 영역에 공간을 생성하고 생성된 공간의 메모리 주소를 반환한다.
	* 
	* 동적할당된 메모리는 자동으로 해제가 안된다.
	* 무조건 delete를 이용해서 해당 메모리를 해제해야 한다.
	* 만약, 동적할당을 해놓고 delete를 안한다면 메모리 누수가 일어나게 되고
	* 메모리 누수는 동적할당하고 안 지우면 해당 메모리는 계속 사용되는 중이라고 인식되는 것을 말한다.
	* 
	* delete 메모리 주소; 를 통해서 해당 주소를 제거한다.
	* 
	* new를 이용해서 생성하면 그 주소가 반환되기 때문에 이 주소를 제거해야 하는 것이다.
	*/

	// new int를 하면 힙 영역에 4바이트 만큼 공간이 할당되고
	// 그 후에 해당 주소를 반환해주고
	// pDynamicNumber는 지역 변수이기 때문에 스택 공간이 만들어지고
	// pDynamicNumber가 힙 영역에 공간이 만들어진 주소를 받아놓고
	// 주소를 알기 때문에 해당 힙 영역에 접근해서 사용할 수 있고
	// 다 썼다면 delete로 메모리를 반환한다.
	int* pDynamicNumber = new int;

	*pDynamicNumber = 2929;

	std::cout << *pDynamicNumber << "\n";

	delete pDynamicNumber;

	pDynamicNumber = nullptr;

	/*
	* 위에서 동적할당된 메모리는 제거가 되었다.
	* 문제는 pDynamicNumber에는 아직까지 제거된 힙영역의 메모리 주소가 그대로 들어가 있다.
	* 
	* 이렇게 제거된 메모리 주소를 저장하는 포인터를 댕글링 포인터라고 하며, 만약
	* 이 댕글링 포인터를 사용하려고하면 프로그램은 그 즉시 문제가 발상해여 종료될 수 있다.
	*/

	/*
	* 배열도 new를 이용해서 동적으로 만들 수 있다.
	* new 타입[개수]; 로 만들어준다.
	*/

	// int크기 * 100 만큼의 메모리가 힙 영역에 만들어지게 되고 그 시작 주소를 반환해서
	// pArray 포인터 변수가 주소를 가지고 있게 된다.
	int* pArray = new int[100];

	for (int i = 0; i < 100; i++)
	{
		pArray[i] = i + 1;
	}

	delete[] pArray;

	// 제거된 메모리 주소를 가지고 있는 포인터 변수는 항상
	// nullptr로 초기화를 해주는 것이 좋다.
	// nullptr은 0이다.
	// 포인터는 다 사용을 했다면 항상 nullptr로 초기화하는 습관이 필요하다.
	pArray = nullptr;

	std::cout << pArray << "\n";

	if (pArray != nullptr)
	{
		std::cout << "pArray is pointing a memory\n";
	}

	return 0;
}