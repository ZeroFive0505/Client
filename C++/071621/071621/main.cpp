#include <iostream>

/*
* ����ü: �پ��� Ÿ���� �������� �ϳ��� ��� ����� �� �ְ� ������ִ�
* ����� �����Ѵ�.
* ��������� ���� Ÿ���� ������ִ� ����̴�.
* ����
* 
* struct ���ϴ� ����ü �̸�
* {
*	���ϴ� �������� �ۼ��Ѵ�.
* }
*/

/*
* ���ڿ�(���� ������)�� ǥ���Ҷ� char �迭�� �̿��ؼ� ǥ���� �� �ִ�.
* ��, ���ڿ��� ������ٶ� ���ڿ��� ���� �ݵ�� \0 �̾�� �Ѵ�.
*/

enum class JOB
{
	Knight,
	Archer,
	Magicion
};


// ����ü ��� ���� ���
// ��ǻ�ʹ� ����ü�� 4����Ʈ ������ �����.(�츮�� ������ ����������)
// 4����Ʈ�� �����ϴ� ������ 2�� ��������̱⿡ �޸� ������ ȿ�����̴�.
// ����ü�� ũ�⸦ ������ �⺻���� ����ü�� ����� ������ �߿��� ���� ũ�Ⱑ ū ���� ��������
// �����ŭ Ŀ����.
struct Player
{
	// __int64 num1;
	char name[32];
	//char test1;
	//char test2;
	JOB job;
	int attack;
	int armor;
	int hp;
	int hpMax;
	int mp;
	int mpMax;
};

struct Test
{
	char test1;
	char test2;
};

int main()
{
	char name[128] = {};
	name[0] = 'A';
	name[1] = 'B';
	name[2] = '\0';

	// ���ڿ��� ����ҋ��� �迭 �̸��� ����ؼ� ����Ѵ�.
	std::cout << name << "\n";

	std::cout << sizeof(Player) << "\n";
	std::cout << sizeof(Test) << "\n";

	Player player = {};
	Player playerArr[100] = {};

	player.attack = 100;
	player.armor = 30;
	player.hp = 1000;
	player.hpMax = 1000;

	player.hp -= 30;

	std::cout << "Attack : " << player.attack << "\n";

	return 0;
}