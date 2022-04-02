#pragma once

#include "../Ref.h"

class CFont :
	public CRef
{
	friend class CResourceManager;

private:
	CFont();
	~CFont();

private:
	LOGFONT m_FontInfo;
	HFONT m_Font;
	HFONT m_PrevFont;

public:
	bool LoadFont(const std::string& name, const TCHAR* fontName, int width = 0, int height = 0);
	void SetFont(HDC hDC);
	void ResetFont(HDC hDC);
};

