#pragma once

/// <summary>
/// CRef클래스를 이용하여 레퍼런스 카운트를 한다.
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class CSharedPtr
{
private:
	T* m_Ptr;

public:
	CSharedPtr() :
		m_Ptr(nullptr)
	{

	}

	CSharedPtr(T* ptr)
	{
		m_Ptr = ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	CSharedPtr(const CSharedPtr<T>& ptr)
	{
		m_Ptr = ptr.m_Ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	~CSharedPtr()
	{
		if (m_Ptr)
			m_Ptr->Release();
	}

public:
	void operator = (T* ptr)
	{
		if (ptr)
			ptr->AddRef();

		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = ptr;

	}

	void operator = (const CSharedPtr<T>& ptr)
	{
		if (ptr.m_Ptr)
			ptr.m_Ptr->AddRef();

		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = ptr.m_Ptr;

	}

	bool operator == (T* ptr) const
	{
		return m_Ptr == ptr;
	}

	bool operator == (const CSharedPtr<T>& ptr) const
	{
		return m_Ptr == ptr.m_Ptr;
	}

	bool operator != (T* ptr) const
	{
		return m_Ptr != ptr;
	}

	bool operator != (const CSharedPtr<T>& ptr) const
	{
		return m_Ptr != ptr.m_Ptr;
	}

	operator T* () const
	{
		return m_Ptr;
	}

	T* operator -> () const
	{
		return m_Ptr;
	}

	T* operator * () const
	{
		return m_Ptr;
	}

	T* Get()
	{
		return m_Ptr;
	}
};

