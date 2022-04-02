#include <iostream>

class CStatic
{
public:
	CStatic()
	{

	}

	~CStatic()
	{

	}

public:
	int m_iNumber;
	static int m_iNumberStatic;

public:
	void Output()
	{
		std::cout << "Output\n";
	}

	// static ����Լ������� this�� ����� �� ����.
	// �׷��� ������ �Ϲ� ��������� ����� �� ����.
	// ���⿡���� static ��� ������ ����� �����ϴ�.
	// �Լ������� ���°� �����Լ��� ������ ���·� ����� �����ϴ�.
	static void OutputStatic()
	{
		std::cout << "Output Static\n";
		// std::cout << m_Number << "\n";
		// std::cout << this << "\n";
	}
};

// Ŭ������ static ��� ������ �ݵ�� Ŭ���� �ܺο� ���� �κ��� ������־�� �Ѵ�.
// �⺻���� 0���� �ʱ�ȭ �ȴ�. ���ϴ� �� ���Ե� ����.
// Ŭ������ static ��������� �� Ŭ������ �̿��ؼ� ��ü�� �ƹ��� ���� �����ϴ���
// �� ������ �޸𸮴� �� 1���� ������ �ȴ�.
// �Ϲ� ���������� ��ü�� ����ŭ �޸� ������ �Ҵ�� ���̴�.
int CStatic::m_iNumberStatic;

class CSingleton
{
	
private:
	// �����ڿ� �Ҹ��ڰ� private�̶�� �� Ŭ������ �ܺο��� ��ü�� �����ϴ� ���� �Ұ����ϴ�.
	// �Ұ����ϰ� �� Ŭ������ �ܺο��� ��ü�� �޸𸮿��� �����ϴ� ���� �Ұ��� �ϴ�.
	CSingleton()
	{

	}

	~CSingleton()
	{

	}
private:
	static CSingleton* m_pInst;

public: 
	static CSingleton* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CSingleton;

		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst != nullptr)
		{
			delete m_pInst;
			m_pInst = nullptr;
		}
	}
};

CSingleton* CSingleton::m_pInst = nullptr;

int main()
{
	CStatic std1;

	std1.m_iNumberStatic = 100;
	CStatic::m_iNumberStatic = 300;

	std::cout << std1.m_iNumberStatic << "\n";

	void(*pFunc)() = CStatic::OutputStatic;

	// �Ϲ� ��� �Լ��� �����ʹ� ������� �ٸ���.
	// pFunc = CStatic::Output;


	void(CStatic:: * pFunc1)() = &CStatic::Output;

	pFunc();

	// �Ϲ� ����Լ��� �ݵ�� this�� ������ �Ǿ�� �Ѵ�.
	// �׷��� ������ � ��ü�� this�� �������� �ݵ�� ����Ǿ�� �Ѵ�.
	(std1.*pFunc1)();

	// CSingleton singleton;

	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";
	std::cout << CSingleton::GetInst() << "\n";

	CSingleton::DestroyInst();

	return 0;
}