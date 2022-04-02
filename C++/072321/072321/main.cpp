// 사용자 정의 헤더파일을 include 할때는 ""안에 작성해준다.
#include "Player.h"
#include "Monster.h"
#include "Template.h"

// 순환 참조 : 헤더파일끼리 서로 include를 해주었을 경우를 말한다.
// 순환참조를 해결하는 방법 : 전방선언으로 해결 가능.

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
* 탬플릿은 타입을 가변적으로 바꿔줄 때 사용할 수 있는 문법이다.
* template <typename 원하는 이름>
* template <class 원하는 이름> 이런식으로 사용한다.
* template <typename T, typename T1> 이런식으로 몇개든 타입을 가변적으로 저장할 수 있다.
*/

template <typename T>
void Output(T Number)
{
	std::cout << Number << "\n";
	// typeid(타입)을 넣어주면 해당 타입을 문자열로 얻어올 수 있다.
	std::cout << typeid(T).name() << "\n";
	// typeid(변수)을 넣어주면 해당 타입을 문자열로 얻어올 수 있다.
	// name() 함수에 문자열이 들어오게 된다.
	std::cout << typeid(Number).name() << "\n";
}

template<typename T, typename T1>
void Output(T Number, T1 Number1)
{
	std::cout << Number << "\n";
	std::cout << Number1 << "\n";

	std::cout << typeid(T).name() << "\n";
	std::cout << typeid(T1).name() << "\n";

	// hash_code() 함수를 이용해서 이 타입의 고유한 번호를 얻어올 수 있다.
	std::cout << typeid(T).hash_code() << "\n";
	std::cout << typeid(T1).hash_code() << "\n";
}

/*
* 템플릿 특수화 : 템플릿은 다양한 타입으로 변환되어 들어올 수 있다.
* 다양한 타입 중 내가 원하는 특정 타입만 함수 자체를 새로 구현할 수 있는 기능을 제공하는데 그것을
* 템플릿 특수화라고 한다.
* 이름과 매개변수는 동일해야한다.
* T가 3개 들어있으니까 동일하게 3군데
*/

template<typename T>
T Add(T Number1, T Number2)
{
	return Number1 + Number2;
}

// float 타입일 경우 Add함수를 좀 더 특별하게 구성해보고자 한다.
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

	// <타입>을 통해 T에 들어갈 타입을 정해줄 수 있다.
	Output<int>(10);
	Output<float>(3.14f);

	// 아래처럼 들어간 인자의 타입으로 자동으로 타입이 정해질 수도 있다.
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

	// 아래는 #define COUNT 10 으로 되어있는 define을 사용했다.
	// 이 경우 컴퍼일을 하기전에 Visual Studio는 10을 여기에 붙여서
	// std::cout << 10 << "\n";
	// 이렇게 코드를 대체하고 컴파일을 하게 된다.
	std::cout << COUNT << "\n";

	// std::cout << "PrintDefine\n";
	PRINTDEFINE;

	return 0;
}