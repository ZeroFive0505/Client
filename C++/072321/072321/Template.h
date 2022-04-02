#pragma once

// 클래스를 탬플릿 클래스로 만들어주게 되면 .h에 구현부분까지 모두 함께 만들어주어야 한다.
// 비타입 탬플릿 인자의 경우 변수 타입이 아니라 상수 값을 전달한다.
template <typename T, int Size = 999>
class CTemplate
{
public:
	CTemplate()
	{
		std::cout << typeid(T).name() << "\n";
		std::cout << "Size : " << Size << "\n";
	}

	~CTemplate()
	{

	}

private:
	T m_T1;
	T m_T2;

public:
	// 클래스의 멤버 함수마다 따로 template을 지정할 수도 있다.
	template <typename T1>
	void TestTemplateFunction()
	{

		std::cout <<"Function Template Type : " << typeid(T1).hash_code() << "\n";
	}
};

