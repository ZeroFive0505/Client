#include <iostream>
#include <time.h>
/*
* �Լ� : ���� �ڵ带 ������ ���۽�ų ��� �Լ��� �̸� �����ΰ� �ش� �Լ��� ���ϴ� ������ ȣ�⸸ �ϸ� �ش�
* �ڵ尡 ���۵� �� �ְ� ������ִ� ����̴�.
* 
* �Լ��� ����
* ��ȯŸ�� �Լ��̸�(����)
* {
*	
* }
* 
* �� ���·� ������ �ȴ�.
* 
* ��ȯŸ�� : �� �Լ��� Ư�� �ڵ带 �����ϰ� � ����� ȣ���� ������ ������ �ʿ䰡 �ִٸ�
* ���jŸ���� �̿��ؼ� ������ ���� Ÿ���� �����ϰ� ��ȯ���ָ� �ȴ�.
* ������ ��ȯ�ؾ� �Ѵٸ� ��ȯŸ���� int�� �ϰ� return ���ϴ� ��; �� ���ؼ� �ش� �Լ���
* ��� �������� ���� ��ȯ�ϰԲ� ������ش�.
* 
* �Ǽ��� ��ȯ�ؾ� �Ѵٸ� ��ȯ Ÿ���� float���� �����Ѵ�.
* 
* �Լ��̸� : �� �Լ��� ������ ��ɿ� �´� ��Ȯ�� �̸��� �ۼ����ش�.
* 
* ���� : �־ �ǰ� ��� �ȴ�. ���� �� �Լ��� ����Ҷ� �� �Լ��� �����͸� ������ �ʿ䰡 ���� ���
* ���ڸ� �̿��ؼ� �Լ��� �����͸� �����ϰ� �ȴ�.
* 
* ���ڴ� �ش� �Լ� �������� ����� �� �ִ� �����̴�.
* ���ڴ� �ش� �Լ��� ȣ���� �Ǹ� �׶� �޸𸮿� ������ ��������� �ǰ� �ش� �Լ��� ����Ǹ� �޸𸮿��� ���ŵȴ�.
* 
* �޸𸮰� ���ŵǴ� ����� �Լ�ȣ��Ծ࿡ ���ؼ� ���Ű� �ȴ�.
* 
* �Լ� ȣ��Ծ� �˻��ؼ� Ȯ��.
* 
* ��ȯŸ���� �̿��ؼ� �Լ��� ����� ��ȯ�ϴ� ���¸� Call By Value
* �Լ��� ���ڿ� ������ Ÿ���� �̿��ؼ� ������ ������ �������Ͽ� ����� ��ȯ�ϴ� ���¸� Call By Address
*/

int Add(int num1, int num2)
{
	return num1 + num2;
}

// ��ȯŸ�Կ� void�� ����ϸ� ��ȯ���� return ���ص� �ȴ�.
// return�� �� �Լ��� �����Ű�� �� �Լ��� ����� �ᰡ���� ��ȯ�Ҷ� ����Ѵ�.
// void Ÿ���� ��ȯŸ���� ���� �Լ������� return; �� ���ؼ� ���Ҷ� �� �Լ��� �����ų ���� �ִ�.
void Output()
{
	std::cout << "Output Function" << "\n";
}

void Output1(int num)
{
	std::cout << num << "\n";
}


void SetArray(int* pArray)
{
	for (int i = 0; i < 10; i++)
		pArray[i] = i + 1;
}

void OutputArray(int* pArray)
{
	for (int i = 0; i < 10; i++)
		std::cout << pArray[i] << " ";
	std::cout << "\n";
}

/*
* �Լ��� �����ε� : ���� �̸����� �������� �Լ��� ������� �� �ִ� ����̴�.
* ��, �̸��� ���ٸ� �ݵ�� ������ ���� Ȥ�� Ÿ���� �޶�� �Ѵ�.
* ��ȯŸ���� �����ε��� ������ ����.
*/

void ChangeNumber(int number)
{
	number = 9999;
}

void ChangeNumber(float number)
{
	number = 3.14f;
}

void ChangeNumber(int* number)
{
	*number = 9999;
}

void ChangeNumber(int num1, int num2)
{
	num1 = 100;
	num2 = 200;
}

//int ChangeNumber(int num)
//{
//	num = 30;
//	return num;
//}

void SetNumber(int* pNumber)
{
	for (int i = 0; i < 25; i++)
	{
		pNumber[i] = i + 1;
	}
}

void Shuffle(int* pNumber)
{
	for (int i = 0; i < 100; i++)
	{
		int idx1 = rand() % 25;
		int idx2 = rand() % 25;

		int temp = pNumber[idx1];
		pNumber[idx1] = pNumber[idx2];
		pNumber[idx2] = temp;
	}
}

void OutputBingo(int* pNumber)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			std::cout << pNumber[i * 5 + j] << "\t";
		}
		std::cout << "\n";
	}

	std::cout << "\n";
}

int main()
{
	// �Լ��� ȣ�����ش�. �Լ��� ȣ���Ҷ� Add�Լ��� ���ڷ� 2���� ������ �����Ƿ�
	// �ݵ�� 2���� ������ �־��־���Ѵ�.
	// Add�Լ����� ��ȯ�� ���� num�� �־��ش�.
	int num = Add(10, 20);
	int num1 = 102;
	srand(time(0));

	std::cout << num << "\n";
	std::cout << Add(50, 130) << "\n";
	std::cout << Add(100, 100) << "\n";

	Output();

	Output1(900);

	ChangeNumber(num);

	ChangeNumber(num, num1);

	std::cout << "Number : " << num << "\n";

	ChangeNumber(&num);

	std::cout << "Number : " << num << "\n";

	int arr[10] = {};

	SetArray(arr);

	OutputArray(arr);

	int myBingo[25] = {};
	int aiBingo[25] = {};
	
	SetNumber(myBingo);
	SetNumber(aiBingo);

	Shuffle(myBingo);
	Shuffle(aiBingo);

	OutputBingo(myBingo);
	OutputBingo(aiBingo);

	return 0;
}