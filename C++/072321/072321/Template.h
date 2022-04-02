#pragma once

// Ŭ������ ���ø� Ŭ������ ������ְ� �Ǹ� .h�� �����κб��� ��� �Բ� ������־�� �Ѵ�.
// ��Ÿ�� ���ø� ������ ��� ���� Ÿ���� �ƴ϶� ��� ���� �����Ѵ�.
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
	// Ŭ������ ��� �Լ����� ���� template�� ������ ���� �ִ�.
	template <typename T1>
	void TestTemplateFunction()
	{

		std::cout <<"Function Template Type : " << typeid(T1).hash_code() << "\n";
	}
};

