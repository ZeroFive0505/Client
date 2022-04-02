#pragma once

// Game.h는 게임에 쓰일 공용 헤더파일과 enum문을 작성한다.

#include <iostream>
#include <time.h>

// End를 넣는 이유는 반복문을 편하게 하기 위함.
enum Map_Type
{
	MT_Easy,
	MT_Normal,
	MT_Hard,
	MT_End
};

// 메인 메뉴 선택
enum class Main_Menu
{
	None,
	Map,
	Store,
	Inventory,
	Exit
};

// 맵의 설정
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