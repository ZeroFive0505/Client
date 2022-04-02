#include "Coin.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CCoin::CCoin(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, CRaceScene* scene) :
	CFloorObject(topLeftPixel, Vector2(8.0f, 8.0f), EFloorObjectType::COIN, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, orientation, scene)
{
	m_Type = EFloorObjectType::COIN;

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			scene->GetSceneResoruce()->LoadTexture("CoinUp", TEXT("FloorObjects/CoinUp.bmp"));
			name = "CoinUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			scene->GetSceneResoruce()->LoadTexture("CoinRight", TEXT("FloorObjects/CoinRight.bmp"));
			name = "CoinRight";
			break;
		case EFloorObjectOrientation::DOWN:
			scene->GetSceneResoruce()->LoadTexture("CoinDown", TEXT("FloorObjects/CoinDown.bmp"));
			name = "CoinDown";
			break;
		case EFloorObjectOrientation::LEFT:
			scene->GetSceneResoruce()->LoadTexture("CoinLeft", TEXT("FloorObjects/CoinLeft.bmp"));
			name = "CoinLeft";
			break;
		}

		m_ActiveCoins[i] = scene->GetSceneResoruce()->FindTexture(name);
		m_ActiveCoins[i]->SetColorKey(255, 0, 255);
	}

	scene->GetSceneResoruce()->LoadTexture("InactiveCoin", TEXT("FloorObjects/InactiveCoin.bmp"));

	m_InactiveCoin = scene->GetSceneResoruce()->FindTexture("InactiveCoin");

	m_InactiveCoin->SetColorKey(255, 0, 255);
}

void CCoin::InteractWith(const CSharedPtr<CDriver>& driver, float currentTime)
{
	if (GetState() == EFloorObjectState::ACTIVE)
	{
		SetState(EFloorObjectState::INACTIVE);
		driver->AddCoin();
	}
}

void CCoin::ApplyChanges() const
{
	CFloorObject::DefaultApplyChanges(this);
}

CTexture* CCoin::GetCurrentTexture() const
{
	if (GetState() == EFloorObjectState::ACTIVE)
	{
		CTexture* cloneTexture = m_ActiveCoins[(int)m_Orientation]->Clone();
		cloneTexture->SetColorKey(255, 0, 255);

		return cloneTexture;
	}
	else
	{
		CTexture* cloneTexture = m_InactiveCoin->Clone();
		cloneTexture->SetColorKey(255, 0, 255);

		return cloneTexture;
	}
}

ETileProperty CCoin::GetCurrentTile() const
{
	return GetState() == EFloorObjectState::ACTIVE ? ETileProperty::OTHER :
		ETileProperty::TRACK;
}
