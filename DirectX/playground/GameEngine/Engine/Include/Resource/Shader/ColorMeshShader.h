#pragma once

#include "GraphicShader.h"

/// <summary>
/// �⺻���� Į�� �޽��� ������ Ŭ�����̴�.
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

