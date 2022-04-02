#pragma once

#include "GameInfo.h"
/// <summary>
/// Ref는 레퍼런스 카운팅을 이용한 스마트 포인터이다. 레퍼런스 카운트를 계산해서 자동으로 0이 됬을시 즉, 아무도 참조하고 있지
/// 않을시에 그 포인터를 지운다.
/// </summary>
class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	std::string	m_Name;
	bool	m_Active;
	int m_RefCount;

public:
	void Destroy()
	{
		m_Active = false;
	}

	void AddRef()
	{
		m_RefCount++;
	}

	int GetRefCount() const
	{
		return m_RefCount;
	}

	int Release()
	{
		m_RefCount--;

		if (m_RefCount <= 0)
		{
			delete this;
			return 0;
		}

		return m_RefCount;
	}

public:
	bool IsActive()	const
	{
		return m_Active;
	}

	std::string GetName()	const
	{
		return m_Name;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
};

