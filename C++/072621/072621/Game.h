#pragma once

// Game.h�� ���ӿ� ���� ���� ������ϰ� enum���� �ۼ��Ѵ�.

#include <iostream>
#include <time.h>

// End�� �ִ� ������ �ݺ����� ���ϰ� �ϱ� ����.
enum Map_Type
{
	MT_Easy,
	MT_Normal,
	MT_Hard,
	MT_End
};

// ���� �޴� ����
enum class Main_Menu
{
	None,
	Map,
	Store,
	Inventory,
	Exit
};

// ���� ����
enum class Map_Menu
{
	None,
	Easy,
	Normal,
	Hard,
	Back
};

enum class Job
{
	None,
	Knight,
	Archer,
	Magicion,
	End
};