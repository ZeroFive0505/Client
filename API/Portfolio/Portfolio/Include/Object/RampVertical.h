#pragma once

#include "../Resource/Texture.h"
#include "Driver.h"
#include "FloorObject.h"
#include "../Map/TileMap.h"

class CRampVertical :
	public CFloorObject
{
private:
	virtual EFloorObjectState GetInitState() const override
	{
		return EFloorObjectState::INITIAL;
	}


public:
	CRampVertical(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, class CRaceScene* scene);

	virtual void ApplyChanges() const override;

	virtual CTexture* GetCurrentTexture() const;

	virtual void InteractWith(const CSharedPtr<CDriver>& driver, float currentTime);

	virtual ETileProperty GetCurrentTile() const override;
};

