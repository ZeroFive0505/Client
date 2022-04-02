#pragma once

#include "../Map/MapInfo.h"
#include "../GameInfo.h"
#include "WallObject.h"

/// <summary>
/// 벽 오브젝트 파이프
/// 충돌은 기본 벽 오브젝트 충돌을 기반으로 사용한다.
/// </summary>
class CPipe :
	public CWallObject
{
private:
	CSharedPtr<CTexture> m_Texture;
	EWallObjectType m_Type;

public:
	CPipe(const Vector2& pos, const EWallObjectType type, class CRaceScene* scene, const std::string& name);

	CTexture* GetTexture();
};

