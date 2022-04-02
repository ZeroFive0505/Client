#include "ScrollMap.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CScrollMap::CScrollMap() :
	m_ScrollRatio(1.0f, 1.0f),
	m_Loop(false)
{
}

CScrollMap::~CScrollMap()
{
}

void CScrollMap::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	SetTexture(Name);
}

void CScrollMap::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	SetTexture(Name);
}

void CScrollMap::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	SetTexture(Name);
}

void CScrollMap::SetTexture(const std::string& Name)
{
	m_ScrollTexture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CScrollMap::SetTextureColorKey(const unsigned char r, const unsigned char g, const unsigned char b)
{
	if (m_ScrollTexture)
		m_ScrollTexture->SetColorKey(r, g, b);
}

void CScrollMap::Start()
{
	CMapBase::Start();

	if (m_ScrollTexture)
		m_TextureSize = Vector2((float)m_ScrollTexture->GetWidth(), (float)m_ScrollTexture->GetHeight());
}

bool CScrollMap::Init()
{
	CMapBase::Init();

	return true;
}

void CScrollMap::Update(float DeltaTile)
{
	CMapBase::Update(DeltaTile);
}

void CScrollMap::PostUpdate(float DeltaTile)
{
	CMapBase::PostUpdate(DeltaTile);
}

void CScrollMap::PrevRender()
{
	CMapBase::PrevRender();
}

void CScrollMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);

	if (m_ScrollTexture)
	{
		if (!m_Loop)
		{
			CCamera* Camera = m_Scene->GetCamera();

			Vector2 Resolution = Camera->GetResolution();

			Vector2 ImagePos = Camera->GetPos() * m_ScrollRatio;

			unsigned int Width = m_ScrollTexture->GetWidth();
			unsigned int Height = m_ScrollTexture->GetHeight();

			ImagePos.x = ImagePos.x < 0.0f ? 0.0f : ImagePos.x;
			ImagePos.x = ImagePos.x + Resolution.x > Width ? Width - Resolution.x : ImagePos.x;

			ImagePos.y = ImagePos.y < 0.0f ? 0.0f : ImagePos.y;
			ImagePos.y = ImagePos.y + Resolution.y > Height ? Height - Resolution.y : ImagePos.y;

			m_ScrollTexture->Render(hDC, m_Pos, ImagePos, m_Size);
		}
		else
		{
			CCamera* Camera = m_Scene->GetCamera();

			Vector2 CameraPos = Camera->GetPos();

			Vector2 Resolution = Camera->GetResolution();

			Vector2 ImagePos = Camera->GetPos() * m_ScrollRatio;

			unsigned int Width = m_ScrollTexture->GetWidth();
			unsigned int Height = m_ScrollTexture->GetHeight();

			// 좌상단, 우상단, 좌하단, 우하단 출력
			unsigned int WidthRatio = (unsigned int)(CameraPos.x / Width);
			unsigned int HeigthRatio = (unsigned int)(CameraPos.y / Height);

			Vector2 ConvertPos;
			ConvertPos.x = CameraPos.x - WidthRatio * Width;
			ConvertPos.y = CameraPos.y - HeigthRatio * Height;

			Vector2 ConvertLeftTopSize;
			ConvertLeftTopSize.x = Width - ConvertPos.x;
			ConvertLeftTopSize.y = Height - ConvertPos.y;

			// 좌 상단 출력
			// ConvertPos = (x, y)
			// 먼저 좌상단의 경우에는 x, y좌표는 그대로 살아남아 이미지의 좌표가 된다.
			// 윈도우창에서 이미지 출력위치는 0, 0부터 출력한다.
			m_ScrollTexture->Render(hDC, Vector2(0.0f, 0.0f), ConvertPos, ConvertLeftTopSize);

			// 우 상단 출력
			// 우 상단의 경우 x축으로 넓이가 넘어가버렸다.
			// 따라서 ConvertPos의 x의 값은 다시 0으로 초기화된다고 생각한다.
			// 이미지의 출력위치는 ConvertLeftTopSize.x 만큼 오른쪽으로 이동시키고
			// 이미지의 좌표는 x는 0, y의 경우 전체 높이에서 ConvertLeftTopSize.y를빼서 정확한 좌표를 구한다.
			// 출력의 크기는 x의 경우 화면을 넘어가버린 경우이니 전체 카메라 해상도에서 ConvertLeftTopSize.x 만큼 빼준다.
			// y의 경우 좌 상단과 같이 동일한 크기가 된다.
			// ImgPos의 경우 이미지의 좌표이니 Height(이미지의 높이 값을 이용해서 계산)
			// 마지막으로 사이즈의 값의 경우 화면에 출력이 되는 것이니 화면의 해상도 즉 카메라의 해상도를 이용해서 크기를 계산한다.
			m_ScrollTexture->Render(hDC, Vector2(ConvertLeftTopSize.x, 0.0f), 
				Vector2(0.0f, Height - ConvertLeftTopSize.y),
				Vector2(Resolution.x - ConvertLeftTopSize.x, ConvertLeftTopSize.y));

			// 좌 하단 출력
			// 좌 하단의 경우 y의 값이 초기화된다.
			// 윈도우상에서 이미지 출력위치는 좌 상단의 이미지크기만큼 부터 시작하며
			// 이미지의 좌표는 x의 값은 좌 상단의 값을 그대로 이용하고 y의 값은 0으로 초기화한다.
			// 이미지의 x 크기는 좌 상단의 크기만큼 출력하고 y의 경우 전체 해상도에서 ConvertLeftTopSize.y만큼을 제거한다.
			m_ScrollTexture->Render(hDC, Vector2(0.0f, ConvertLeftTopSize.y),
				Vector2(ConvertPos.x, 0.0f),
				Vector2(ConvertLeftTopSize.x, Resolution.y - ConvertLeftTopSize.y));


			// 우 하단 출력
			// 우 하단의 경우 x, y값이 모두 범위를 벗어난 경우이다.
			// 윈도우상에서 출력위치는 ConverLeftTopSize이며
			// 이미지상에서 좌표는 x, y 모두 0으로 초기화한다.
			// 출력의 경우 카메라의 해상도에서 ConvertLeftSize만큼을 제거한다.
			m_ScrollTexture->Render(hDC, Vector2(ConvertLeftTopSize.x, ConvertLeftTopSize.y),
				Vector2(0.0f, 0.0f), 
				Vector2(Resolution.x - ConvertLeftTopSize.x, Resolution.y - ConvertLeftTopSize.y));
		}
	}
}

void CScrollMap::Save(FILE* pFile)
{
}

void CScrollMap::Load(FILE* pFile)
{
}
