#include <iostream>

/*
* 구조체: 다양한 타입의 변수들을 하나로 묶어서 사용할 수 있게 만들어주는
* 기능을 제공한다.
* 사용자정의 변수 타입을 만들어주는 기능이다.
* 형태
* 
* struct 원하는 구조체 이름
* {
*	원하는 변수들을 작성한다.
* }
*/

/*
* 문자열(문자 여러개)을 표현할때 char 배열을 이용해서 표현할 수 있다.
* 단, 문자열을 만들어줄때 문자열의 끝은 반드시 \0 이어야 한다.
*/

enum class JOB
{
	Knight,
	Archer,
	Magicion
};


// 구조체 멤버 맞춤 기능
// 컴퓨터는 구조체를 4바이트 단위로 맞춘다.(우리가 설정은 가능하지만)
// 4바이트를 권장하는 이유는 2의 배수단위이기에 메모리 관리에 효율적이다.
// 구조체의 크기를 잡을때 기본값은 구조체에 선언된 변수들 중에서 가장 크기가 큰 값을 기준으로
// 배수만큼 커진다.
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

	// 문자열을 출력할떄는 배열 이름만 사용해서 출력한다.
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