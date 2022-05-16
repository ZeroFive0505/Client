#pragma once

#include "GraphicShader.h"

class CColliderShader3D :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CColliderShader3D();
	virtual ~CColliderShader3D();

public:
	virtual bool Init();
};

