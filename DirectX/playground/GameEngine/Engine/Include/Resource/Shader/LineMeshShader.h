#pragma once

#include "GraphicShader.h"

class CLineMeshShader :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CLineMeshShader();
	virtual ~CLineMeshShader();

public:
	virtual bool Init();
};

