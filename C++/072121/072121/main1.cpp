#include <iostream>

/*
* operator : ��� �����ڸ� ���� ���ϴ� ��� �����Ǹ� �ؼ� ����� �� �ִ� ����̴�.
* 
* ����ü Ȥ�� ���߿� ��� Ŭ�������� operator�� �̿��ؼ� ���ϴ� �����ڸ� ���ϴ� ����� ����� �� ���� �ִ�.
* ����
* ��ȯ Ÿ�� operator �������� ������(����)
*/

struct Point
{
	int x;
	int y;

	Point operator+(const Point& rhs)
	{
		Point ret;

		// �׳� x�� ���� �� ����ü ������ ����ϴ� ���ε�
		// pt1 + pt2�� �ߴٸ� x�� pt1�� x���� �ȴ�.
		// x + pt.x �� �ϰ� �� ��� pt1�� x�� + ���ڷ� pt2�� �������Ƿ�
		// pt2�� x�� ���� ���Ѱ��� �ǰ�, pt1.x + pt2.x�� ret�� ����� ���̴�.

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

	// ++�� �տ� ���� ���
	void operator++()
	{
		++x;
		++y;
	}

	// ++�� �ڿ� ���϶��� �Ʒ��� ���� ������ operator�� �ʿ��ϴ�.
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

	// �Ʒ����� + ������ ���ְ� �ִµ� �� ��� pt1�� + operator�� ȣ���Ѵٴ� �ǹ��̴�.
	// pt1�� + operator�� ȣ���Ҷ� �ڿ� �ִ� pt2�� operator�� ���ڷ� �־��ִ� ���̴�.
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