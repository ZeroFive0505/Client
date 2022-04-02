#pragma once

#include "GameInfo.h"

/// <summary>
/// ���۷��� ī������ ���ظ��� Ŭ����.
/// </summary>
class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	std::string m_Name;
	bool m_Active;
	int m_RefCount;

public:
	inline void Destroy()
	{
		m_Active = false;
	}

	inline void AddRef()
	{
		m_RefCount++;
	}

	inline int GetRefCount() const
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
	inline bool IsActive() const
	{
		return m_Active;
	}

	inline std::string GetName() const
	{
		return m_Name;
	}

	inline void SetName(const std::string& name)
	{
		m_Name = name;
	}
};

