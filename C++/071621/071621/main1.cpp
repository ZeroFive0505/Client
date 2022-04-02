#include <iostream>

int main()
{
	/*
	* 포인터: 메모리에 공간을 만들면 반드시 주소가 부여된다.
	* 모든 변수 타입은 포인터 타입을 가질 수 있다.
	* 포인터 타입의 변수는 해당 타입의 메모리 주소를 가질 수 있다.
	* int number; 를 선언했을때 주소가 1000번지라면 int의 포인터 타입 변수에
	* number의 주소인 1000번지를 저장할 수 있는 것이다.
	* 
	* 포인터 변수 선언방법
	* 
	* 변수타입 *포인터 이름;
	* 으로 선언한다.
	* 
	* int타입의 주소는 int포인터 타입의 변수에 저장할 수 있다.
	* 서로 타입이 다르다면 주소를 저장할 수 없는데 형변환을 통해서 저장은 가능하다.
	* 
	* x86으로 개발을 하게되면 32bit 시스템으로 개발하는 것이다.
	* x64로 개발을 하게되면 64bit 시스템으로 개발하는 것이다.
	* 
	* 32bit에서는 메모리 주소가 16진수 8자리로 표현되고 64bit에서는
	* 메모리 주소가 16진수 16자리로 표현이 된다.
	* 
	* 그래서 포인터 타입의 변수는 메모리 주소를 저장하는 변수이므로 어떤 타입의
	* 포인터 변수라도 무조건 32bit에서는 4바이트 64bit에서는 8바이트만큼의 공간을 차지하게 되는 것이다.
	* 
	* 포인터 변수가 다른 변수의 메모리 주소를 가지고 있으면 해당 변수를 참조한다고 한다.
	* 
	* 
	* 포인터 변수가 다른 변수의 메모리 주소를 저장하게 된다면 해당 주소에 접근하여
	* 값을 얻어오거나 변경할 수 있게 된다.
	* 이를 역참조라고 한다.
	*/

	int number = 100;
	int* pNumber = &number;
	// __int64 number64 = 1;
	float numberF = 3.14f;
	int number64 = 1;

	// 역참조는 포인터 변수 앞에 *을 붙여서 해당 포인터 변수가 가지고 있는 주소에 접근할 수 있다.
	
	std::cout << "number : " << number << "\n";
	*pNumber = 10;
	std::cout << "number : " << number << "\n";

	// 아래처럼 하면 int* 타입의 변수에 float 타입의 메모리 주소를 저장하려 하기 때문에 에러가 발생한다.
	// pNumber = &numberF
	// 
	// 아래의 경우 int* 타입으로 형변한을 해주었기 때문에 저장이 가능하다.
	// pNumber = (int*)&numberF
	// __int64* pNumber64 = (__int64*)&number64;



	std::cout << "number address : " << &number << "\n";
	std::cout << sizeof(int*) << "\n";
	std::cout << sizeof(__int64*) << "\n";
	std::cout << sizeof(char*) << "\n";

	/*
	* 포인터와 배열의 관계
	* 배열명은 포인터다. 배열의 이름은 해당 배열이 할당된 메모리의 시작주소를 의미한다.
	*/

	int arr[10] = {};
	int arr2[10][10] = {};

	std::cout << "arr start address : " << arr << "\n";
	std::cout << "arr2 start address : " << arr2 << "\n";

	// arr가 해당 배열의 시작 주소이므로 이를 포인터 변수에 저장해줄 수 있다.
	int* pArr = arr;
	int(*pArr2)[10] = arr2;

	arr[2] = 1010;
	pArr[2] = 3030;
	*(pArr + 2) = 4040;

	std::cout << arr[2] << "\n";

	// ""안에 문자열을 적어주면 const char* 타입으로 인식이된다.
	const char* pText = "Hello Char";

	std::cout << pText << "\n";

	/*
	* 포인터 연산: +, - 2가지를 제공한다.
	* 포인터 연산은 1을 더한다고 해서 무조건 메모리 주소가 1이 증가하는 개념이 아니다.
	* 
	* 포인터 변수에 1을 더해주게 된다면 해당 변수 타입의 크기만큼 증가하게 된다.
	* int 포인터일 경우 int의 크기인 4만큼 증가하게되는 것이다.
	* __int64 포인터라면 1을 더할 경우 __int64의 크기인 8만큼 증가하게 된다.
	*/

	std::cout << "pArr value : " << pArr << "\n";
	std::cout << "pArr + 1 value : " << pArr + 1 << "\n";
	std::cout << "pArr + 2 value : " << pArr + 2 << "\n";

	return 0;
}