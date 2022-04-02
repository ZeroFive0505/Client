#pragma once

#include "GameInfo.h"

// CMonster 클래스를 전방 선언하여 해당 CMonster라는 클래스가 어딘가에 있다라고 가정하고 사용할 수 있게한다.
// 전방선언시 이 타입을 실제로 생성하는것은 불가능하고 이 타입에 대한 메모리 주소만을 사용할 수 있다.
// 즉, 포인터로만 사용이 가능하다.
class CMonster;

class CPlayer
{
public:
	CPlayer();
	~CPlayer();
	void Output();

private:
	// 아래처럼 실제 객체를 만들기 위해서는 반드시 이 클래스가 있는 헤더파일을 include하고
	// 객체를 만들어서 사용해야 한다.
	// CMonster m_Monster;
	CMonster* m_pMonster;
};

