#pragma once

#include "GameInfo.h"

// CMonster Ŭ������ ���� �����Ͽ� �ش� CMonster��� Ŭ������ ��򰡿� �ִٶ�� �����ϰ� ����� �� �ְ��Ѵ�.
// ���漱��� �� Ÿ���� ������ �����ϴ°��� �Ұ����ϰ� �� Ÿ�Կ� ���� �޸� �ּҸ��� ����� �� �ִ�.
// ��, �����ͷθ� ����� �����ϴ�.
class CMonster;

class CPlayer
{
public:
	CPlayer();
	~CPlayer();
	void Output();

private:
	// �Ʒ�ó�� ���� ��ü�� ����� ���ؼ��� �ݵ�� �� Ŭ������ �ִ� ��������� include�ϰ�
	// ��ü�� ���� ����ؾ� �Ѵ�.
	// CMonster m_Monster;
	CMonster* m_pMonster;
};

