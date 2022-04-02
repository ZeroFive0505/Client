#include "Map.h"

CMap::CMap() : m_Type(Map_Type::MT_Normal)
{
}

CMap::~CMap()
{
}

bool CMap::Init(Map_Type Type)
{
	m_Type = Type;

	return true;
}

void CMap::Run()
{
}
