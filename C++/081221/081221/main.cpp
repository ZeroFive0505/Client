
#include <iostream>

/*
��� : Ŭ������ �θ��� �ڽ��� ���踦 ������ �� �ִ�.
�ڽ�Ŭ������ �θ�Ŭ������ ������� Ȥ�� ����Լ��� �����޾� ����� �� �ְ� �ȴ�.

��Ӱ��迡�� �����ڿ� �Ҹ����� ȣ�� ����
������ : �θ� -> �ڽ�
�Ҹ��� : �ڽ� -> �θ�

protected : Ŭ������ �ܺο����� ������ �Ұ����ϰ� Ŭ������ ���� Ȥ�� �ڽ�Ŭ������ ���ο�����
������ �����ϴ�.

���� ����� ����ϴ� ������ Ŭ������ ���뼺�� �ϳ��� ������ �ȴ�.
�θ�Ŭ������ �������� ������ �� ��������� �����ϰ� �������� ������ �� ����� ����Լ��� �������
�Ŀ� �������� ����鸸 �ڽ�Ŭ������ ���� ���ݾ� ���� �ϳ��� ��ü�� ���鶧 ����Ѵ�.

������ : ��Ӱ��迡 �ִ� Ŭ�������� ���� ����ȯ�� �����ϴ�. �پ��� �θ�� �ڽ� Ÿ�������� ����ȯ��
�������ֱ� ������ ���� �θ� ��ӹް� �ִ� �ڽ�Ŭ������ �ش� �θ�Ÿ������ ��� �ڽ�Ŭ���� Ÿ����
�������� �θ�Ÿ������ ������ ���������� �Ǵ� ���̴�.

��ĳ���� : �ڽ� -> �θ� Ÿ������ ����ȯ
�ٿ�ĳ���� : �θ� -> �ڽ� Ÿ������ ����ȯ

�����Լ� : �Լ� �տ� virtual�� �ٿ��� �����Լ��� ������� �� �ִ�.
�����Լ��� ������ �ִ� Ŭ������ �̿��ؼ� ��ü�� �����ϸ� �����Լ� ���̺��� �ּҸ� ������ �ִ�
������ ������ ���������� ������ �ȴ�.

�����Լ� ���̺��� �ش� Ŭ������ ������ �ִ� �����Լ��� �Լ��ּҸ� �����ϰ� �ִ� �迭�̶�� �� �� �ִ�.
Ŭ������ �̿��ؼ� ��ü�� �����ϸ� ���ο� __vfptr ������ ������ ��������� �� ������ �����Լ����̺�
�迭�� �޸� �����ּҰ� ���� �ȴ�.

�����Լ��� �ڽ�Ŭ�������� �ش� �Լ��� �������� �� �ִ�.
�̸� �Լ� �������̵� �̶�� �Ѵ�.
�Լ��� ������ �Ҷ��� ��ȯŸ��, ���ڰ� ��� �����ؾ� �Ѵ�. �Լ� �̸��� �翬�� �����ؾ� �Ѵ�.
�Լ� ������ �̱� ������ �� �״�� �ش� �Լ��� �ڽ�Ŭ������ �ٽ� ���� �ڽ�Ŭ������ �Լ���
ȣ�� �� �� �ֵ��� ������ִ� ����� ���̴�.
�Ҹ��ڴ� ��� Ŭ������ ����ǰ� �̸��� �ٸ��� �Ҹ����̹Ƿ� �θ�Ŭ���� �Ҹ��ڿ� virtual�� �ٿ��ָ�
�ڽ�Ŭ������ �ڵ����� �����ǵ� �Ҹ��ڷ� �ν��� �ǰ� �ȴ�.

�Ϲ� ����Լ��� virtual�� �ٿ��� �����Լ��� ������ָ� �ڽ�Ŭ������ �����Ǹ� �ص� �ǰ� ���ص� �ȴ�.
���� CChildŬ������ �����ǰ� �Ǿ��ְ� CChild1 Ŭ������ �����ǰ� �ȵǾ��� ���
�����Լ� ���̺� ����Ǵ� �Լ��� �ּҴ� CChild Ŭ������ �̿��ؼ� ������ ��ü�� CChildŬ������
�������� �Լ��� �ּҰ� ���� �ǰ�
CChild1 Ŭ������ �̿��ؼ� ������ ��ü�� �����ǰ� �ȵǾ� �����Ƿ� CParent Ŭ������ ���ǵ� �Լ���
�ּҰ� ���� �Ǵ� ���̴�.

�׷��� �����Լ��� ȣ���� �ϰ� �Ǹ� ���� �����Լ������� �Ǵ��ϰ� �����Լ���� �����Լ� ���̺���
���� �Լ��� �ּҸ� ã�ƿ� �Ŀ� �ش� �Լ��� �ּҸ� �̿��Ͽ� �Լ�ȣ���� �����ϰ� �ǹǷ�
CChild Ŭ������ �̿��ؼ� ������ ��ü�� �����Լ����̺� CChildŬ������ �����ǵ� �Լ��� �ּҰ� ��
�����Ƿ� �����ǵ� �Լ��� ȣ�����ְ�
CChild1 Ŭ������ �̿��ؼ� ������ ��ü�� �����Լ����̺� CParentŬ������ ����� �Լ��� �ּҰ�
�� �����Ƿ� �ش� �Լ��� ȣ���ϰ� �Ǵ� ���̴�.

*/
class CParent
{
public:
	CParent()
	{
		std::cout << "CParent ������" << std::endl;
	}

	virtual ~CParent()
	{
		std::cout << "CParent �Ҹ���" << std::endl;
	}

public:
	int	m_Number1;

private:	// �ڽ�Ŭ���������� ������ �Ұ����ϴ�.
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
		std::cout << "CParent1 ������" << std::endl;
	}

	~CParent1()
	{
		std::cout << "CParent1 �Ҹ���" << std::endl;
	}

public:
	int	m_Number3;
};

class CChild2 : public CParent, CParent1
{
public:
	CChild2()
	{
		std::cout << "CChild2 ������" << std::endl;
		//m_Number3 = 4040;
	}

	~CChild2()
	{
		std::cout << "CChild2 �Ҹ���" << std::endl;
	}
};

class CChild : public CParent
{
public:
	CChild()
	{
		std::cout << "CChild ������" << std::endl;
		m_Number1 = 100;
		//m_Number2 = 300;
		m_Number3 = 500;

		m_NumberPointer = new int;
	}

	~CChild()
	{
		std::cout << "CChild �Ҹ���" << std::endl;
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
		std::cout << "CChild���� �ִ� Output" << std::endl;
	}
};

class CChild1 : private CParent
{
public:
	CChild1()
	{
		std::cout << "CChild1 ������" << std::endl;
		m_Number1 = 100;
		m_Number3 = 300;
	}

	~CChild1()
	{
		std::cout << "CChild1 �Ҹ���" << std::endl;
	}
};

class CChildChild : public CChild
{
public:
	CChildChild()
	{
		std::cout << "CChildChild ������" << std::endl;
		m_Number1 = 100;
		m_Number3 = 300;
	}

	~CChildChild()
	{
		std::cout << "CChildChild �Ҹ���" << std::endl;
	}
};

class CChildChild1 : public CChild1
{
public:
	CChildChild1()
	{
		std::cout << "CChildChild1 ������" << std::endl;
		//m_Number1 = 100;
		//m_Number3 = 300;
	}

	~CChildChild1()
	{
		std::cout << "CChildChild1 �Ҹ���" << std::endl;
	}
};


// ��������� ���� ��� ũ�⸦ ���ϸ� 1����Ʈ�� ���´�.
// ����ü�� Ŭ������ �ش� Ÿ���� �̿��Ͽ� ������ ������ �� �ֱ� ������
// �ּ� ����Ʈ�� 1����Ʈ�� ���� ���� ������ �����ϰ� ���ش�.
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

	// CChild Ŭ������ �����Ҵ��ϰ� �ش� �޸� �ּҸ� CParent ������ Ÿ�Կ� �����صξ���.
	// ���� ��Ӱ��迡 �ִ� Ŭ�����̱� ������ �̷� ����ȯ�� ������ ���̴�.
	CParent* Child = new CChild;
	//CParent* Child1 = new CChild1;
	CParent* Child2 = new CChild2;
	CParent* ChildChild = new CChildChild;
	//CParent* ChildChild1 = new CChildChild1;

	std::cout << "=================Test output=================\n";

	Child->Output();
	Child->CParent::Output();
	ChildChild->Output();

	// Child�� �ٺ������� CParent* Ÿ���� �����̴�.
	// �ٿ�ĳ������ �Ҷ��� ���� �����ؼ� ����ؾ� �Ѵ�.
	// �ֳ��ϸ� Child�� CChildŸ�� Ŭ������ ��ü�� ������ ���̴�.
	// �׷��� ���� �̰��� CChild1 Ÿ������ ����ȯ�� �ϸ�
	// CChild ���� -> CParent* ����ȯ -> CChild1 ����ȯ �� �Ǿ� ������ �Ǵ°��̴�.
	// �� ��ü�� �ٺ������� CChild ��ü�� ������ ���̹Ƿ� CChild1 Ÿ������ ����ȯ�Ͽ� �����
	// ��� ���� ������ �߻���ų �� �ִ�.
	// �׷��Ƿ� �ٿ�ĳ������ �Ҷ��� �׻� ���� ���� ���� ���� ���� ���� ���� ���� �ؾ� �˴ϴ�.
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
