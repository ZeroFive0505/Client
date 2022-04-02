#pragma once

#include "../Ref.h"

/// <summary>
/// ��Ʈ�� �����ϴ� Ŭ����
/// </summary>
class CFont :
	public CRef
{
	friend class CResourceManager;

private:
	CFont();
	~CFont();

private:
	// ���� ��Ʈ�� ����
	LOGFONT m_FontInfo;
	// ���� ��Ʈ
	HFONT m_Font;
	// ���� ��Ʈ
	HFONT m_PrevFont;

public:
	bool LoadFont(const std::string& Name, const TCHAR* FontName,
		int Width = 0, int Height = 0);
	void SetFont(HDC hDC);
	void ResetFont(HDC hDC);
};