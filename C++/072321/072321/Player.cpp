#include "Player.h"
// cpp 파일에서 include를 함으로써 순환 참조를 방지한다.
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