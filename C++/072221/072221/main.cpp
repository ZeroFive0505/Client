#include <iostream>

/*
* 절차지향 프로그래밍 : 순차적으로 처리해주는 프로그래밍 기법이다.
* 
* 객체지향 프로그래밍(OOP) : 객체(모든 사물)을 제작하여 객체를 조립하여 하나의 완성된 프로그램을
* 만들어나가는 방식이다.
* C++에서는 객체지향 프로그래밍을 지원하기 위한 수단으로 클래스를 제공한다.
* 
* 캡슐화 : 구조체와 같이 여러 데이터 혹은 함수를 묶어서 사용할 수 있게 만들어주는 속성이다.
* 은닉화 : 구조체나 클래스의 멤버를 외부에 공개하는 수준을 설정할 수 있는 속성이다.
* - public: 클래스 혹은 구조체의 내부(클래스나 구조체에 만들어져있는 함수 안)와 외부(클래스의 함수 안이 아닌 다른 클래스
* 혹은 다른 함수 안)에서 모두 접근이 가능한 속성이다.
* 구조체는 아무것도 안 붙여줄 경우 기본으로 public 이다.
* - private: 클래스 혹은 구조체의 내부에서만 접근이 가능하고 외부에서는 접근이 불가능하다.
* 클래스는 아무것도 안 붙여줄 경우 기본으로 private 이다.
* 
* - protected: 상속할때 설명
* 상속성 : 상속할때 설명.
* 다형성 : 상속할때 설명.
* 
* 클래스 : C++에서 객체지향을 위해 제공하는 수단이다.
* 구조체와 마찬가지로 다양한 변수와 함수들을 클래스 내부에 만들어두고 사용이 가능하다.
*/

struct Test
{
	int x;
	int y;
};

class CPlayer
{
public:
	int m_Number1;
	int m_Number2;
	int* m_pArray;
private:
	int m_Number3;

public:
	// 클래스는 객체를 만들기 위한 수단이며 객체가 아니다.
	// 생성자 : 이 클래스를 이용해서 객체를 생성할때 자동으로 호출이 되는 함수이다.
	// 생성자에서는 Initializer를 사용해서 멤버변수를 초기화 할 수 있다.
	CPlayer()
	{
		std::cout << "CPlayer Constructor\n";
		m_Number1 = 100;

		int Num2 = 100;

		m_pArray = new int[100];

		for (int i = 0; i < 100; i++)
		{
			m_pArray[i] = i + 1;
		}
	}

	CPlayer(int Number1) : m_Number1(Number1)
	{
		std::cout << "One int init\n";


		m_pArray = new int[100];

		for (int i = 0; i < 100; i++)
		{
			m_pArray[i] = i + 1;
		}
	}

	CPlayer(int Number1, int Number2, int Number3) :
		m_Number1(Number1), m_Number2(Number2), m_Number3(Number3)
	{
		std::cout << "Three int init\n";


		m_pArray = new int[100];

		for (int i = 0; i < 100; i++)
		{
			m_pArray[i] = i + 1;
		}
	}


	// 복사 생성자.
	CPlayer(const CPlayer& player)
	{
		// 단순하게 변수의 값들만 복사하는 방식을 얕은 복사라고 한다.
		m_Number1 = player.m_Number1;
		m_Number2 = player.m_Number2;
		m_Number3 = player.m_Number3;

		// 이럴 경우 두 객체가 모두 같은 배열의 메모리 주소를 갖게 되므로
		// 소멸자에서 해당 동적배열을 메모리에서 제거할때 문제가 발생할 수 있다.
		// 아래처럼 메모리 공간도 새로운 공간으로 만들고 그 공간 안에 있는 데이터들만
		// 복사를 하는 방법을 깊은 복사라고 한다.
		// m_pArray = player.m_pArray;
		m_pArray = new int[100];
		for (int i = 0; i < 100; i++)
		{
			m_pArray[i] = player.m_pArray[i];
		}
	}

	// 소멸자 : 이 클래스를 이용해서 생성한 객체가 메모리에서 해제될때 자동으로 호출이 되는 함수이다.
	// 소멸자는 오버로딩 불가능.
	~CPlayer()
	{
		delete[] m_pArray;
		std::cout << "CPlayer Destructor\n";
	}

public:
	/*
	* 클래스의 멤버함수 : 클래스의 멤버함수는 일반함수와 마찬가지로 함수 주소가 1개만 나오게 된다.
	* 그런데 클래스의 멤버함수에서는 이 멤버함수를 호출한 객체의 멤버변수들의 값을 정확하게 인식하고 사용이 가능하다.
	* 그 이유는 this 라는 것 때문이다.
	* this는 자기 자신에 대한 포인터이다.
	* this->m_Number1 이렇게 사용해야 하는데 this->는 생략이 가능
	* 
	* this는 이 멤버함수를 호출할때 호출하는 객체로 지정이 되게 된다.
	* 그렇기 때문에 매번 이 함수를 호출할때마다 this에는 호출하는 객체로 지정이 되며,
	* 매번 다른 객체로 호출한다면 다른 객체들로 매번 this가 바뀌게 될 것이다.
	*/
	void Output()
	{
		std::cout << "Number1 : " << m_Number1 << "\n";
		std::cout << "This : " << this << "\n";
	}

private:
	// 내부에서만 호출이 가능하다.
	void OutputPrivate()
	{

	}
};

int main()
{
	Test test;
	test.x = 0;
	test.y = 0;

	// CPlayer player;

	// player.a = 1010;
	// player.m_Number1 = 1010;
	// player.m_Number2 = 2020;

	// 일반 생성자 호출
	CPlayer* pPlayer = new CPlayer;

	std::cout << "Test\n";

	delete pPlayer;

	// 일반 생성자 호출
	CPlayer player;

	// int타입 1개를 인자로 받는 생성자 호출.
	CPlayer player1(2020);

	// int타입 3개를 인자로 받는 생성자 호출.
	CPlayer player2(10, 20, 30);

	// player2 객체의 Output 함수를 호출한다.
	// player1.Output()을 호출하면 this는 player1의 주소를 값으로 갖게 되며
	// player2.Output()을 호출하면 this는 player2의 주소를 값으로 갖게 된다.
	player1.Output();
	player2.Output();

	std::cout << "player1 address : " << &player1 << "\n";
	std::cout << "player2 address : " << &player2 << "\n";

	return 0;
}