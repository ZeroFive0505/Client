#include <iostream>

/*
* ����ü: ����� �̸��� �ο����� �� �ִ� �����̴�.
* enum ���ϴ� ����ü �̸� {}
* 
* ����ü �̸��� ��� �ȴ�.
* ���� ����ü �̸��� �ۼ����شٸ� ����ü �̸� �ش� ����ü�� ����Ÿ���� �ȴ�.
* ��, ����ڰ� ����Ÿ���� ���� �� �ְ� �Ǵ� ���̴�.
* 
* ����ü�� ������ ������ �⺻���� 0���� ������ �ȴ�. 1�� �����Ǹ� ���� �ο��Ǵ� ���̴�.
*/

enum
{
	JOB_KNIGHT = 0,
	JOB_ARCHER,
	JOB_MAGE
};

enum JOB
{
	JOB_KNIGHT1 = 10,
	JOB_ARCHER1,
	JOB_MAGICION1
};

enum JOB1
{
	JOB1_KNIGHT,
	JOB1_ARCHER = 30,
	JOB1_MAGICION
};


enum class JOB2
{
	Knight,
	Archer,
	Magicion
};

int main()
{
	/*
	* ������ ����� : �����ڸ� �ٿ��� ����� �� �ִ�.
	* ��� �����ڿ� ���� ����� �����ϴ�.
	*/

	int buf = 0;

	// buf = buf + 1
	buf += 1;

	buf |= 2;

	// ���� / ������ �ϰ� �� ��� �Ҽ��� �ڸ��� ©���� ������ �����κи� ���Եȴ�.
	// ����� �Ǽ��� ������ �ϱ� ���ؼ��� ���� �ϳ��� �Ǽ����� �Ѵ�.
	// 3.f�� �ϰ� �Ǹ� 3.0f�� ����. 0�� ������ �� �ִ�.
	buf = 10 / 3;

	float num = 10 / 3.0f;

	std::cout << num << "\n";

	std::cout << buf << "\n";

	// ������ ���� 3.3333�� ������ �ǰ����� ������ 3.3333�̶�� ���� ������ ��� �Ҽ��� �ڸ��� ©���Եȴ�.
	// �Ǽ��� ������ �������ְ� �� ��� ����ȯ�̶�� ���� �߻��ϰ� �ȴ�.
	// ĳ������ �� �� ��� �Ͻ������� ����ȯ�� �̷����� �� ��� �߻��ϰ� �ȴ�. ���� ��������� ĳ������ �ؼ� ��� ���ش�.
	buf = (int)(10 / 3.0f);

	/*
	* ���� ������: ++, -- 2���� ������ �����Ѵ�.
	* ++: 1 ���� ��Ų��.
	* --: 1 ���� ��Ų��.
	* 
	* ���� �տ� ���� �����ڸ� ���̰� �Ǹ� ��ġ������ �ǰ�
	* ���� �ڿ� ���� �����ڸ� ���̰� �Ǹ� ��ġ������ �ȴ�.
	* 
	* ��ġ����: ���� ���� 1 ���� ��Ű�� �ش翬���� �����Ѵ�.
	* ��ġ����: �ش� ������ �����ϰ� ���� 1 ���� ��Ų��.
	*/

	buf = 100;
	buf++;

	std::cout << buf << "\n";

	buf--;
	std::cout << buf << "\n";

	++buf;
	std::cout << buf << "\n";

	--buf;
	std::cout << buf << "\n";

	std::cout << ++buf << "\n";
	std::cout << buf++ << "\n";

	std::cout << buf << "\n";

	/*
	* ���׿�����: ���� ? ��1 : ��2 �� ���·� ����� �ȴ�.
	* ������ true��� ��1�� �ǰ� false��� ��2�� �Ǵ� ������ �������̴�.
	*/

	buf = 100;

	int number1 = buf < 10 ? 100 : 200;

	std::cout << number1 << "\n";

	std::cout << "size of char : " << sizeof(char) << "\n";
	std::cout << "size of int : " << sizeof(int) << "\n";
	std::cout << "size of __int64 : " << sizeof(__int64) << "\n";
	std::cout << "size of float : " << sizeof(float) << "\n";
	std::cout << "size of double : " << sizeof(double) << "\n";
	std::cout << "size of long long : " << sizeof(long long) << "\n";

	/*
	* ���ǹ�: Ư�� ������ üũ�Ͽ� true���� false������ ���� Ư�� ������ �� �������� �Ǵ��Ҷ� ����Ѵ�.
	* if (����)
	* { ������ true�϶� ����Ǵ� �ڵ� }
	* 
	* {}�� �ڵ� ����̶�� �Ѵ�.
	* 
	* else: ���������� ����� �� ����.
	* �ݵ�� if�� �Բ� ����� �Ǿ�� �Ѵ�.
	* else�� �������� ���� �� ����.
	* else�� if�� ������ false�� ��� �����ϰ� �ȴ�.
	* 
	* else if(���ǽ�): ���������� ����� �� ����.
	* if�� ���ǽ��� false�� ��� �Ʒ��� else if ���ǽ��� ���������� �˻��Ѵ�.
	* if�� else if�� ���ǽĵ��� ���ʷ� �����ϸ� true�� ������ ��ü ���� ������ ���������� �ȴ�.
	*/

	buf = 15;

	if (buf < 10)
		std::cout << "buf < 10\n";
	else if (buf < 20)
		std::cout << "10 < buf < 20\n";
	else if (buf < 30)
		std::cout << "20 < buf < 30\n";
	else
		std::cout << "buf > 30\n";

	/*
	* switch��: � �������� �Ǵ��ؼ� ���۽�ų�� Ȱ��ȴ�.
	* switch(����(��))
	* {
	*	case üũ�ϰ����ϴ� ��(���):
	*		break;
	* }
	* 
	* break�� ������ �Ǹ� switch���� ���������� �ȴ�.
	*/

	number1 = 3;
	const int number2 = 2;
	switch (number1)
	{
	case 1:
		std::cout << "number1 == 1\n";
		break;
	case number2:
		std::cout << "number1 == 2\n";
		break;
	case 3:
		std::cout << "number1 == 3\n";
		break;
	case 4:
		std::cout << "number1 == 4\n";
	case 5:
		std::cout << "number1 == 5\n";
		break;
	default:
		break;
	}

	// 0 : ��� 1 : �ü� 2 : ������
	int Job = 0;

	switch (Job)
	{
	case 0:
		std::cout << "Knight\n";
		break;
	case 1:
		std::cout << "Archer\n";
		break;
	case 2:
		std::cout << "Mage\n";
		break;
	}

	// ����ü Ÿ���� ������ �����ϸ� �ش� ������ �ش� ����ü Ÿ���� ������ ���� �� �ִ� ������
	// ��������� �ȴ�.
	JOB Job1 = JOB_ARCHER1;

	std::cout << "Size of JOB : " << sizeof(JOB) << "\n";

	JOB2 Job2 = JOB2::Knight;

	switch (Job2)
	{
	case JOB2::Knight:
		std::cout << "Knight\n";
		break;
	case JOB2::Archer:
		std::cout << "Archer\n";
		break;
	case JOB2::Magicion:
		std::cout << "Mage\n";
		break;
	}
	
	switch ((JOB2)Job)
	{
	case JOB2::Knight:
		std::cout << "Knight\n";
		break;
	case JOB2::Archer:
		std::cout << "Archer\n";
		break;
	case JOB2::Magicion:
		std::cout << "Mage\n";
		break;
	}

	if (Job2 == JOB2::Knight)
		std::cout << "Knight\n";
	else if (Job2 == JOB2::Archer)
		std::cout << "Archer\n";
	else if (Job2 == JOB2::Magicion)
		std::cout << "Magicion\n";

	return 0;
}