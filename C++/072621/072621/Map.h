#pragma once

#include "Game.h"

// 맵 클래는 맵 매니저에 의해서 관리
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

