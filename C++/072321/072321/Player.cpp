#include "Player.h"
// cpp ���Ͽ��� include�� �����ν� ��ȯ ������ �����Ѵ�.
#include "Monster.h"

CPlayer::CPlayer()
{
	std::cout << "Player Constructor\n";
}

CPlayer::~CPlayer()
{
	std::cout << "Player Destructor\n";
}

void CPlayer::Output()
{
	std::cout << "Player Output\n";
}