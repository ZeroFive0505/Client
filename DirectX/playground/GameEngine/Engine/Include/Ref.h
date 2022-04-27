#pragma once

#include "GameInfo.h"

/// <summary>
/// 레퍼런스 카운팅을 위한 클래스
/// CSharedPtr을 사용하기 위해서는 이 클래스를 상속 받아야한다.
/// </summary>
class CRef
{
public:
	CRef();
	virtual ~CRef();

protected:
	std::string m_Name;
	int m_RefCount;
	bool m_Enable;
	bool m_Active;
	size_t m_TypeID;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void Enable(bool bEnable)
	{
		m_Enable = bEnable;
	}

	virtual void Destroy()
	{
		m_Active = false;
	}

	bool IsActive() const
	{
		return m_Active;
	}

	bool IsEnable() const
	{
		return m_Enable;
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
			delete	this;
			return 0;
		}

		return m_RefCount;
	}

	void SetName(const std::string& name)
	{
		m_Name = name;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	inline size_t GetTypeID() const
	{
		return m_TypeID;
	}

	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	template <typename T>
	bool TypeCheck()
	{
		return m_TypeID == typeid(T).hash_code();
	}
};

