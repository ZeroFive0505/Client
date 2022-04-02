
#include <iostream>
#include "HashTable.h"

/*
HashTable : ���� ����Ǵ� �����ͺ��� �ξ� ū �޸� ������ �������� �����͸� Hash �Լ��� ���ؼ�
�ε����� ���Ѵ��� �����ϴ� ����̴�. ���� �����Ͱ� ����ɶ� �������� ����Ǵ� ���� �ƴ϶�
�迭�� ��������� �ΰ� ������ �Ǵ� ����̴�. ��, �޸𸮰� ����Ǵ� ���� ���� �� ������ �޸𸮸�
����ؼ� Ž���ӵ��� �״�ȭ ��Ű�� �˰����̴�.
*/

int main()
{
	CHashTable<const char*, const char*>	Table;

	Table.insert("�߽���", "����");
	Table.insert("���", "����");
	Table.insert("Ƽ��", "���۳�");
	Table.insert("Ʈ���ٹ̾�", "�̱�������");
	Table.insert("����", "�����ƴ³�");
	Table.insert("ABC", "ABC");
	Table.insert("ACB", "ACB");

	std::cout << Table["�߽���"] << std::endl;
	std::cout << Table["Ʈ���ٹ̾�"] << std::endl;

	Table["Test"] = "Test��.";
	Table["Ʈ���ٹ̾�"] = "������ ������";
	std::cout << Table["Test"] << std::endl;
	std::cout << Table["Ʈ���ٹ̾�"] << std::endl;
	std::cout << Table["Ƽ��"] << std::endl;


	CHashTable<const char*, const char*>::iterator	iter = Table.Find("Ƽ��");

	if (Table.IsValid(iter))
		std::cout << "Ƽ��� �������� Ű���Դϴ�." << std::endl;

	iter = Table.Find("��ũ��");

	if (Table.IsValid(iter))
		std::cout << "��ũ���� �������� Ű���Դϴ�." << std::endl;

	else
		std::cout << "��ũ���� �߸��� Ű���Դϴ�." << std::endl;

	return 0;
}
