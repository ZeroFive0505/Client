#include "Ref.h"

CRef::CRef() :
	m_RefCount(0),
	m_Enable(true),
	m_Active(true),
	m_TypeID(0)
{
}

CRef::~CRef()
{
}

void CRef::Save(FILE* pFile)
{
	// 레퍼런스 카운트는 자동으로 계산되기에 저장하지 않는다.

	int length = (int)m_Name.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), length, pFile);

	fwrite(&m_Enable, sizeof(bool), 1, pFile);
	fwrite(&m_Active, sizeof(bool), 1, pFile);
	fwrite(&m_TypeID, sizeof(size_t), 1, pFile);
}

void CRef::Load(FILE* pFile)
{
	int length = 0;
	fread(&length, sizeof(int), 1, pFile);
	char name[256] = {};

	fread(name, sizeof(char), length, pFile);

	m_Name = name;

	fread(&m_Enable, sizeof(bool), 1, pFile);
	fread(&m_Active, sizeof(bool), 1, pFile);
	fread(&m_TypeID, sizeof(size_t), 1, pFile);
}
