#pragma once

#include "../Map/MapInfo.h"
#include "../GameInfo.h"
#include "WallObject.h"

/// <summary>
/// �� ������Ʈ ������
/// �浹�� �⺻ �� ������Ʈ �浹�� ������� ����Ѵ�.
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

