// 코드를 컴파일한다. -> 실행 파일을 만든다. -> 실행한다.
/*
* 앞에 #이 붙으면 전처리기
* 전처리기란 컴파일 전에 처리를 하는 기능들을 말한다.
* 
* #include는 원하는 헤더파일을 여기에 포함시켜주는 기능이다.
* iostream: C++ 표준 입출력 기능 외에 다양한 기능이 들어가있는 헤더파일이다.
*/
#include <iostream>

/*
* main은 C++의 진입접이다.
* 콘솔 프로젝트는 반드시 main이 있어야한다.
* main안에 들어간 코드만 동작되는 방식이다.
*/
int main()
{
	// cout : 콘솔창에 출력하는 기능을 제공한다.
	// cout은 std namespace안에 소속이 되어 있다.
	std::cout << "We have one more year to continue!\n";
	// \n : newline character
	// endl == \n
	std::cout << "We have two more years to continue!" << std::endl;

	/*
	* 주기억장치 : 메모리(램) 휘발성
	* 보조기억장치 : HDD, SSD 비휘발성
	* 
	* 컴퓨터 메모리 크기의 최소단위 : bit
	* 1bit == 0, 1 둘중에 하나의 값을 저장할 수 있는 공간.
	* 1byte == 8bit
	* 1Kbyte = 1024byte
	* 1Mbyte = 1024Kbyte
	* 1Gbyte = 1024Mbyte
	* 1Tbyte = 1024Gbyte
	* 
	* 변수 : 값을 변경할 수 있는 메모리 공간. 프로그램 작성시 어떤 정보를 저장하기 위해서 사용하는 것이 변수이다.
	* 메모리(램)에 공간이 만드렁지고 데이터를 저장하다가 프로그램이 종료되면 메모리에서 제거가 된다.
	* 
	* 컴퓨터에서 문자를 표현하는 방식
	* ASCII 코드 표라는 것이 있다.
	* ASCII 코드표는 각각의 문자마다 값을 부여해놓은 표이다.
	* 대문자 A == 65 이런식으로 각 문자마다 값이 부여되어 있는 것이다.
	* 실제 char 변수에 문자를 저장하면 문자에 대응하는 값이 메모리에 저장되고
	* 출력시에 해당값이 무엇이냐에 따라 ASCII 코드표의 문자가 출력되는 방식이다.
	* 
	* 모든 변수 타입들은 2가지 표현방법이 있다.
	* Signed: 부호를 포함하는 표현 방법이다. +값, -값을 의미한다.
	* Unsigned: 부호가 없는 표현 방법이다. +값만을 표현하게 한다.
	* 
	* 0 0 0 0 0 0 0 0
	* 
	* 가장 왼쪽 비트는 부호비트.
	* -128 ~ 127
	* 
	* 변수타입	|	메모리크기	|	저장종류		|	  값의 범위		|	unsigned
	* bool			1byte		   true/false		 true/false
	* char			1byte			 문자			 -128 ~ 127         0 ~ 255
	* short         2byte            정수          -2^15 ~ 2^15 -1	    0 ~ 2^16
	* int			4byte            정수          -2^31 ~ 2^31 - 1		0 ~ 2^32
	* __int64       8byte            정수          -2^63 ~ 2^63 - 1		0 ~ 2^64
	* float			4byte		 실수(부동소수점)
	* double        8byte        실수(부동소수점)
	* 
	* 
	* 3.1415926f float 타입 실수
	* 3.1415926 double 타입 실수
	*/

	short number = 32768;	

	float numberFloat = 3.1415926535f;

	std::cout << numberFloat << "\n";

	std::cout << number << "\n";

	return 0;
}