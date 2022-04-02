
#include <iostream>

/*
상속 : 클래스는 부모의 자식의 관계를 형성할 수 있다.
자식클래스는 부모클래스의 멤버변수 혹은 멤버함수를 물려받아 사용할 수 있게 된다.

상속관계에서 생성자와 소멸자의 호출 순서
생성자 : 부모 -> 자식
소멸자 : 자식 -> 부모

protected : 클래스의 외부에서는 접근이 불가능하고 클래스의 내부 혹은 자식클래스의 내부에서는
접근이 가능하다.

보통 상속을 사용하는 이유는 클래스의 재사용성도 하나의 이유가 된다.
부모클래스에 공통으로 가져야 할 멤버변수를 선언하고 공통으로 가져야 할 기능을 멤버함수로 만들어준
후에 개별적인 내용들만 자식클래스에 따로 조금씩 만들어서 하나의 객체를 만들때 사용한다.

다형성 : 상속관계에 있는 클래스들은 서로 형변환이 가능하다. 다양한 부모와 자식 타입으로의 형변환을
지원해주기 때문에 같은 부모를 상속받고 있는 자식클래스는 해당 부모타입으로 모든 자식클래스 타입의
변수들을 부모타입으로 관리가 가능해지게 되는 것이다.

업캐스팅 : 자식 -> 부모 타입으로 형변환
다운캐스팅 : 부모 -> 자식 타입으로 형변환

가상함수 : 함수 앞에 virtual을 붙여서 가상함수로 만들어줄 수 있다.
가상함수를 가지고 있는 클래스를 이용해서 객체를 생성하면 가상함수 테이블의 주소를 가지고 있는
포인터 변수를 내부적으로 가지게 된다.

가상함수 테이블은 해당 클래스가 가지고 있는 가상함수의 함수주소를 저장하고 있는 배열이라고 볼 수 있다.
클래스를 이용해서 객체를 생성하면 내부에 __vfptr 포인터 변수가 만들어지고 이 변수에 가상함수테이블
배열의 메모리 시작주소가 들어가게 된다.

가상함수는 자식클래스에서 해당 함수를 재정의할 수 있다.
이를 함수 오버라이딩 이라고 한다.
함수를 재정의 할때는 반환타입, 인자가 모두 동일해야 한다. 함수 이름도 당연히 동일해야 한다.
함수 재정의 이기 때문에 말 그대로 해당 함수를 자식클래스에 다시 만들어서 자식클래스의 함수가
호출 될 수 있도록 만들어주는 기능인 것이다.
소멸자는 모든 클래스가 공통되게 이름만 다를뿐 소멸자이므로 부모클래의 소멸자에 virtual을 붙여주면
자식클래스는 자동으로 재정의된 소멸자로 인식이 되게 된다.

일반 멤버함수에 virtual을 붙여서 가상함수로 만들어주면 자식클래스의 재정의를 해도 되고 안해도 된다.
만약 CChild클래스는 재정의가 되어있고 CChild1 클래스는 재정의가 안되었을 경우
가상함수 테이블에 저장되는 함수의 주소는 CChild 클래스를 이용해서 생성한 객체는 CChild클래스에
재정의한 함수의 주소가 들어가게 되고
CChild1 클래스를 이용해서 생성한 객체는 재정의가 안되어 있으므로 CParent 클래스에 정의된 함수의
주소가 들어가게 되는 것이다.

그래서 가상함수를 호출을 하게 되면 먼저 가상함수인지를 판단하고 가상함수라면 가상함수 테이블에서
실제 함수의 주소를 찾아온 후에 해당 함수의 주소를 이용하여 함수호출을 진행하게 되므로
CChild 클래스를 이용해서 생성한 객체는 가상함수테이블에 CChild클래스에 재정의된 함수의 주소가 들어가
있으므로 재정의된 함수를 호출해주고
CChild1 클래스를 이용해서 생성한 객체는 가상함수테이블에 CParent클래스에 선언된 함수의 주소가
들어가 있으므로 해당 함수를 호출하게 되는 것이다.

*/
class CParent
{
public:
	CParent()
	{
		std::cout << "CParent 생성자" << std::endl;
	}

	virtual ~CParent()
	{
		std::cout << "CParent 소멸자" << std::endl;
	}

public:
	int	m_Number1;

private:	// 자식클래스에서도 접근이 불가능하다.
	int	m_Number2;

protected:
	int	m_Number3;

public:
	void SetNumber2(int Num2)
	{
		m_Number2 = Num2;
	}

public:
	virtual void Output()
	{
		std::cout << "CParent Output" << std::endl;
	}
};

class CParent1
{
public:
	CParent1()
	{
		std::cout << "CParent1 생성자" << std::endl;
	}

	~CParent1()
	{
		std::cout << "CParent1 소멸자" << std::endl;
	}

public:
	int	m_Number3;
};

class CChild2 : public CParent, CParent1
{
public:
	CChild2()
	{
		std::cout << "CChild2 생성자" << std::endl;
		//m_Number3 = 4040;
	}

	~CChild2()
	{
		std::cout << "CChild2 소멸자" << std::endl;
	}
};

class CChild : public CParent
{
public:
	CChild()
	{
		std::cout << "CChild 생성자" << std::endl;
		m_Number1 = 100;
		//m_Number2 = 300;
		m_Number3 = 500;

		m_NumberPointer = new int;
	}

	~CChild()
	{
		std::cout << "CChild 소멸자" << std::endl;
		delete	m_NumberPointer;
	}

protected:
	int* m_NumberPointer;


public:
	void Output()
	{
		// CParent::Output();
		std::cout << "CChild Output" << std::endl;
	}

	void ChildOutput()
	{
		std::cout << "CChild에만 있는 Output" << std::endl;
	}
};

class CChild1 : private CParent
{
public:
	CChild1()
	{
		std::cout << "CChild1 생성자" << std::endl;
		m_Number1 = 100;
		m_Number3 = 300;
	}

	~CChild1()
	{
		std::cout << "CChild1 소멸자" << std::endl;
	}
};

class CChildChild : public CChild
{
public:
	CChildChild()
	{
		std::cout << "CChildChild 생성자" << std::endl;
		m_Number1 = 100;
		m_Number3 = 300;
	}

	~CChildChild()
	{
		std::cout << "CChildChild 소멸자" << std::endl;
	}
};

class CChildChild1 : public CChild1
{
public:
	CChildChild1()
	{
		std::cout << "CChildChild1 생성자" << std::endl;
		//m_Number1 = 100;
		//m_Number3 = 300;
	}

	~CChildChild1()
	{
		std::cout << "CChildChild1 소멸자" << std::endl;
	}
};


// 멤버변수가 없을 경우 크기를 구하면 1바이트가 나온다.
// 구조체나 클래스는 해당 타입을 이용하여 변수를 선언할 수 있기 때문에
// 최소 바이트인 1바이트를 만들어서 변수 선언이 가능하게 해준다.
struct Test
{
};

class CTestParent
{
public:
	CTestParent()
	{
	}

	virtual ~CTestParent()
	{
	}

	virtual void Output()
	{
	}

	virtual void Output1()
	{
	}

	virtual void Output2()
	{
	}

public:
	int	m_Test;
	int	m_Test1;
	int	m_Test2;
	int	m_Test3;
};

class CTestChild : public CTestParent
{
public:
	CTestChild()
	{
	}

	~CTestChild()
	{
	}
};

int main()
{
	std::cout << sizeof(CChild) << std::endl;
	std::cout << sizeof(Test) << std::endl;
	std::cout << sizeof(CTestChild) << std::endl;

	CTestChild	testchild;

	// CChild 클래스를 동적할당하고 해당 메모리 주소를 CParent 포인터 타입에 저장해두었다.
	// 서로 상속관계에 있는 클래스이기 때문에 이런 형변환이 가능한 것이다.
	CParent* Child = new CChild;
	//CParent* Child1 = new CChild1;
	CParent* Child2 = new CChild2;
	CParent* ChildChild = new CChildChild;
	//CParent* ChildChild1 = new CChildChild1;

	std::cout << "=================Test output=================\n";

	Child->Output();
	Child->CParent::Output();
	ChildChild->Output();

	// Child는 근본적으로 CParent* 타입의 변수이다.
	// 다운캐스팅을 할때는 정말 주의해서 사용해야 한다.
	// 왜냐하면 Child는 CChild타입 클래스를 객체로 생성한 것이다.
	// 그런데 만약 이것을 CChild1 타입으로 형변환을 하면
	// CChild 생성 -> CParent* 형변환 -> CChild1 형변환 이 되어 버리게 되는것이다.
	// 이 객체는 근본적으로 CChild 객체를 생성한 것이므로 CChild1 타입으로 형변환하여 사용할
	// 경우 많은 문제를 발생시킬 수 있다.
	// 그러므로 다운캐스팅을 할때는 항상 정말 많이 많이 많이 많이 많이 많이 주의 해야 됩니다.
	((CChild*)Child)->ChildOutput();

	Child2->Output();

	delete	Child;
	delete	Child2;
	delete	ChildChild;

	//CChild2	child2;

	/*Test	test1;

	CChild	child;
	CChild1	child1;*/

	//child1.m_Number1 = 300;

	/*child.m_Number1 = 100;
	child.SetNumber2(1010);
	child.Output();*/

	return 0;
}
