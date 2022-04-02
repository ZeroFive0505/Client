// ����� ���� ��������� include �Ҷ��� ""�ȿ� �ۼ����ش�.
#include "Player.h"
#include "Monster.h"
#include "Template.h"

// ��ȯ ���� : ������ϳ��� ���� include�� ���־��� ��츦 ���Ѵ�.
// ��ȯ������ �ذ��ϴ� ��� : ���漱������ �ذ� ����.

//void OutputNumber(int Number)
//{
//	std::cout << Number << "\n";
//}
//
//void OutputNumber(float Number)
//{
//	std::cout << Number << "\n";
//}

/*
* ���ø��� Ÿ���� ���������� �ٲ��� �� ����� �� �ִ� �����̴�.
* template <typename ���ϴ� �̸�>
* template <class ���ϴ� �̸�> �̷������� ����Ѵ�.
* template <typename T, typename T1> �̷������� ��� Ÿ���� ���������� ������ �� �ִ�.
*/

template <typename T>
void Output(T Number)
{
	std::cout << Number << "\n";
	// typeid(Ÿ��)�� �־��ָ� �ش� Ÿ���� ���ڿ��� ���� �� �ִ�.
	std::cout << typeid(T).name() << "\n";
	// typeid(����)�� �־��ָ� �ش� Ÿ���� ���ڿ��� ���� �� �ִ�.
	// name() �Լ��� ���ڿ��� ������ �ȴ�.
	std::cout << typeid(Number).name() << "\n";
}

template<typename T, typename T1>
void Output(T Number, T1 Number1)
{
	std::cout << Number << "\n";
	std::cout << Number1 << "\n";

	std::cout << typeid(T).name() << "\n";
	std::cout << typeid(T1).name() << "\n";

	// hash_code() �Լ��� �̿��ؼ� �� Ÿ���� ������ ��ȣ�� ���� �� �ִ�.
	std::cout << typeid(T).hash_code() << "\n";
	std::cout << typeid(T1).hash_code() << "\n";
}

/*
* ���ø� Ư��ȭ : ���ø��� �پ��� Ÿ������ ��ȯ�Ǿ� ���� �� �ִ�.
* �پ��� Ÿ�� �� ���� ���ϴ� Ư�� Ÿ�Ը� �Լ� ��ü�� ���� ������ �� �ִ� ����� �����ϴµ� �װ���
* ���ø� Ư��ȭ��� �Ѵ�.
* �̸��� �Ű������� �����ؾ��Ѵ�.
* T�� 3�� ��������ϱ� �����ϰ� 3����
*/

template<typename T>
T Add(T Number1, T Number2)
{
	return Number1 + Number2;
}

// float Ÿ���� ��� Add�Լ��� �� �� Ư���ϰ� �����غ����� �Ѵ�.
template <>
float Add(float Number1, float Number2)
{
	std::cout << "float type.\n";
	return Number1 + Number2;
}

template <>
double Add(double Number1, double Number2)
{
	std::cout << "double type.\n";
	return Number1 + Number2;
}

double Add(double Number1, double Number2)
{
	std::cout << "Add Function\n";
	return Number1 + Number2;
}

int main()
{
	std::cout << "Output\n";
	CPlayer player;

	player.Output();

	CMonster monster;

	// <Ÿ��>�� ���� T�� �� Ÿ���� ������ �� �ִ�.
	Output<int>(10);
	Output<float>(3.14f);

	// �Ʒ�ó�� �� ������ Ÿ������ �ڵ����� Ÿ���� ������ ���� �ִ�.
	Output(5.23);

	Output<int, float>(3, 3.14f);

	system("cls");

	CTemplate<int, 100> temp;
	CTemplate<float> temp1;

	temp.TestTemplateFunction<CPlayer>();
	temp.TestTemplateFunction<CMonster>();


	std::cout << Add<int>(10, 20) << "\n";
	std::cout << Add<float>(0.5f, 0.5f) << "\n";
	std::cout << Add<double>(0.5, 0.5) << "\n";
	std::cout << Add(0.5, 0.5) << "\n";

	// �Ʒ��� #define COUNT 10 ���� �Ǿ��ִ� define�� ����ߴ�.
	// �� ��� �������� �ϱ����� Visual Studio�� 10�� ���⿡ �ٿ���
	// std::cout << 10 << "\n";
	// �̷��� �ڵ带 ��ü�ϰ� �������� �ϰ� �ȴ�.
	std::cout << COUNT << "\n";

	// std::cout << "PrintDefine\n";
	PRINTDEFINE;

	return 0;
}