
#include <iostream>

/*
* 순수 가상 함수 : 가상함수 뒤에 = 0을 붙여주면 해당 가상함수는 순수가상함수가 된다.
* 순수가상함수는 자식클래스에서 무조건 재정의를 해야된다.
* 만약 자식 클래스에서 재정의를 안한다면 해당 자식클래스는 추상클래스가 되어서 해당 클래스 타입의 객체 생성이 불가능하다.
* 순수 가상함수는 해당 순수가상함수를 만들어주는 클래스에 구현부분을 만들어도되고 안 만들어된다.
* 순수가상함수는 자식클래스마다 따로 구현이 되어야 하는 경우 순수가상함수로 만들어서 자식에서 무조건 재정의를 하여 구현하게끔
* 만들때 사용한다.
* 
* 추상 클래스 : 순수가상함수를 가지고 있는 클래스를 추상 클래스라고 한다. 추상 클래스는 객체 생성이 불가능하다.
*/

class CParent
{
public:
	CParent()
	{

	}

	virtual ~CParent()
	{

	}

public:
	virtual void Output()
	{
		std::cout << "CParent Output\n";
	}

	virtual void OutputPure() = 0;

	virtual void OutputPure1() = 0
	{
		std::cout << "CParent OutputPure1\n";
	}
};

class CChild : public CParent
{
public:
	CChild()
	{
	}

	~CChild()
	{

	}

	virtual void OutputPure() override 
	{

	}

	virtual void OutputPure1() override
	{
		std::cout << "CChild OutputPure1\n";
	}
};

class CChild1 : public CParent
{
public:
	CChild1()
	{
	}

	~CChild1()
	{

	}

public:
	void Output() override
	{
		std::cout << "CChild1 Output\n";
	}

	virtual void OutputPure() override
	{

	}

	virtual void OutputPure1() override
	{
		std::cout << "CChild OutputPure1\n";
	}
};

int main()
{
	CParent* Child = new CChild;
	CParent* Child1 = new CChild1;

	Child->Output();
	Child1->Output();

	delete Child;
	delete Child1;

	return 0;
}