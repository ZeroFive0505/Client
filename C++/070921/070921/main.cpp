#include <iostream>

/*
* 열거체: 상수에 이름을 부여해줄 수 있는 문법이다.
* enum 원하는 열거체 이름 {}
* 
* 열거체 이름은 없어도 된다.
* 만약 열거체 이름을 작성해준다면 열거체 이름 해당 열거체의 변수타입이 된다.
* 즉, 사용자가 변수타입을 만들 수 있게 되는 것이다.
* 
* 열거체는 나열한 값들이 기본으로 0부터 시작이 된다. 1씩 증가되며 값이 부여되는 것이다.
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
	* 연산자 축약형 : 연산자를 줄여서 사용할 수 있다.
	* 모든 연산자에 대해 축약이 가능하다.
	*/

	int buf = 0;

	// buf = buf + 1
	buf += 1;

	buf |= 2;

	// 정수 / 정수를 하게 될 경우 소수점 자리가 짤리고 무조건 정수부분만 남게된다.
	// 결과로 실수가 나오게 하기 위해서는 둘중 하나라도 실수여야 한다.
	// 3.f를 하게 되면 3.0f와 같다. 0을 생략할 수 있다.
	buf = 10 / 3;

	float num = 10 / 3.0f;

	std::cout << num << "\n";

	std::cout << buf << "\n";

	// 연산한 값은 3.3333이 나오게 되겠지만 정수에 3.3333이라는 값을 저장할 경우 소수점 자리가 짤리게된다.
	// 실수를 정수에 대입해주게 될 경우 형변환이라는 것이 발생하게 된다.
	// 캐스팅을 안 할 경우 암시적으로 형변환이 이뤄져서 즉 경고가 발생하게 된다. 따라서 명시적으로 캐스팅을 해서 경고를 없앤다.
	buf = (int)(10 / 3.0f);

	/*
	* 증감 연산자: ++, -- 2가지 연산을 지원한다.
	* ++: 1 증가 시킨다.
	* --: 1 감소 시킨다.
	* 
	* 변수 앞에 증감 연산자를 붙이게 되면 전치연산이 되고
	* 변수 뒤에 증감 연산자를 붙이게 되면 후치연산이 된다.
	* 
	* 전치연산: 먼저 값을 1 증감 시키고 해당연산을 진행한다.
	* 후치연산: 해당 연산을 진행하고 값을 1 증감 시킨다.
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
	* 삼항연산자: 조건 ? 값1 : 값2 의 형태로 사용이 된다.
	* 조건이 true라면 값1이 되고 false라면 값2가 되는 형태의 연산자이다.
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
	* 조건문: 특정 조건을 체크하여 true인지 false인지에 따라 특정 동작을 할 것인지를 판단할때 사용한다.
	* if (조건)
	* { 조건이 true일때 실행되는 코드 }
	* 
	* {}를 코드 블록이라고 한다.
	* 
	* else: 독립적으로 사용할 수 없다.
	* 반드시 if와 함께 사용이 되어야 한다.
	* else가 여러가개 들어올 수 없다.
	* else는 if의 조건이 false일 경우 동작하게 된다.
	* 
	* else if(조건식): 독립적으로 사용할 수 없다.
	* if의 조건식이 false일 경우 아래의 else if 조건식을 순차적으로 검사한다.
	* if나 else if의 조건식들중 차례로 진행하며 true가 나오면 전체 조건 구문을 빠져나오게 된다.
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
	* switch문: 어떤 값인지를 판단해서 동작시킬때 활용된다.
	* switch(변수(값))
	* {
	*	case 체크하고자하는 값(상수):
	*		break;
	* }
	* 
	* break를 만나게 되면 switch문을 빠져나가게 된다.
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

	// 0 : 기사 1 : 궁수 2 : 마법사
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

	// 열거체 타입의 변수를 생성하면 해당 변수는 해당 열거체 타입의 값만을 가질 수 있는 변수가
	// 만들어지게 된다.
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