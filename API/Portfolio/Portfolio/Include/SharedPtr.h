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
		// ���� ������ �ִ� �����ʹ� Ǯ���ش�.
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = ptr;

		// ���Ӱ� ���Ե� �����ʹ� ���۷��� ī���͸� �÷��ش�.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	void operator = (const CSharedPtr<T>& rhs)
	{
		// ���� ������ �ִ� �����ʹ� Ǯ���ش�.
		if (m_Ptr)
			m_Ptr->Release();

		m_Ptr = rhs.m_Ptr;

		// ���Ӱ� ���Ե� �����ʹ� ���۷��� ī���͸� �÷��ش�.
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