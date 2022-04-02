#include <iostream>

/*
* �������� ���α׷��� : ���������� ó�����ִ� ���α׷��� ����̴�.
* 
* ��ü���� ���α׷���(OOP) : ��ü(��� �繰)�� �����Ͽ� ��ü�� �����Ͽ� �ϳ��� �ϼ��� ���α׷���
* �������� ����̴�.
* C++������ ��ü���� ���α׷����� �����ϱ� ���� �������� Ŭ������ �����Ѵ�.
* 
* ĸ��ȭ : ����ü�� ���� ���� ������ Ȥ�� �Լ��� ��� ����� �� �ְ� ������ִ� �Ӽ��̴�.
* ����ȭ : ����ü�� Ŭ������ ����� �ܺο� �����ϴ� ������ ������ �� �ִ� �Ӽ��̴�.
* - public: Ŭ���� Ȥ�� ����ü�� ����(Ŭ������ ����ü�� ��������ִ� �Լ� ��)�� �ܺ�(Ŭ������ �Լ� ���� �ƴ� �ٸ� Ŭ����
* Ȥ�� �ٸ� �Լ� ��)���� ��� ������ ������ �Ӽ��̴�.
* ����ü�� �ƹ��͵� �� �ٿ��� ��� �⺻���� public �̴�.
* - private: Ŭ���� Ȥ�� ����ü�� ���ο����� ������ �����ϰ� �ܺο����� ������ �Ұ����ϴ�.
* Ŭ������ �ƹ��͵� �� �ٿ��� ��� �⺻���� private �̴�.
* 
* - protected: ����Ҷ� ����
* ��Ӽ� : ����Ҷ� ����.
* ������ : ����Ҷ� ����.
* 
* Ŭ���� : C++���� ��ü������ ���� �����ϴ� �����̴�.
* ����ü�� ���������� �پ��� ������ �Լ����� Ŭ���� ���ο� �����ΰ� ����� �����ϴ�.
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
	// Ŭ������ ��ü�� ����� ���� �����̸� ��ü�� �ƴϴ�.
	// ������ : �� Ŭ������ �̿��ؼ� ��ü�� �����Ҷ� �ڵ����� ȣ���� �Ǵ� �Լ��̴�.
	// �����ڿ����� Initializer�� ����ؼ� ��������� �ʱ�ȭ �� �� �ִ�.
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


	// ���� ������.
	CPlayer(const CPlayer& player)
	{
		// �ܼ��ϰ� ������ ���鸸 �����ϴ� ����� ���� ������ �Ѵ�.
		m_Number1 = player.m_Number1;
		m_Number2 = player.m_Number2;
		m_Number3 = player.m_Number3;

		// �̷� ��� �� ��ü�� ��� ���� �迭�� �޸� �ּҸ� ���� �ǹǷ�
		// �Ҹ��ڿ��� �ش� �����迭�� �޸𸮿��� �����Ҷ� ������ �߻��� �� �ִ�.
		// �Ʒ�ó�� �޸� ������ ���ο� �������� ����� �� ���� �ȿ� �ִ� �����͵鸸
		// ���縦 �ϴ� ����� ���� ������ �Ѵ�.
		// m_pArray = player.m_pArray;
		m_pArray = new int[100];
		for (int i = 0; i < 100; i++)
		{
			m_pArray[i] = player.m_pArray[i];
		}
	}

	// �Ҹ��� : �� Ŭ������ �̿��ؼ� ������ ��ü�� �޸𸮿��� �����ɶ� �ڵ����� ȣ���� �Ǵ� �Լ��̴�.
	// �Ҹ��ڴ� �����ε� �Ұ���.
	~CPlayer()
	{
		delete[] m_pArray;
		std::cout << "CPlayer Destructor\n";
	}

public:
	/*
	* Ŭ������ ����Լ� : Ŭ������ ����Լ��� �Ϲ��Լ��� ���������� �Լ� �ּҰ� 1���� ������ �ȴ�.
	* �׷��� Ŭ������ ����Լ������� �� ����Լ��� ȣ���� ��ü�� ����������� ���� ��Ȯ�ϰ� �ν��ϰ� ����� �����ϴ�.
	* �� ������ this ��� �� �����̴�.
	* this�� �ڱ� �ڽſ� ���� �������̴�.
	* this->m_Number1 �̷��� ����ؾ� �ϴµ� this->�� ������ ����
	* 
	* this�� �� ����Լ��� ȣ���Ҷ� ȣ���ϴ� ��ü�� ������ �ǰ� �ȴ�.
	* �׷��� ������ �Ź� �� �Լ��� ȣ���Ҷ����� this���� ȣ���ϴ� ��ü�� ������ �Ǹ�,
	* �Ź� �ٸ� ��ü�� ȣ���Ѵٸ� �ٸ� ��ü��� �Ź� this�� �ٲ�� �� ���̴�.
	*/
	void Output()
	{
		std::cout << "Number1 : " << m_Number1 << "\n";
		std::cout << "This : " << this << "\n";
	}

private:
	// ���ο����� ȣ���� �����ϴ�.
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

	// �Ϲ� ������ ȣ��
	CPlayer* pPlayer = new CPlayer;

	std::cout << "Test\n";

	delete pPlayer;

	// �Ϲ� ������ ȣ��
	CPlayer player;

	// intŸ�� 1���� ���ڷ� �޴� ������ ȣ��.
	CPlayer player1(2020);

	// intŸ�� 3���� ���ڷ� �޴� ������ ȣ��.
	CPlayer player2(10, 20, 30);

	// player2 ��ü�� Output �Լ��� ȣ���Ѵ�.
	// player1.Output()�� ȣ���ϸ� this�� player1�� �ּҸ� ������ ���� �Ǹ�
	// player2.Output()�� ȣ���ϸ� this�� player2�� �ּҸ� ������ ���� �ȴ�.
	player1.Output();
	player2.Output();

	std::cout << "player1 address : " << &player1 << "\n";
	std::cout << "player2 address : " << &player2 << "\n";

	return 0;
}