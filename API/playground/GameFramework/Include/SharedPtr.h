#pragma once
/// <summary>
/// 레퍼런스 카운트를 이용한 스마트 포인터 템플릿 클래스
/// 레퍼런스 카운터를 이용하기에 이 포인터를 이용하는 클래스는 Ref클래스를 상속받아야 한다.
/// </summary>
/// <typeparam name="T">클래스 타입</typeparam>
template <typename T>
class CSharedPtr
{
public:
	CSharedPtr() :
		m_Ptr(nullptr)
	{

	}

	// 포인터 타입을 인자로 받는 경우
	CSharedPtr(T* Ptr)
	{
		m_Ptr = Ptr;

		// 카운트를 늘려준다.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	// 복사 생성자
	CSharedPtr(const CSharedPtr<T>& Ptr)
	{
		m_Ptr = Ptr.m_Ptr;

		// 마찬가지로 카운트를 늘려준다.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	~CSharedPtr()
	{
		// 소멸시에는 카운트를 감소시켜준다.
		if (m_Ptr)
			m_Ptr->Release();
	}

private:
	T* m_Ptr;

public:
	void operator=(T* Ptr)
	{
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = Ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	void operator=(const CSharedPtr<T>& Ptr)
	{
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = Ptr.m_Ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	bool operator==(T* Ptr) const
	{
		return m_Ptr == Ptr;
	}

	bool operator==(const CSharedPtr<T>& Ptr) const
	{
		return m_Ptr == Ptr.m_Ptr;
	}

	bool operator!=(T* Ptr) const
	{
		return m_Ptr != Ptr;
	}

	bool operator!=(const CSharedPtr<T>& Ptr) const
	{
		return m_Ptr != Ptr.m_Ptr;
	}

	operator T* () const
	{
		return m_Ptr;
	}

	T* operator->() const
	{
		return m_Ptr;
	}
};

