
#pragma once

#include <iostream>
#include <time.h>

#define LELVE_MAX 10
#define INVENTORY_MAX 10

enum Map_Type
{
    MT_Easy,
    MT_Normal,
    MT_Hard,
    MT_End
};

enum class Main_Menu
{
    None,
    Map,
    Store,
    Inventory,
    Exit
};

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

enum class Battle_Menu
{
    None, 
    Attack,
    Back
};

enum class Battle_Result
{
    None,
    Monster_Death,
    Player_Death
};

enum Store_Type
{
    ST_Weapon,
    ST_Armor,
    ST_End
};

enum class Store_Menu
{
    None,
    Weapon,
    Armor,
    Back
};

enum Item_Type
{
    IT_Weapon,
    IT_Armor,
    IT_End
};

enum Equip_Type
{
    Equip_Weapon,
    Equip_Armor,
    Equip_End
};