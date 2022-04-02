#pragma once

#include "../../GameInfo.h"

class CFontManager
{
	friend class CResourceManager;

private:
	CFontManager();
	~CFontManager();

private:
	IDWriteFactory5* m_WriteFactory;
	std::unordered_map<std::string, IDWriteFontCollection1*> m_mapFontFile;
	std::unordered_map<std::string, IDWriteTextFormat*> m_mapFont;
	// R, G, B, A의 총 32비트의 칼라키를 통해 브러쉬를 지정한다.
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*> m_mapFontColor;
	TCHAR m_FontFamilyName[256];
	char m_FontFamilyNameMultibyte[256];

public:
	bool Init();

	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);
	bool LoadFont(const std::string& name, const TCHAR* fontName, int weight, float fontSize,
		const TCHAR* localName, int stretch = DWRITE_FONT_STRETCH_NORMAL);

	const TCHAR* GetFontFaceName(const std::string& name);
	const char* GetFontFaceNameMultibyte(const std::string& name);

	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(unsigned int color);

	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int color);

	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& color);

	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, float width, float height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, const std::string& fontName, float width, float height);

	IDWriteFontCollection1* FindFontFile(const std::string& name);
	IDWriteTextFormat* FindFont(const std::string& name);
};

