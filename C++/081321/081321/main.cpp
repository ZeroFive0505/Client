
#include <iostream>

/*
* ���� ���� �Լ� : �����Լ� �ڿ� = 0�� �ٿ��ָ� �ش� �����Լ��� ���������Լ��� �ȴ�.
* ���������Լ��� �ڽ�Ŭ�������� ������ �����Ǹ� �ؾߵȴ�.
* ���� �ڽ� Ŭ�������� �����Ǹ� ���Ѵٸ� �ش� �ڽ�Ŭ������ �߻�Ŭ������ �Ǿ �ش� Ŭ���� Ÿ���� ��ü ������ �Ұ����ϴ�.
* ���� �����Լ��� �ش� ���������Լ��� ������ִ� Ŭ������ �����κ��� �����ǰ� �� �����ȴ�.
* ���������Լ��� �ڽ�Ŭ�������� ���� ������ �Ǿ�� �ϴ� ��� ���������Լ��� ���� �ڽĿ��� ������ �����Ǹ� �Ͽ� �����ϰԲ�
* ���鶧 ����Ѵ�.
* 
* �߻� Ŭ���� : ���������Լ��� ������ �ִ� Ŭ������ �߻� Ŭ������� �Ѵ�. �߻� Ŭ������ ��ü ������ �Ұ����ϴ�.
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