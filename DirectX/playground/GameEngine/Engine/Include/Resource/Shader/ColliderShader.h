#pragma once

#include "GraphicShader.h"

/// <summary>
/// 충돌체 마테리얼을 출력할때 이용하는 쉐이더
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

