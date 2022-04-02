#include "Texture.h"
#include "../PathManager.h"
#include "../GameManager.h"

CTexture::CTexture() :
	m_Pivot(0.0f, 0.0f),
	m_Pos(0.0f, 0.0f),
	m_Offset(0.0f, 0.0f)
{
}

CTexture::~CTexture()
{
	size_t size = m_vecTextureInfo.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

void CTexture::operator=(const CTexture& rhs)
{
	SetName(rhs.GetName());

	for (size_t i = 0; i < m_vecTextureInfo.size(); i++)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}

	m_vecTextureInfo.clear();

	TextureInfo* info = new TextureInfo;

	info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	info->hBmp = (HBITMAP)CopyImage(rhs.GetHBITMAP(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

	info->hPrev = (HBITMAP)SelectObject(info->hDC, info->hBmp);

	GetObject(info->hBmp, sizeof(BITMAP), &info->bmpInfo);

	m_vecTextureInfo.push_back(info);

	m_Size.x = (float)info->bmpInfo.bmWidth;
	m_Size.y = (float)info->bmpInfo.bmHeight;

	m_TextureType = ETexture_Type::Atlas;
}

CTexture::CTexture(const CTexture& rhs)
{
	SetName(rhs.GetName());
	TextureInfo* info = new TextureInfo;

	info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	info->hBmp = (HBITMAP)CopyImage(rhs.GetHBITMAP(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

	info->hPrev = (HBITMAP)SelectObject(info->hDC, info->hBmp);

	GetObject(info->hBmp, sizeof(BITMAP), &info->bmpInfo);

	m_vecTextureInfo.push_back(info);

	m_Size.x = (float)info->bmpInfo.bmWidth;
	m_Size.y = (float)info->bmpInfo.bmHeight;

	m_TextureType = ETexture_Type::Atlas;
}

bool CTexture::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	TCHAR fullPath[MAX_PATH] = {};

	if (info)
		lstrcpy(fullPath, info->unicodePath);
	lstrcat(fullPath, fileName);

	return LoadTextureFullPath(name, fullPath);
}

bool CTexture::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	SetName(name);

	TextureInfo* info = new TextureInfo;

	info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		fullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	info->hPrev = (HBITMAP)SelectObject(info->hDC, info->hBmp);

	GetObject(info->hBmp, sizeof(BITMAP), &info->bmpInfo);

	m_vecTextureInfo.push_back(info);

	m_Size.x = (float)info->bmpInfo.bmWidth;
	m_Size.y = (float)info->bmpInfo.bmHeight;


	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;
	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

bool CTexture::LoadTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	size_t size = vecFileName.size();

	for (size_t i = 0; i < size; i++)
	{
		TCHAR fullPath[MAX_PATH] = {};

		if (info)
			lstrcpy(fullPath, info->unicodePath);
		lstrcat(fullPath, vecFileName[i].c_str());

		if (!LoadTextureFullPath(name, fullPath))
			return false;
	}


	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;
	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

void CTexture::Render(HDC hDC, const Vector2& windowPos, const Vector2& imgPos, const Vector2& size, int index)
{

	Vector2 LT = windowPos - m_Pivot * m_Size + m_Offset;

	if (m_vecTextureInfo[index]->colorKeyEnable)
	{
		if (!m_vecTextureInfo[index]->scaleEnable)
			TransparentBlt(hDC, (int)LT.x, (int)LT.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->hDC,
				(int)imgPos.x, (int)imgPos.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->colorKey);
		else if (m_vecTextureInfo[index]->scaleEnable)
			TransparentBlt(hDC, (int)LT.x, (int)LT.y, int(size.x * m_vecTextureInfo[index]->xScale), int(size.y * m_vecTextureInfo[index]->yScale),
				m_vecTextureInfo[index]->hDC,
				(int)imgPos.x, (int)imgPos.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->colorKey);
	}
	else
	{
		if (!m_vecTextureInfo[index]->scaleEnable)
			BitBlt(hDC, (int)LT.x, (int)LT.y, (int)size.x, (int)size.y,
				m_vecTextureInfo[index]->hDC, (int)imgPos.x, (int)imgPos.y, SRCCOPY);
		else if (m_vecTextureInfo[index]->scaleEnable)
			StretchBlt(hDC, (int)LT.x, (int)LT.y, int(size.x * m_vecTextureInfo[index]->xScale),
				int(size.y * m_vecTextureInfo[index]->yScale), m_vecTextureInfo[index]->hDC, (int)imgPos.x, (int)imgPos.y,
				(int)size.x, (int)size.y, SRCCOPY);
	}
}

void CTexture::RenderByFactor(HDC hDC, const Vector2& windowPos, const Vector2& imgPos, const Vector2& size, float factor, int index)
{
	Vector2 LT;

	if (factor > 1.0f)
		LT = windowPos - m_Pivot * (m_Size / factor) + m_Offset;
	else
		LT = windowPos - m_Pivot * m_Size + m_Offset;


	if (m_vecTextureInfo[index]->colorKeyEnable)
	{
		if (!m_vecTextureInfo[index]->scaleEnable)
			TransparentBlt(hDC, (int)LT.x, (int)LT.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->hDC,
				(int)imgPos.x, (int)imgPos.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->colorKey);
		else if (m_vecTextureInfo[index]->scaleEnable)
			TransparentBlt(hDC, (int)LT.x, (int)LT.y, int(size.x * m_vecTextureInfo[index]->xScale), int(size.y * m_vecTextureInfo[index]->yScale),
				m_vecTextureInfo[index]->hDC,
				(int)imgPos.x, (int)imgPos.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->colorKey);
	}
	else
	{
		if (!m_vecTextureInfo[index]->scaleEnable)
			BitBlt(hDC, (int)LT.x, (int)LT.y, (int)size.x, (int)size.y,
				m_vecTextureInfo[index]->hDC, (int)imgPos.x, (int)imgPos.y, SRCCOPY);
		else if (m_vecTextureInfo[index]->scaleEnable)
			StretchBlt(hDC, (int)LT.x, (int)LT.y, int(size.x * m_vecTextureInfo[index]->xScale),
				int(size.y * m_vecTextureInfo[index]->yScale), m_vecTextureInfo[index]->hDC, (int)imgPos.x, (int)imgPos.y,
				(int)size.x, (int)size.y, SRCCOPY);
	}
}


void CTexture::Render(HDC hDC, const Vector2& imgPos, const Vector2& size, int index)
{
	Vector2 LT = m_Pos - m_Pivot * m_Size + m_Offset;

	if (m_vecTextureInfo[index]->colorKeyEnable)
	{
		if (!m_vecTextureInfo[index]->scaleEnable)
			TransparentBlt(hDC, (int)LT.x, (int)LT.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->hDC,
				(int)imgPos.x, (int)imgPos.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->colorKey);
		else if (m_vecTextureInfo[index]->scaleEnable)
			TransparentBlt(hDC, (int)LT.x, (int)LT.y, int(size.x * m_vecTextureInfo[index]->xScale), int(size.y * m_vecTextureInfo[index]->yScale),
				m_vecTextureInfo[index]->hDC,
				(int)imgPos.x, (int)imgPos.y, (int)size.x, (int)size.y, m_vecTextureInfo[index]->colorKey);
	}
	else
	{
		if (!m_vecTextureInfo[index]->scaleEnable)
			BitBlt(hDC, (int)LT.x, (int)LT.y, (int)size.x, (int)size.y,
				m_vecTextureInfo[index]->hDC, (int)imgPos.x, (int)imgPos.y, SRCCOPY);
		else if (m_vecTextureInfo[index]->scaleEnable)
			StretchBlt(hDC, (int)LT.x, (int)LT.y, int(size.x * m_vecTextureInfo[index]->xScale),
				int(size.y * m_vecTextureInfo[index]->yScale), m_vecTextureInfo[index]->hDC, (int)imgPos.x, (int)imgPos.y,
				(int)size.x, (int)size.y, SRCCOPY);
	}
}

void CTexture::Create(const Vector2& size, COLORREF* colors)
{
	TextureInfo* info = new TextureInfo;

	info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	info->hBmp = CreateBitmap((int)size.x, (int)size.y, 1, sizeof(COLORREF) * 8, (void*)colors);

	info->hPrev = (HBITMAP)SelectObject(info->hDC, info->hBmp);

	GetObject(info->hBmp, sizeof(BITMAP), &info->bmpInfo);

	m_vecTextureInfo.push_back(info);

	m_TextureType = ETexture_Type::Atlas;
}

void CTexture::Create(const float x, const float y, COLORREF* colors)
{
	TextureInfo* info = new TextureInfo;

	info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	info->hBmp = CreateBitmap((int)x, (int)y, 1, sizeof(COLORREF) * 8, (void*)colors);

	info->hPrev = (HBITMAP)SelectObject(info->hDC, info->hBmp);

	GetObject(info->hBmp, sizeof(BITMAP), &info->bmpInfo);

	m_vecTextureInfo.push_back(info);

	m_TextureType = ETexture_Type::Atlas;
}

COLORREF CTexture::PixelArrayAt(const Vector2& pos, int index)
{
	int width = m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biWidth;
	return m_vecTextureInfo[index]->pixelArray[(int)pos.y * width + (int)pos.x];
}

COLORREF CTexture::PixelArrayAt(int x, int y, int index)
{
	int width = m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biWidth;
	return m_vecTextureInfo[index]->pixelArray[(int)y * width + (int)x];
}

void CTexture::ResetPixelArray(int index)
{
	if (m_vecTextureInfo[index] == nullptr)
		return;

	int width = m_vecTextureInfo[index]->bmpInfo.bmWidth;
	int height = m_vecTextureInfo[index]->bmpInfo.bmHeight;

	int ret;


	ret = GetDIBits(m_vecTextureInfo[index]->hDC, m_vecTextureInfo[index]->hBmp, 0, m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biHeight,
		(LPVOID)m_vecTextureInfo[index]->pixelArray, &m_vecTextureInfo[index]->myBitmapInfo, DIB_RGB_COLORS);

}

void CTexture::SetPixelArray(int index)
{
	if (m_vecTextureInfo[index] == nullptr)
		return;

	int width = m_vecTextureInfo[index]->bmpInfo.bmWidth;
	int height = m_vecTextureInfo[index]->bmpInfo.bmHeight;

	m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biSize = sizeof(m_vecTextureInfo[index]->myBitmapInfo.bmiHeader);

	int ret;

	ret = GetDIBits(m_vecTextureInfo[index]->hDC, m_vecTextureInfo[index]->hBmp, 0, 0, nullptr, &m_vecTextureInfo[index]->myBitmapInfo,
		DIB_RGB_COLORS);

	m_vecTextureInfo[index]->pixelArray = new COLORREF[width * height];

	m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biCompression = BI_RGB;

	// 위에서 아래로 내려오는 이미지로 만들려면 부호를 반대로 만들어준다.
	m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biHeight = -m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biHeight;

	ret = GetDIBits(m_vecTextureInfo[index]->hDC, m_vecTextureInfo[index]->hBmp, 0, m_vecTextureInfo[index]->myBitmapInfo.bmiHeader.biHeight,
		(LPVOID)m_vecTextureInfo[index]->pixelArray, &m_vecTextureInfo[index]->myBitmapInfo, DIB_RGB_COLORS);
}