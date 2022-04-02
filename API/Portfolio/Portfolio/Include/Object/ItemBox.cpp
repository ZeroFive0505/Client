#include "ItemBox.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

EItem CItemBox::ItemArray_1_CPU[CItemBox::NUM_ITEMS_ARRAY] =
{
	EItem::BANANA,      EItem::BANANA,      EItem::BANANA,
	EItem::BANANA,      EItem::BANANA,      EItem::BANANA,
	EItem::COIN,        EItem::COIN,        EItem::COIN,
	EItem::COIN,        EItem::COIN,        EItem::COIN,
	EItem::GREEN_SHELL, EItem::GREEN_SHELL, EItem::RED_SHELL,
	EItem::MUSHROOM,
};

EItem CItemBox::ItemArray_23_CPU[CItemBox::NUM_ITEMS_ARRAY] =
{
	EItem::BANANA,      EItem::RED_SHELL,   EItem::RED_SHELL,
	EItem::RED_SHELL,   EItem::GREEN_SHELL, EItem::RED_SHELL,
	EItem::BANANA,      EItem::BANANA,      EItem::BANANA,
	EItem::MUSHROOM,    EItem::MUSHROOM,    EItem::GREEN_SHELL,
	EItem::COIN,        EItem::COIN,        EItem::COIN,
	EItem::GREEN_SHELL,
};

EItem CItemBox::ItemArray_45_CPU[CItemBox::NUM_ITEMS_ARRAY] =
{
	EItem::RED_SHELL, EItem::RED_SHELL,   EItem::RED_SHELL,
	EItem::STAR,      EItem::GREEN_SHELL, EItem::COIN,
	EItem::BANANA,    EItem::BANANA,      EItem::BANANA,
	EItem::MUSHROOM,  EItem::MUSHROOM,    EItem::MUSHROOM,
	EItem::COIN,      EItem::GREEN_SHELL, EItem::GREEN_SHELL,
	EItem::COIN,
};

EItem CItemBox::ItemArray_67_CPU[CItemBox::NUM_ITEMS_ARRAY] =
{
	EItem::MUSHROOM,  EItem::MUSHROOM,    EItem::MUSHROOM,
	EItem::MUSHROOM,  EItem::COIN,        EItem::BANANA,
	EItem::RED_SHELL, EItem::RED_SHELL,   EItem::RED_SHELL,
	EItem::STAR,      EItem::GREEN_SHELL, EItem::STAR,
	EItem::THUNDER,   EItem::GREEN_SHELL, EItem::BANANA,
	EItem::COIN,
};

EItem CItemBox::ItemArray_8_CPU[CItemBox::NUM_ITEMS_ARRAY] =
{
	EItem::MUSHROOM,  EItem::MUSHROOM,  EItem::MUSHROOM,
	EItem::MUSHROOM,  EItem::MUSHROOM,  EItem::RED_SHELL,
	EItem::RED_SHELL, EItem::RED_SHELL, EItem::RED_SHELL,
	EItem::STAR,      EItem::STAR,      EItem::STAR,
	EItem::THUNDER,   EItem::THUNDER,   EItem::GREEN_SHELL,
	EItem::COIN,
};

EItem CItemBox::ItemArray_1_PLAYER[CItemBox::NUM_ITEMS_ARRAY] = {
	EItem::BANANA,      EItem::BANANA,      EItem::BANANA,
	EItem::BANANA,      EItem::BANANA,      EItem::BANANA,
	EItem::COIN,        EItem::COIN,        EItem::COIN,
	EItem::COIN,        EItem::COIN,        EItem::COIN,
	EItem::GREEN_SHELL, EItem::GREEN_SHELL, EItem::GREEN_SHELL,
	EItem::MUSHROOM,
};

EItem CItemBox::ItemArray_23_PLAYER[CItemBox::NUM_ITEMS_ARRAY] = {
	EItem::BANANA,      EItem::RED_SHELL,   EItem::RED_SHELL,
	EItem::RED_SHELL,   EItem::GREEN_SHELL, EItem::RED_SHELL,
	EItem::BANANA,      EItem::BANANA,      EItem::BANANA,
	EItem::MUSHROOM,    EItem::MUSHROOM,    EItem::GREEN_SHELL,
	EItem::COIN,        EItem::COIN,        EItem::COIN,
	EItem::GREEN_SHELL,
};

EItem CItemBox::ItemArray_45_PLAYER[CItemBox::NUM_ITEMS_ARRAY] = {
		EItem::RED_SHELL, EItem::RED_SHELL,   EItem::RED_SHELL,
	EItem::STAR,      EItem::GREEN_SHELL, EItem::COIN,
	EItem::BANANA,    EItem::BANANA,      EItem::BANANA,
	EItem::MUSHROOM,  EItem::MUSHROOM,    EItem::MUSHROOM,
	EItem::COIN,      EItem::GREEN_SHELL, EItem::GREEN_SHELL,
	EItem::COIN,
};

EItem CItemBox::ItemArray_67_PLAYER[CItemBox::NUM_ITEMS_ARRAY] = {
	EItem::MUSHROOM,  EItem::MUSHROOM,    EItem::MUSHROOM,
	EItem::MUSHROOM,  EItem::COIN,        EItem::FEATHER,
	EItem::RED_SHELL, EItem::RED_SHELL,   EItem::RED_SHELL,
	EItem::STAR,      EItem::GREEN_SHELL, EItem::STAR,
	EItem::THUNDER,   EItem::GREEN_SHELL, EItem::FEATHER,
	EItem::COIN,
};

EItem CItemBox::ItemArray_8_PLAYER[CItemBox::NUM_ITEMS_ARRAY] = {
	EItem::MUSHROOM,  EItem::MUSHROOM,  EItem::FEATHER,
	EItem::MUSHROOM,  EItem::MUSHROOM,  EItem::FEATHER,
	EItem::RED_SHELL, EItem::RED_SHELL, EItem::RED_SHELL,
	EItem::STAR,      EItem::STAR,      EItem::STAR,
	EItem::THUNDER,   EItem::THUNDER,   EItem::GREEN_SHELL,
	EItem::FEATHER,
};

CItemBox::CItemBox(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, CRaceScene* scene) :
	CFloorObject(topLeftPixel, Vector2(16.0f, 16.0f), EFloorObjectType::ITEM_BOX, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, orientation, scene)
{
	m_Type = EFloorObjectType::ITEM_BOX;

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			scene->GetSceneResoruce()->LoadTexture("ItemBoxUp", TEXT("FloorObjects/ItemBoxUp.bmp"));
			name = "ItemBoxUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			scene->GetSceneResoruce()->LoadTexture("ItemBoxRight", TEXT("FloorObjects/ItemBoxRight.bmp"));
			name = "ItemBoxRight";
			break;
		case EFloorObjectOrientation::DOWN:
			scene->GetSceneResoruce()->LoadTexture("ItemBoxDown", TEXT("FloorObjects/ItemBoxDown.bmp"));
			name = "ItemBoxDown";
			break;
		case EFloorObjectOrientation::LEFT:
			scene->GetSceneResoruce()->LoadTexture("ItemBoxLeft", TEXT("FloorObjects/ItemBoxLeft.bmp"));
			name = "ItemBoxLeft";
			break;
		}

		m_ItemBoxActive[i] = scene->GetSceneResoruce()->FindTexture(name);
		m_ItemBoxActive[i]->SetColorKey(255, 0, 255);
	}


	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			scene->GetSceneResoruce()->LoadTexture("EmptyBoxUp", TEXT("FloorObjects/EmptyBoxUp.bmp"));
			name = "EmptyBoxUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			scene->GetSceneResoruce()->LoadTexture("EmptyBoxRight", TEXT("FloorObjects/EmptyBoxRight.bmp"));
			name = "EmptyBoxRight";
			break;
		case EFloorObjectOrientation::DOWN:
			scene->GetSceneResoruce()->LoadTexture("EmptyBoxDown", TEXT("FloorObjects/EmptyBoxDown.bmp"));
			name = "EmptyBoxDown";
			break;
		case EFloorObjectOrientation::LEFT:
			scene->GetSceneResoruce()->LoadTexture("EmptyBoxLeft", TEXT("FloorObjects/EmptyBoxLeft.bmp"));
			name = "EmptyBoxLeft";
			break;
		}

		m_EmptyBox[i] = scene->GetSceneResoruce()->FindTexture(name);
		m_EmptyBox[i]->SetColorKey(255, 0, 255);
	}
	
}

void CItemBox::ApplyChanges() const
{
	CFloorObject::DefaultApplyChanges(this);
}

void CItemBox::Update(float deltaTime)
{
	if (m_State == EFloorObjectState::INACTIVE)
	{
		m_fElapsedTime += deltaTime;
		ReactiveItemBox();
	}
}

void CItemBox::InteractWith(const CSharedPtr<CDriver>& driver, float currentTime)
{
	if (GetState() == EFloorObjectState::ACTIVE && driver->GetItem() == EItem::NONE)
	{
		SetState(EFloorObjectState::INACTIVE);
		m_InactivatedAt = currentTime;
		m_fElapsedTime = currentTime;

		int ID = rand() % NUM_ITEMS_ARRAY;

		EItem item;

		switch (driver->m_Rank)
		{
		case 0:
			if (driver->m_ControlType == EDriverControlType::AI_GRADIENT)
				item = ItemArray_1_CPU[ID];
			else
				item = ItemArray_1_PLAYER[ID];
			break;
		case 1:
		case 2:
			if (driver->m_ControlType == EDriverControlType::AI_GRADIENT)
				item = ItemArray_23_CPU[ID];
			else
				item = ItemArray_23_PLAYER[ID];
			break;
		case 3:
		case 4:
			if (driver->m_ControlType == EDriverControlType::AI_GRADIENT)
				item = ItemArray_45_CPU[ID];
			else
				item = ItemArray_45_PLAYER[ID];
			break;
		case 5:
		case 6:
			if (driver->m_ControlType == EDriverControlType::AI_GRADIENT)
				item = ItemArray_67_CPU[ID];
			else
				item = ItemArray_67_PLAYER[ID];
		default:
			if (driver->m_ControlType == EDriverControlType::AI_GRADIENT)
				item = ItemArray_8_CPU[ID];
			else
				item = ItemArray_8_PLAYER[ID];
			break;
		}

		driver->PickUpItem(item);
	}
}

CTexture* CItemBox::GetCurrentTexture() const
{
	if (GetState() == EFloorObjectState::ACTIVE)
	{
		CTexture* cloneTexture = m_ItemBoxActive[(int)m_Orientation]->Clone();
		cloneTexture->SetColorKey(255, 0, 255);

		return cloneTexture;
	}
	else
	{
		CTexture* cloneTexture = m_EmptyBox[(int)m_Orientation]->Clone();
		cloneTexture->SetColorKey(255, 0, 255);

		return cloneTexture;
	}
}

void CItemBox::ReactiveItemBox()
{
	if (m_fElapsedTime - m_InactivatedAt >= REACTIVE_ITEM_BOX)
	{
		m_State = EFloorObjectState::ACTIVE;
	}
}

ETileProperty CItemBox::GetCurrentTile() const
{
	return GetState() == EFloorObjectState::ACTIVE ? ETileProperty::OTHER :
		ETileProperty::TRACK;
}
