#pragma once

#include "Driver.h"
#include "ObjectInfo.h"
#include "../Map/MapInfo.h"
#include "WallObject.h"
#include "../Resource/Texture.h"

const float THUNDER_INIT_DURATION = 5.0f;
const float THUNDER_INCREMENT_DURATION = 1.5f;

const int UPDATES_PER_SECOND = 15;

class CItem :
	public CWallObject
{
protected:
	CSharedPtr<CTexture> m_Texture;

	bool m_Used;

public:
	static class CRaceScene* m_StaticScene;

	CItem(const Vector2& pos, const float visualRadius, const float hitboxRadius, const float height, class CRaceScene* scene, const std::string& name);

	static void UseItem(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking,
		const bool isFront, class CRaceScene* scene, class CRaceGUI* gui, const bool forceToUse = false);

	static std::pair<float, bool> GetItemUseProb(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking);

	virtual void Update(float deltaTime) = 0;

	virtual CTexture* GetTexture()
	{
		if (m_Texture)
		{
			CTexture* cloneTexture = m_Texture->Clone();
			cloneTexture->SetColorKey(255, 0, 255);
			cloneTexture->SetPivot((float)cloneTexture->GetWidth() / 2.0f, (float)cloneTexture->GetHeight());

			return cloneTexture;
		}
		else
			return nullptr;
	}

	virtual bool RegisterCollisions() const
	{
		return false;
	}

	inline bool GetUsed() const
	{
		return m_Used;
	}
};

