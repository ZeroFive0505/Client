#include "RampVertical.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CRampVertical::CRampVertical(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, CRaceScene* scene)
    : CFloorObject(topLeftPixel, Vector2(8.0f, 8.0f), EFloorObjectType::RAMP_VERTICAL, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, orientation, scene)
{
    m_Type = EFloorObjectType::RAMP_VERTICAL;
}

void CRampVertical::ApplyChanges() const
{
    CFloorObject::DefaultApplyChanges(this);
}

CTexture* CRampVertical::GetCurrentTexture() const
{
    return nullptr;
}

void CRampVertical::InteractWith(const CSharedPtr<CDriver>& driver, float currentTime)
{
}


ETileProperty CRampVertical::GetCurrentTile() const
{
    return ETileProperty::RAMP_VERTICAL;
}
