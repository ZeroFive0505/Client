#include "RampHorizontal.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CRampHorizontal::CRampHorizontal(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, CRaceScene* scene)
    : CFloorObject(topLeftPixel, Vector2(8.0f, 8.0f), EFloorObjectType::RAMP_HORIZONTAL, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, orientation, scene)
{
    m_Type = EFloorObjectType::RAMP_HORIZONTAL;
}


void CRampHorizontal::ApplyChanges() const
{
    CFloorObject::DefaultApplyChanges(this);
}

CTexture* CRampHorizontal::GetCurrentTexture() const
{
    return nullptr;
}

void CRampHorizontal::InteractWith(const CSharedPtr<CDriver>& driver, float currentTime)
{
}

ETileProperty CRampHorizontal::GetCurrentTile() const
{
    return ETileProperty::RAMP_HORIZONTAL;
}
