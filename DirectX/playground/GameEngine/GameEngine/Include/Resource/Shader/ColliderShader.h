#pragma once

#include "GraphicShader.h"

/// <summary>
/// �浹ü ���׸����� ����Ҷ� �̿��ϴ� ���̴�
/// </summary>
class CColliderShader :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CColliderShader();
	virtual ~CColliderShader();

public:
	virtual bool Init();
};

