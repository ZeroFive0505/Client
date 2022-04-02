#pragma once

#include "GraphicShader.h"

/// <summary>
/// 기본적인 칼라 메쉬를 만들어내는 클래스이다.
/// </summary>
class CColorMeshShader :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CColorMeshShader();
	virtual ~CColorMeshShader();

public:
	virtual bool Init();
};

