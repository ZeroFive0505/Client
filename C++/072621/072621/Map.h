#pragma once

#include "Game.h"

// �� Ŭ���� �� �Ŵ����� ���ؼ� ����
class CMap
{
public:
	CMap();
	~CMap();

private:
	Map_Type m_Type;

public:
	bool Init(Map_Type Type);
	void Run();
};

