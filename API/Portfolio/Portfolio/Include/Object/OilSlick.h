#pragma once

#include "../Resource/Texture.h"
#include "Driver.h"
#include "../Map/TileMap.h"
#include "FloorObject.h"

class COilSlick :
	public CFloorObject
{
private:
	CSharedPtr<CTexture> m_OilTexture[(int)EFloorObjectOrientation::END];

	virtual EFloorObjectState GetInitState() const override
	{
		return EFloorObjectState::ACTIVE;
	}

public:
	COilSlick(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, class CRaceScene* scene);

	virtual void ApplyChanges() const override;

	void InteractWith(const CSharedPtr<CDriver>& driver, float currentTime) override;

	CTexture* GetCurrentTexture() const override;

	virtual ETileProperty GetCurrentTile() const override;
};

