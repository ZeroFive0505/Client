#pragma once

#include "../GameInfo.h"
#include "KartProperties.h"
#include <array>
#include <random>

enum class EMenuPlayer : int
{
	MARIO,
	PEARCH,
	BOWSER,
	KOOPA,
	LUIGI,
	YOSHI,
	DK,
	TOAD,
	END
};

const std::array<std::string, (int)EMenuPlayer::END> DRIVER_DISPLAY_NAMES = {
	"mario", "princess", "bowser", "koopa troopa",
	"luigi", "yoshi", "donkey kong hr", "toad"
};

const std::array<std::string, (int)EMenuPlayer::END> DRIVER_ASSET_NAMES = {
	"Mario", "Peach",
	"Bowser", "Koopa",
	"Luigi", "Yoshi",
	"DK", "Toad"
};

const std::array<const CKartProperties*, (int)EMenuPlayer::END> DRIVER_PROPERTIES = {
	&CKartProperties::BALACNED, &CKartProperties::ACCELERATION,
	&CKartProperties::HEAVY, &CKartProperties::HANDLING,
	&CKartProperties::BALACNED, &CKartProperties::ACCELERATION,
	&CKartProperties::HEAVY, &CKartProperties::HANDLING
};

enum class EDriverControlType
{
	DISABLED,
	AI_GRADIENT,
	PLAYER
};

enum class EItem
{
	NONE,
	MUSHROOM,
	COIN,
	STAR,
	BANANA,
	GREEN_SHELL,
	RED_SHELL,
	THUNDER,
	FEATHER,
	END
};

static std::random_device rd;
static std::mt19937 randGen(rd());

const int FAR_VISIONS[(int)ECCOption::END][(int)EMenuPlayer::END] = {
	{7, 7, 6, 6, 6, 6, 2, 1},
	{4, 4, 4, 4, 3, 2, 2, 1},
	{2, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

const int ITEM_PROB_MODS[(int)ECCOption::END][(int)EMenuPlayer::END] = {
	{7, 7, 6, 6, 6, 6, 2, 1},
	{4, 4, 4, 4, 3, 2, 2, 1},
	{2, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

const unsigned int IMPEDIMENTS[(int)ECCOption::END][(int)EMenuPlayer::END] = {
	{128, 128, 96, 96, 96, 64, 64, 48},
	{96, 64, 64, 48, 48, 48, 32, 32},
	{64, 64, 64, 48, 48, 32, 32, 16},
	{64, 64, 32, 32, 16, 16, 8, 8}
};