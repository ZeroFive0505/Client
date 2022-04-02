#include <iostream>

/*
* operator : 모든 연산자를 내가 원하는 대로 재정의를 해서 사용할 수 있는 기능이다.
* 
* 구조체 혹은 나중에 배울 클래스에서 operator를 이용해서 원하는 연산자를 원하는 기능을 만들어 줄 수가 있다.
* 형태
* 반환 타입 operator 재정의할 연산자(인자)
*/

struct Point
{
	int x;
	int y;

	Point operator+(const Point& rhs)
	{
		Point ret;

		// 그냥 x를 쓰면 이 구조체 변수를 사용하는 것인데
		// pt1 + pt2를 했다면 x는 pt1의 x값이 된다.
		// x + pt.x 를 하게 될 경우 pt1의 x값 + 인자로 pt2가 들어왔으므로
		// pt2의 x의 값을 더한것이 되고, pt1.x + pt2.x가 ret에 저장될 것이다.

		ret.x = this->x + rhs.x;
		ret.y = this->y + rhs.y;

		return ret;
	}

	Point operator+(int Value)
	{
		Point ret;

		ret.x = x + Value;
		ret.y = y + Value;

		return ret;
	}
	
	Point operator-(int Value)
	{
		Point ret;

		ret.x = x - Value;
		ret.y = y - Value;

		return ret;
	}

	// ++을 앞에 붙일 경우
	void operator++()
	{
		++x;
		++y;
	}

	// ++을 뒤에 붙일때는 아래와 같이 설정된 operator가 필요하다.
	void operator++(int)
	{
		x++;
		y++;
	}

	void operator+=(const Point& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& pt)
	{
		return os << pt.x << ", " << pt.y << "\n";
	}

	void operator<< (const char* pText)
	{
		std::cout << pText << "\n";
	}
};

int main()
{
	Point pt1, pt2, pt3;

	pt1.x = 10;
	pt1.y = 20;

	pt2.x = 20;
	pt2.y = 30;

	// 아래에서 + 연산을 해주고 있는데 이 경우 pt1의 + operator를 호출한다는 의미이다.
	// pt1의 + operator를 호출할때 뒤에 있는 pt2를 operator에 인자로 넣어주는 것이다.
	std::cout << pt1;
	std::cout << pt2;
	pt3 = pt1 + pt2;

	std::cout << pt3;

	pt3 = pt1 + 5;

	std::cout << pt3;

	pt3 += pt1;

	std::cout << pt3;

	++pt3;

	std::cout << pt3;

	pt3++;

	std::cout << pt3;;

	pt3 << "Hello Operator\n";

	std::cout << "Point 3 : " << pt3;

	return 0;
}