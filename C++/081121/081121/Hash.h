#pragma once

#include <string>

class CHash
{
public:
	CHash()
	{

	}

	~CHash()
	{

	}

private:
	unsigned __int64 m_HashKey;

public:
	template <typename Key>
	unsigned __int64 GetHash(Key key)
	{
		m_HashKey = 0;

		size_t Length = sizeof(key);

		unsigned __int64 NewKey = (unsigned __int64)key;

		for (size_t i = 0; i < Length; i++)
		{
			unsigned char data = NewKey & 0xff;

			m_HashKey += data;

			NewKey >>= 8;
		}

		return m_HashKey;
	}

	// 탬플릿 특수화: 탬플릿의 인자 타입에 따라 별도의 처리 함수를 만들어놓고
	// 작업할 수 있게 해준다.
	template <>
	unsigned __int64 GetHash(std::string key)
	{
		m_HashKey = 0;

		size_t Length = key.length();

		for (size_t i = 0; i < Length; i++)
		{
			if (i % 2 == 0)
				m_HashKey += (unsigned __int64)key[i];
			else
				m_HashKey *= (unsigned __int64)key[i];
		}

		return m_HashKey;
	}

	template <>
	unsigned __int64 GetHash(const char* key)
	{
		m_HashKey = 0;

		size_t Length = strlen(key);

		for (size_t i = 0; i < Length; i++)
		{
			if (i % 2 == 0)
				m_HashKey += (unsigned __int64)key[i];
			else
				m_HashKey *= (unsigned __int64)key[i];
		}

		return m_HashKey;
	}
};