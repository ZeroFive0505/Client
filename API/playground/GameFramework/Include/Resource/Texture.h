#pragma once

#include "../Ref.h"

struct TextureInfo
{
	// 이미지파일을 불러와서 HBITMAP을 만들고 이 HBITMAP을 화면에 출력할
	// 메모리DC를 만들어서 출력하게 한다.
	HDC		hDC;
	HBITMAP	hBmp;
	HBITMAP	hPrev;
	BITMAP	BmpInfo;
	bool	ColorKeyEnable;
	unsigned int	ColorKey;
	TCHAR FileName[MAX_PATH];

	TextureInfo() :
		hDC(0),
		hBmp(0),
		hPrev(0),
		BmpInfo{},
		ColorKeyEnable(false),
		ColorKey(0),
		FileName{}
	{
	}

	~TextureInfo()
	{
		// 원래의 HGDIOBJ로 되돌려준다.
		SelectObject(hDC, hPrev);

		// HBITMAP과 HDC를 제거한다.
		DeleteObject(hBmp);

		DeleteDC(hDC);
	}
};
/// <summary>
/// 텍스쳐또한 레퍼런스 카운팅을 적용한다. 해당 텍스쳐의 참조자의 수가 1이면 리소스매니저만 참조하고있다는 뜻이다.
/// </summary>
class CTexture :
	public CRef
{
public:
	CTexture();
	~CTexture();

private:
	// 텍스쳐의 타입 낱장인지 한장인지
	ETexture_Type		m_TextureType;
	// 해당 텍스쳐의 대한 정보
	std::vector<TextureInfo*>	m_vecTextureInfo;

public:
	ETexture_Type GetTextureType()	const
	{
		return m_TextureType;
	}

	unsigned int GetWidth(int idx = 0)	const
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmWidth;
	}

	unsigned int GetHeight(int idx = 0)	const
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmHeight;
	}

	void SetColorKey(const unsigned char r, const unsigned char g, const unsigned char b, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = RGB(r, g, b);
	}

	void SetColorKey(unsigned int ColorKey, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = ColorKey;
	}

public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImgPos,
		const Vector2& Size, int Index = 0);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

	static CTexture* LoadStatic(FILE* pFile, class CScene* Scene);
};

