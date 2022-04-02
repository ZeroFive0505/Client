#pragma once

#include "../GameInfo.h"
#include "Driver.h"
#include "FloorObject.h"

const float REACTIVE_ITEM_BOX = 10.0f;

class CItemBox :
	public CFloorObject
{
private:
	static const int NUM_ITEMS_ARRAY = 16;

	static EItem ItemArray_1_CPU[NUM_ITEMS_ARRAY];
	static EItem ItemArray_23_CPU[NUM_ITEMS_ARRAY];
	static EItem ItemArray_45_CPU[NUM_ITEMS_ARRAY];
	static EItem ItemArray_67_CPU[NUM_ITEMS_ARRAY];
	static EItem ItemArray_8_CPU[NUM_ITEMS_ARRAY];

	static EItem ItemArray_1_PLAYER[NUM_ITEMS_ARRAY];
	static EItem ItemArray_23_PLAYER[NUM_ITEMS_ARRAY];
	static EItem ItemArray_45_PLAYER[NUM_ITEMS_ARRAY];
	static EItem ItemArray_67_PLAYER[NUM_ITEMS_ARRAY];
	static EItem ItemArray_8_PLAYER[NUM_ITEMS_ARRAY];

	CSharedPtr<CTexture> m_ItemBoxActive[(int)EFloorObjectOrientation::END];
	CSharedPtr<CTexture> m_EmptyBox[(int)EFloorObjectOrientation::END];

	float m_InactivatedAt;
	float m_fElapsedTime;

	virtual EFloorObjectState GetInitState() const override
	{
		return EFloorObjectState::ACTIVE;
	}

public:
	CItemBox(const Vector2& topLeftPixel, const EFloorObjectOrientation orientation, class CRaceScene* scene);

	virtual void ApplyChanges() const override;

	virtual void Update(float deltaTime) override;

	void InteractWith(const CSharedPtr<CDriver>& driver, float currentTime) override;

	CTexture* GetCurrentTexture() const override;

	void ReactiveItemBox();

	virtual ETileProperty GetCurrentTile() const override;
};

