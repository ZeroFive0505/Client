#include "OilSlick.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

COilSlick::COilSlick(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, CRaceScene* scene) :
    CFloorObject(topLeftPixel, Vector2(16.0f, 16.0f), EFloorObjectType::OIL_SLICK, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, orientation, scene)
{
    m_Type = EFloorObjectType::OIL_SLICK;

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			scene->GetSceneResoruce()->LoadTexture("OilUp", TEXT("FloorObjects/OilUp.bmp"));
			name = "OilUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			scene->GetSceneResoruce()->LoadTexture("OilRight", TEXT("FloorObjects/OilRight.bmp"));
			name = "OilRight";
			break;
		case EFloorObjectOrientation::DOWN:
			scene->GetSceneResoruce()->LoadTexture("OilDown", TEXT("FloorObjects/OilDown.bmp"));
			name = "OilDown";
			break;
		case EFloorObjectOrientation::LEFT:
			scene->GetSceneResoruce()->LoadTexture("OilLeft", TEXT("FloorObjects/OilLeft.bmp"));
			name = "OilLeft";
			break;
		}

		m_OilTexture[i] = scene->GetSceneResoruce()->FindTexture(name);
		m_OilTexture[i]->SetColorKey(255, 0, 255);
	}
}

void COilSlick::ApplyChanges() const
{
}

void COilSlick::InteractWith(const CSharedPtr<CDriver>& driver, float currentTime)
{
}

CTexture* COilSlick::GetCurrentTexture() const
{
	CTexture* cloneTexture = m_OilTexture[(int)m_Orientation]->Clone();

	cloneTexture->SetColorKey(255, 0, 255);

	return cloneTexture;
}

ETileProperty COilSlick::GetCurrentTile() const
{
	return ETileProperty::OIL_SLICK;
}
