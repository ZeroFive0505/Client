#pragma once

#include "../Ref.h"

/// <summary>
/// 폰트를 관리하는 클래스
/// </summary>
class CFont :
	public CRef
{
	friend class CResourceManager;

private:
	CFont();
	~CFont();

private:
	// 현재 폰트의 정보
	LOGFONT m_FontInfo;
	// 지금 폰트
	HFONT m_Font;
	// 이전 폰트
	HFONT m_PrevFont;

public:
	bool LoadFont(const std::string& Name, const TCHAR* FontName,
		int Width = 0, int Height = 0);
	void SetFont(HDC hDC);
	void ResetFont(HDC hDC);
};