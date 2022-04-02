#pragma once

template <typename T>
class CSharedPtr
{
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

	CSharedPtr(const CSharedPtr<T>& rhs)
	{
		m_Ptr = rhs.m_Ptr;

		if (m_Ptr)
			m_Ptr->AddRef();
	}

	~CSharedPtr()
	{
		if (m_Ptr)
			m_Ptr->Release();
	}

private:
	T* m_Ptr;

public:
	void operator = (T* ptr)
	{
		// 원래 가지고 있는 포인터는 풀어준다.
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = ptr;

		// 새롭게 대입된 포인터는 레퍼런스 카운터를 늘려준다.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	void operator = (const CSharedPtr<T>& rhs)
	{
		// 원래 가지고 있는 포인터는 풀어준다.
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = rhs.m_Ptr;

		// 새롭게 대입된 포인터는 레퍼런스 카운터를 늘려준다.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	inline bool operator == (T* ptr) const
	{
		return m_Ptr == ptr;
	}

	inline bool operator == (const CSharedPtr<T>& rhs) const
	{
		return m_Ptr == rhs.m_Ptr;
	}

	inline bool operator != (T* ptr) const
	{
		return m_Ptr != ptr;
	}

	inline bool operator != (const CSharedPtr<T>& rhs) const
	{
		return m_Ptr != rhs.m_Ptr;
	}

	inline operator T* () const
	{
		return m_Ptr;
	}

	inline T* operator->() const
	{
		return m_Ptr;
	}
};