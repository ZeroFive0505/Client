#include <iostream>

class CStatic
{
public:
	CStatic()
	{

	}

	~CStatic()
	{

	}

public:
	int m_iNumber;
	static int m_iNumberStatic;

public:
	void Output()
	{
		std::cout << "Output\n";
	}

	// static 멤버함수에서는 this를 사용할 수 없다.
	// 그렇기 때문에 일반 멤버변수는 사용할 수 없다.
	// 여기에서는 static 멤버 변수만 사용이 가능하다.
	// 함수포인터 형태가 전역함수와 동일한 형태로 사용이 가능하다.
	static void OutputStatic()
	{
		std::cout << "Output Static\n";
		// std::cout << m_Number << "\n";
		// std::cout << this << "\n";
	}
};

// 클래스의 static 멤버 변수는 반드시 클래스 외부에 선언 부분을 만들어주어야 한다.
// 기본으로 0으로 초기화 된다. 원하는 값 대입도 가능.
// 클래스의 static 멤버변수는 이 클래스를 이용해서 객체를 아무리 많이 생성하더라도
// 이 변수의 메모리는 딱 1개만 생성이 된다.
// 일반 멤버변수라면 객체의 수만큼 메모리 공간이 할당될 것이다.
int CStatic::m_iNumberStatic;

class CSingleton
{
	
private:
	// 생성자와 소멸자가 private이라면 이 클래스의 외부에서 객체를 생성하는 것이 불가능하다.
	// 불가능하고 이 클래스의 외부에서 객체를 메모리에서 해제하는 것이 불가능 하다.
	CSingleton()
	{

	}

	~CSingleton()
	{

	}
private:
	static CSingleton* m_pInst;

public: 
	static CSingleton* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CSingleton;

		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst != nullptr)
		{
			delete m_pInst;
			m_pInst = nullptr;
		}
	}
};

CSingleton* CSingleton::m_pInst = nullptr;

int main()
{
	CStatic std1;

	std1.m_iNumberStatic = 100;
	CStatic::m_iNumberStatic = 300;

	std::cout << std1.m_iNumberStatic << "\n";

	void(*pFunc)() = CStatic::OutputStatic;

	// 일반 멤버 함수의 포인터는 사용방법이 다르다.
	// pFunc = CStatic::Output;


	void(CStatic:: * pFunc1)() = &CStatic::Output;

	pFunc();

	// 일반 멤버함수는 반드시 this가 지정이 되어야 한다.
	// 그렇기 때문에 어떤 객체를 this로 지정할지 반드시 선행되어야 한다.
	(std1.*pFunc1)();

	// CSingleton singleton;

	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";

	CSingleton::DestroyInst();

	return 0;
}