#pragma once

#include "GraphicShader.h"


// 텍스쳐를 입힌 메쉬 클래스
class CMesh2DShader :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CMesh2DShader();
	virtual ~CMesh2DShader();

public:
	virtual bool Init();
};

