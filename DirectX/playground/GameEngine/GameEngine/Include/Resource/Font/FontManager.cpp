#include "FontManager.h"
#include "../../PathManager.h"
#include "../../Device.h"

CFontManager::CFontManager() :
	m_WriteFactory(nullptr),
	m_FontFamilyName{},
	m_FontFamilyNameMultibyte{}
{
}

CFontManager::~CFontManager()
{
	{
		auto iter = m_mapFontFile.begin();
		auto iterEnd = m_mapFontFile.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto iter = m_mapFont.begin();
		auto iterEnd = m_mapFont.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto iter = m_mapFontColor.begin();
		auto iterEnd = m_mapFontColor.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_RELEASE(iter->second);
		}
	}


	SAFE_RELEASE(m_WriteFactory);
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory),
		(IUnknown**)&m_WriteFactory)))
		return false;

	// Engine font loading
	CreateFontFile("Default", TEXT("NotoSansKR-Regular.otf"));
	CreateFontFile("InGame", TEXT("Bangers.ttf"));

	const TCHAR* fontFace = GetFontFaceName("Default");

	LoadFont("Default", fontFace, 600, 20.0f, TEXT("ko"));
	LoadFont("InGame", GetFontFaceName("InGame"), 600, 20.0f, TEXT("en"));

	return true;
}

bool CFontManager::CreateFontFile(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	IDWriteFontCollection1* fontCollection = FindFontFile(name);

	// 폰트 콜렉션이 존재할경우는 그냥 반환
	if (fontCollection)
		return true;

	// 폰트를 완전경로로 불러온다.
	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	IDWriteFontFile* fontFile = nullptr;

	// 완전 경로로 폰트파일을을 불러온다.
	if (FAILED(m_WriteFactory->CreateFontFileReference(fullPath, nullptr, &fontFile)))
		return false;

	IDWriteFontSetBuilder1* builder = nullptr;

	if (FAILED(m_WriteFactory->CreateFontSetBuilder(&builder)))
		return false;
	
	// 폰트 파일을 추가한다.
	builder->AddFontFile(fontFile);

	IDWriteFontSet* fontSet = nullptr;

	if (FAILED(builder->CreateFontSet(&fontSet)))
		return false;
	
	// 폰트 콜렉션을 만든다.
	if (FAILED(m_WriteFactory->CreateFontCollectionFromFontSet(fontSet, &fontCollection)))
		return false;

	m_mapFontFile.insert(std::make_pair(name, fontCollection));

	// 참조 횟수는 초기화
	SAFE_RELEASE(fontSet);
	SAFE_RELEASE(builder);
	SAFE_RELEASE(fontFile);

	return true;
}

bool CFontManager::LoadFont(const std::string& name, const TCHAR* fontName, int weight, 
	float fontSize, const TCHAR* localName, int stretch)
{
	IDWriteTextFormat* font = FindFont(name);

	if (font)
		return true;

	if (FAILED(m_WriteFactory->CreateTextFormat(fontName, nullptr, (DWRITE_FONT_WEIGHT)weight,
		DWRITE_FONT_STYLE_NORMAL, (DWRITE_FONT_STRETCH)stretch, fontSize, localName, &font)))
		return false;

	m_mapFont.insert(std::make_pair(name, font));

	return true;
}

const TCHAR* CFontManager::GetFontFaceName(const std::string& name)
{
	IDWriteFontCollection1* fontCollection = FindFontFile(name);

	if (!fontCollection)
		return nullptr;

	IDWriteFontFamily* family = nullptr;

	if (FAILED(fontCollection->GetFontFamily(0, &family)))
		return nullptr;

	IDWriteLocalizedStrings* localizedName = nullptr;

	if(FAILED(family->GetFamilyNames(&localizedName)))
		return nullptr;

	memset(m_FontFamilyName, 0, sizeof(TCHAR) * 256);

	if (FAILED(localizedName->GetString(0, m_FontFamilyName, 256)))
		return nullptr;

	return m_FontFamilyName;
}

const char* CFontManager::GetFontFaceNameMultibyte(const std::string& name)
{
	if (!GetFontFaceName(name))
		return nullptr;

	memset(m_FontFamilyNameMultibyte, 0, 256);

#ifdef UNICODE

	int length = WideCharToMultiByte(CP_ACP, 0, m_FontFamilyName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, m_FontFamilyName, -1, m_FontFamilyNameMultibyte, length, 0, 0);

#else

	strcpy_s(m_FontFamilyNameMultibyte, m_FontFamilyName);

#endif // UNICODE

	return m_FontFamilyNameMultibyte;
}

bool CFontManager::CreateFontColor(float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* brush = FindFontColor(r, g, b, a);

	if (brush)
		return true;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a),
		&brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), brush));

	return true;
}

bool CFontManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	ID2D1SolidColorBrush* brush = FindFontColor(r, g, b, a);

	if (brush)
		return true;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f), &brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), brush));

	return true;
}

bool CFontManager::CreateFontColor(const Vector4& color)
{
	ID2D1SolidColorBrush* brush = FindFontColor(color);

	if (brush)
		return true;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(color.x, color.y, color.z, color.w), &brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(color), brush));

	return true;
}

bool CFontManager::CreateFontColor(unsigned int color)
{
	ID2D1SolidColorBrush* brush = FindFontColor(color);

	if (brush)
		return true;

	float r, g, b, a;

	// 비트 쉬프트와 마스킹을 통해서 원하는 곳의 8비트의 값을 가져온다.
	b = (color & 0x000000ff) / 255.0f;
	g = ((color >> 8) & 0x000000ff) / 255.0f;
	r = ((color >> 16) & 0x000000ff) / 255.0f;
	a = ((color >> 24) & 0x000000ff) / 255.0f;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a), &brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(color, brush));

	return true;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(float r, float g, float b, float a)
{
	return FindFontColor(CreateFontColorKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return FindFontColor(CreateFontColorKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const Vector4& color)
{
	return FindFontColor(CreateFontColorKey(color));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned int color)
{
	auto iter = m_mapFontColor.find(color);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

unsigned int CFontManager::CreateFontColorKey(float r, float g, float b, float a)
{
	unsigned int color = 0;
	unsigned char R, G, B, A;

	R = (unsigned int)(r * 255);
	G = (unsigned int)(g * 255);
	B = (unsigned int)(b * 255);
	A = (unsigned int)(a * 255);

	// 비트 쉬프트와 비트 온 연산을 이용해서 총 4바이트에 ARGB순으로 색 정보를넣는다.
	color = A;
	color = (color << 8) | R;
	color = (color << 8) | G;
	color = (color << 8) | B;

	return color;
}

unsigned int CFontManager::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	unsigned int color = 0;

	color = a;
	color = (color << 8) | r;
	color = (color << 8) | g;
	color = (color << 8) | b;

	return color;
}

unsigned int CFontManager::CreateFontColorKey(const Vector4& color)
{
	unsigned int ret = 0;
	unsigned char R, G, B, A;

	R = (unsigned char)(color.x * 255.0f);
	G = (unsigned char)(color.y * 255.0f);
	B = (unsigned char)(color.z * 255.0f);
	A = (unsigned char)(color.w * 255.0f);

	ret = A;
	ret = (ret << 8) | R;
	ret = (ret << 8) | G;
	ret = (ret << 8) | B;

	return ret;
}

IDWriteTextLayout* CFontManager::CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, float width, float height)
{
	IDWriteTextLayout* layout = nullptr;

	if (FAILED(m_WriteFactory->CreateTextLayout(text, lstrlen(text), font, width, height, &layout)))
		return nullptr;

	return layout;
}

IDWriteTextLayout* CFontManager::CreateTextLayout(const TCHAR* text, const std::string& fontName, float width, float height)
{
	IDWriteTextFormat* font = FindFont(fontName);

	if (!font)
		return nullptr;

	return CreateTextLayout(text, font, width, height);
}

IDWriteFontCollection1* CFontManager::FindFontFile(const std::string& name)
{
	auto iter = m_mapFontFile.find(name);

	if (iter == m_mapFontFile.end())
		return nullptr;

	return iter->second;
}

IDWriteTextFormat* CFontManager::FindFont(const std::string& name)
{
	auto iter = m_mapFont.find(name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}
