#include "TileMap.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CTileMap::CTileMap() :
	m_TileCountX(0),
	m_TileCountY(0),
	m_StartX(-1),
	m_StartY(-1),
	m_EndX(-1),
	m_EndY(-1)
{
}

CTileMap::~CTileMap()
{
	// 소멸시 잊지말고 모든 타일들을 제거한다.
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();
}

bool CTileMap::CreateTile(int CountX, int CountY, const Vector2& TileSize)
{
	m_TileCountX = CountX;
	m_TileCountY = CountY;
	m_TileSize = TileSize;

	// 기존에 배치된 타일들을 모두 제거한다.
	{
		auto iter = m_vecTile.begin();
		auto iterEnd = m_vecTile.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE((*iter));
		}

		m_vecTile.clear();
	}

	// m_Sizep에는 전체 맵의 크기가 들어갈 것이다. (타일의 가로 세로 크기) * (타일의 가로 개수, 타일의 세로 개수)
	m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

	// 타일 반복문을 효과적으로 돌리기위해서 현재 카메라의 좌표를 가져온다.
	CCamera* Camera = m_Scene->GetCamera();
	// 카메라의 최대 해상도는 전체 맵의 크기이다.
	Camera->SetWorldResolution(m_Size);

	// 모든 타일들을 순회한다.
	for (int i = 0; i < m_TileCountY; i++)
	{
		for (int j = 0; j < m_TileCountX; j++)
		{
			// 타일을 생성하고 초기화
			CTile* Tile = new CTile;

			Tile->Init();

			// 타일은 (0,0)부터 깔리게된다. 따라서 x, y에 하나의 타일 크기를 곱해서 위치를 정한다.
			Vector2 Pos = Vector2((float)j, (float)i) * m_TileSize;

			// 타일의 위치와 크기 그리고 x, y좌표와 1차원 배열에서의 위치 그리고 텍스쳐를 초기화한다.
			Tile->SetTileInfo(Pos, m_TileSize, j, i, i * m_TileCountX + j, m_TileTexture);

			// 타일맵의 타일벡터에 추가한다.
			m_vecTile.push_back(Tile);
		}
	}

	// 모든 타일들을 순회하면서 타일 텍스쳐를 설정한다.
	for (int i = 0; i < m_TileCountY; i++)
	{
		for (int j = 0; j < m_TileCountX; j++)
		{
			m_vecTile[i * m_TileCountX + j]->SetTileTexture(m_TileTexture);
		}
	}
	

	return true;
}

bool CTileMap::SetTileTexture(CTexture* Texture)
{
	// 모든 타일들을 특정 텍스쳐로 설정한다.
	m_TileTexture = Texture;

	for (int i = 0; i < m_TileCountY; i++)
	{
		for (int j = 0; j < m_TileCountX; j++)
		{
			m_vecTile[i * m_TileCountX + j]->SetTileTexture(m_TileTexture);
		}
	}

	return true;
}

bool CTileMap::SetTileTexture(CTexture* Texture, int IndexX, int IndexY)
{
	// 특정위치의 타일의 텍스쳐를 설정한다.
	m_TileTexture = Texture;
	m_vecTile[IndexY * m_TileCountX + IndexX]->SetTileTexture(m_TileTexture);

	return true;
}

void CTileMap::SetTileTexture(const Vector2& Pos, CTexture* Texture)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetTileTexture(Texture);
}

void CTileMap::ChangeTileProperty(const Vector2& Pos, ETileProperty TileProperty)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetTileProperty(TileProperty);
}

void CTileMap::ChangeTileSideCollision(const Vector2& Pos, bool SideCollision)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetSideCollision(SideCollision);
}

void CTileMap::SetTileFrame(const Vector2& Pos, const Vector2& Start, const Vector2& End)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetStartFrame(Start);
	Tile->SetEndFrame(End);
}

CTile* CTileMap::GetTile(const Vector2& Pos)
{
	int IndexX = (int)(Pos.x / m_TileSize.x);
	int IndexY = (int)(Pos.y / m_TileSize.y);

	if (IndexX < 0 || IndexX >= m_TileCountX)
		return nullptr;

	if (IndexY < 0 || IndexY >= m_TileCountY)
		return nullptr;

	return m_vecTile[IndexY * m_TileCountX + IndexX];
}

CTile* CTileMap::GetTile(int Index)
{
	return m_vecTile[Index];
}

CTile* CTileMap::GetTile(int IndexX, int IndexY)
{
	return m_vecTile[IndexY * m_TileCountX + IndexX];
}

int CTileMap::GetTileIndexX(float PosX)
{
	int IndexX = (int)(PosX / m_TileSize.x);

	if (IndexX < 0 || IndexX >= m_TileCountX)
		return -1;

	return IndexX;
}

int CTileMap::GetTileIndexY(float PosY)
{
	int IndexY = (int)(PosY / m_TileSize.y);

	if (IndexY < 0 || IndexY >= m_TileCountY)
		return -1;

	return IndexY;
}

int CTileMap::GetTileIndex(const Vector2& Pos)
{
	int IndexX = GetTileIndexX(Pos.x);
	int IndexY = GetTileIndexY(Pos.y);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_TileCountX + IndexX;
}

int CTileMap::GetOriginTileIndexX(float PosX)
{
	return (int)(PosX / m_TileSize.x);
}

int CTileMap::GetOriginTileIndexY(float PosY)
{
	return (int)(PosY / m_TileSize.y);
}

void CTileMap::TileImageClear()
{
	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecTile[i]->SetTileTexture(nullptr);
	}
}

void CTileMap::Start()
{
	CMapBase::Start();
}

bool CTileMap::Init()
{
	CMapBase::Init();

	return true;
}

void CTileMap::Update(float DeltaTime)
{
	CMapBase::Update(DeltaTime);

	// 만약 타일이 존재한다면
	if (!m_vecTile.empty())
	{
		CCamera* Camera = m_Scene->GetCamera();
		
		// 현재 카메라의 위치와 카메라 해상도를 구한다.
		Vector2 CameraPos = Camera->GetPos();
		Vector2 Resolution = Camera->GetResolution();

		// 좌표의 시작점과 끝점을 구한다. (일종의 정규화라고 생각하자)
		m_StartX = (int)(CameraPos.x / m_TileSize.x);
		m_StartY = (int)(CameraPos.y / m_TileSize.y);
		m_EndX = (int)((CameraPos.x + Resolution.x) / m_TileSize.x);
		m_EndY = (int)((CameraPos.y + Resolution.y) / m_TileSize.y);

		// 시작과 끝 위치가 범위를 벗어날 경우를 대비한다.
		m_StartX = m_StartX < 0 ? 0 : m_StartX;
		m_StartY = m_StartY < 0 ? 0 : m_StartY;
		m_EndX = m_EndX >= m_TileCountX ? m_TileCountX - 1 : m_EndX;
		m_EndY = m_EndY >= m_TileCountY ? m_TileCountY - 1 : m_EndY;

		// 지정된 좌표에서의 타일들만 반복문을 돌린다.
		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->Update(DeltaTime);
			}
		}
	}
}

void CTileMap::PostUpdate(float DeltaTime)
{
	CMapBase::PostUpdate(DeltaTime);

	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->PostUpdate(DeltaTime);
			}
		}
	}
}

void CTileMap::PrevRender()
{
	CMapBase::PrevRender();

	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->PrevRender();
			}
		}
	}
}

void CTileMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);


	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->Render(hDC);
			}
		}
	}
}

void CTileMap::Save(FILE* pFile)
{
	// 먼저 부모 클래스의 기본 속성을 저장한다.
	CMapBase::Save(pFile);

	// 자식 클래스의 세부 사항을 저장한다.
	fwrite(&m_TileCountX, sizeof(int), 1, pFile);
	fwrite(&m_TileCountY, sizeof(int), 1, pFile);
	fwrite(&m_TileSize, sizeof(Vector2), 1, pFile);
	fwrite(&m_StartX, sizeof(int), 1, pFile);
	fwrite(&m_StartY, sizeof(int), 1, pFile);
	fwrite(&m_EndX, sizeof(int), 1, pFile);
	fwrite(&m_EndY, sizeof(int), 1, pFile);

	// 만약 텍스쳐가 존재했다면
	if (m_TileTexture)
	{
		bool Tex = true;
		fwrite(&Tex, sizeof(bool), 1, pFile);

		// 텍스쳐의 저장 함수를 호출한다.
		m_TileTexture->Save(pFile);
	}
	else
	{
		bool Tex = false;
		fwrite(&Tex, sizeof(bool), 1, pFile);
	}

	// 타일의 갯수를 저장한다.
	int TileCount = (int)m_vecTile.size();
	fwrite(&TileCount, sizeof(int), 1, pFile);

	// 모든 타일의 저장함수를 호출한다.
	for (int i = 0; i < TileCount; i++)
	{
		m_vecTile[i]->Save(pFile);
	}
}

void CTileMap::Load(FILE* pFile)
{
	// 먼저 기본 사항을 로드한다.
	// 세이브 순서를 그대로 따라간다.
	CMapBase::Load(pFile);

	fread(&m_TileCountX, sizeof(int), 1, pFile);
	fread(&m_TileCountY, sizeof(int), 1, pFile);
	fread(&m_TileSize, sizeof(Vector2), 1, pFile);
	fread(&m_StartX, sizeof(int), 1, pFile);
	fread(&m_StartY, sizeof(int), 1, pFile);
	fread(&m_EndX, sizeof(int), 1, pFile);
	fread(&m_EndY, sizeof(int), 1, pFile);

	bool Tex = true;
	fread(&Tex, sizeof(bool), 1, pFile);

	// 만약 텍스쳐가 존재한다면
	// 텍스쳐를 불러와야한다.
	if (Tex)
	{
		// 현재 텍스쳐가 없는 상황이다 따라서 텍스쳐를 씬에 로딩해야한다.
		m_TileTexture = CTexture::LoadStatic(pFile, m_Scene);
	}

	// 타일의 수만큼 반복한다.
	int TileCount = 0;
	fread(&TileCount, sizeof(int), 1, pFile);

	// 먼저 기존의 타일을 전부 삭제한다.
	size_t TileSize = m_vecTile.size();

	for (size_t i = 0; i < TileSize; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	// 다시 타일을 만들어 낸다.
	for (int i = 0; i < TileCount; i++)
	{
		CTile* Tile = new CTile;
		Tile->m_Scene = m_Scene;

		m_vecTile.push_back(Tile);

		m_vecTile[i]->Load(pFile);
	}

	m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

	CCamera* Camera = m_Scene->GetCamera();
	Camera->SetWorldResolution(m_Size);
}
