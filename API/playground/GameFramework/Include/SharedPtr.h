#pragma once
/// <summary>
/// ���۷��� ī��Ʈ�� �̿��� ����Ʈ ������ ���ø� Ŭ����
/// ���۷��� ī���͸� �̿��ϱ⿡ �� �����͸� �̿��ϴ� Ŭ������ RefŬ������ ��ӹ޾ƾ� �Ѵ�.
/// </summary>
/// <typeparam name="T">Ŭ���� Ÿ��</typeparam>
template <typename T>
class CSharedPtr
{
public:
	CSharedPtr() :
		m_Ptr(nullptr)
	{

	}

	// ������ Ÿ���� ���ڷ� �޴� ���
	CSharedPtr(T* Ptr)
	{
		m_Ptr = Ptr;

		// ī��Ʈ�� �÷��ش�.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	// ���� ������
	CSharedPtr(const CSharedPtr<T>& Ptr)
	{
		m_Ptr = Ptr.m_Ptr;

		// ���������� ī��Ʈ�� �÷��ش�.
		if (m_Ptr)
			m_Ptr->AddRef();
	}

	~CSharedPtr()
	{
		// �Ҹ�ÿ��� ī��Ʈ�� ���ҽ����ش�.
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

