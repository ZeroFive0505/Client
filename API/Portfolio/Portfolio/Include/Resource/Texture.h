#pragma once

#include "../Ref.h"

struct TextureInfo
{
	HDC hDC;
	HBITMAP hBmp;
	HBITMAP hPrev;
	BITMAP bmpInfo;
	bool colorKeyEnable;
	unsigned int colorKey;
	bool scaleEnable;
	float xScale;
	float yScale;
	BITMAPINFO myBitmapInfo;
	COLORREF* pixelArray;

	TextureInfo() :
		hDC(0),
		hBmp(0),
		hPrev(0),
		bmpInfo{},
		colorKeyEnable(false),
		scaleEnable(false),
		xScale(0),
		yScale(0),
		colorKey(0),
		myBitmapInfo{ 0 },
		pixelArray(nullptr)
	{

	}

	~TextureInfo()
	{
		SAFE_DELETE_ARRAY(pixelArray);

		SelectObject(hDC, hPrev);

		DeleteObject(hBmp);

		DeleteDC(hDC);
	}
};

class CTexture :
	public CRef
{
public:
	CTexture();
	~CTexture();

	void operator=(const CTexture& rhs);

	CTexture(const CTexture& rhs);

private:
	ETexture_Type m_TextureType;
	std::vector<TextureInfo*> m_vecTextureInfo;
	Vector2 m_Pivot;
	Vector2 m_Pos;
	Vector2 m_Size;
	Vector2 m_Offset;

public:
	inline ETexture_Type GetTextureType() const
	{
		return m_TextureType;
	}

	inline unsigned int GetWidth(int idx = 0) const
	{
		return (unsigned int)m_vecTextureInfo[idx]->bmpInfo.bmWidth;
	}

	inline unsigned int GetHeight(int idx = 0) const
	{
		return (unsigned int)m_vecTextureInfo[idx]->bmpInfo.bmHeight;
	}

	inline Vector2 GetScale(int idx = 0) const
	{
		Vector2 scale(m_vecTextureInfo[idx]->xScale, m_vecTextureInfo[idx]->yScale);

		return scale;
	}

	inline Vector2 GetPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetPivot() const
	{
		return m_Pivot;
	}

	inline Vector2 GetOffset() const
	{
		return m_Offset;
	}

	inline Vector2 GetSize() const
	{
		return m_Size;
	}

	inline bool IsEmpty() const
	{
		return m_vecTextureInfo.empty();
	}

public:

	inline void SetPos(const Vector2& pos)
	{
		m_Pos = pos;
	}

	inline void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	inline void Move(float x, float y)
	{
		m_Pos.x += x;
		m_Pos.y += y;
	}

	inline void SetPivot(const Vector2& pivot)
	{
		m_Pivot = pivot;
	}

	inline void SetPivot(float x, float y)
	{
		m_Pivot.x = x / GetWidth();
		m_Pivot.y = y / GetHeight();
	}

	inline void SetOffset(const Vector2& offset)
	{
		m_Offset = offset;
	}

	inline void SetOffset(float x, float y)
	{
		m_Offset = Vector2(x, y);
	}

	inline void SetColorKey(const unsigned char r, const unsigned char g, const unsigned char b, int index = 0)
	{
		m_vecTextureInfo[index]->colorKeyEnable = true;
		m_vecTextureInfo[index]->colorKey = RGB(r, g, b);
	}

	inline void SetScale(const float xScale, const float yScale, int index = 0)
	{
		m_vecTextureInfo[index]->scaleEnable = true;
		m_vecTextureInfo[index]->xScale = xScale;
		m_vecTextureInfo[index]->yScale = yScale;
	}

	inline void SetScale(const Vector2& scale, int index = 0)
	{
		m_vecTextureInfo[index]->scaleEnable = true;
		m_vecTextureInfo[index]->xScale = scale.x;
		m_vecTextureInfo[index]->yScale = scale.y;
	}

	inline void Scale(const float xScale, const float yScale, int index = 0)
	{
		m_vecTextureInfo[index]->xScale *= xScale;
		m_vecTextureInfo[index]->yScale *= yScale;
	}

	inline void Scale(const Vector2& scale, int index = 0)
	{
		m_vecTextureInfo[index]->xScale *= scale.x;
		m_vecTextureInfo[index]->yScale *= scale.y;
	}

public:

	HBITMAP GetHBITMAP(int index = 0) const
	{
		return m_vecTextureInfo[index]->hBmp;
	}


	HDC GetTextureDC(int index = 0) const
	{
		return m_vecTextureInfo[index]->hDC;
	}

	unsigned int GetColorKey(int index = 0) const
	{
		return m_vecTextureInfo[index]->colorKey;
	}

	inline CTexture* Clone()
	{
		return new CTexture(*this);
	}

public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	void Render(HDC hDC, const Vector2& windowPos, const Vector2& imgPos, const Vector2& size, int index = 0);
	void RenderByFactor(HDC hDC, const Vector2& windowPos, const Vector2& imgPos, const Vector2& size, float factor, int index = 0);
	void Render(HDC hDC, const Vector2& imgPos, const Vector2& size, int index = 0);
	void Create(const Vector2& size, COLORREF* colors);
	void Create(const float x, const float y, COLORREF* colors);
	COLORREF PixelArrayAt(const Vector2& pos, int index = 0);
	COLORREF PixelArrayAt(int x, int y, int index = 0);
	void ResetPixelArray(int index = 0);
	void SetPixelArray(int index = 0);
};