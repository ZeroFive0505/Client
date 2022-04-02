#pragma once

#include "Shader.h"

/// <summary>
/// 화면에 렌더링 될때 이용되는 쉐이더이다.
/// </summary>
class CGraphicShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CGraphicShader();
	CGraphicShader(const CGraphicShader& shader);
	virtual ~CGraphicShader() = 0;

private:
	// 쉐이더 입력시 레이아웃을 설정
	ID3D11InputLayout* m_InputLayout;
	// 버텍스마다 다양한 입력 레이아웃을 가질 수 있다.
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputDesc;
	unsigned int m_InputSize;

private:
	// 버텍스 쉐이더
	ID3D11VertexShader* m_VS;
	// 버텍스 쉐이더 바이트 코드
	ID3DBlob* m_VSBlob;

	// 픽셀 쉐이더
	ID3D11PixelShader* m_PS;
	// 픽셀 쉐이더 바이트 코드
	ID3DBlob* m_PSBlob;

	// 헐 쉐이더
	ID3D11HullShader* m_HS;
	// 헐 쉐이더 바이트 코드
	ID3DBlob* m_HSBlob;

	// 도메인 쉐이더
	ID3D11DomainShader* m_DS;
	// 도메인 쉐이더 바이트 코드
	ID3DBlob* m_DSBlob;

	// 지오메트리 쉐이더
	ID3D11GeometryShader* m_GS;
	// 지오메트리 쉐이더 바이트 코드
	ID3DBlob* m_GSBlob;

public:
	void AddInputDesc(const char* name, unsigned int index, DXGI_FORMAT fmt, unsigned int slot, unsigned int size,
		D3D11_INPUT_CLASSIFICATION slotClass, unsigned int instanceDataStep);

	bool CreateInputLayout();

public:
	virtual bool Init() = 0;
	virtual void SetShader();
	virtual CGraphicShader* Clone();

public:
	bool LoadVertexShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadPixelShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadHullShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadDomainShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadGeometryShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
};

