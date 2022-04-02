#pragma once

#include "../GameInfo.h"
#include "FloorObject.h"
#include "Driver.h"

class CCoin :
	public CFloorObject
{
private:
	CSharedPtr<CTexture> m_ActiveCoins[(int)EFloorObjectOrientation::END];
	CSharedPtr<CTexture> m_InactiveCoin;

	virtual EFloorObjectState GetInitState() const override
	{
		return EFloorObjectState::ACTIVE;
	}

public:
	CCoin(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, class CRaceScene* scene);

	void InteractWith(const CSharedPtr<CDriver>& driver, float currentTime) override;

	virtual void ApplyChanges() const override;

	CTexture* GetCurrentTexture() const override;

	virtual ETileProperty GetCurrentTile() const override;
};

