#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"
#include "Tile.h"


/// <summary>
/// MapBase를 상속받아 만든 타일맵 기반의 맵
/// </summary>
class CTileMap :
	public CMapBase
{
	// 씬은 역시나 자유롭게 생성이 가능해야한다.
	friend class CScene;

protected:
	CTileMap();
	virtual ~CTileMap();

protected:
	// 타일맵에 있는 타일들
	std::vector<CTile*> m_vecTile;
	// 타일의 텍스쳐
	CSharedPtr<CTexture> m_TileTexture;
	// 타일의 가로 개수
	int m_TileCountX;
	// 타일의 새로 개수
	int m_TileCountY;
	// 타일의 크기
	Vector2 m_TileSize;

	/// <summary>
	/// 반복문을 효과적으로 돌기위해서 시작 좌표와 끝 좌표를 구한다.
	/// </summary>
	int m_StartX;
	int m_StartY;
	int m_EndX;
	int m_EndY;

public:
	int GetTileCountX() const
	{
		return m_TileCountX;
	}

	int GetTileCountY() const
	{
		return m_TileCountY;
	}

public:
	// 타일을 생성
	bool CreateTile(int CountX, int CountY, const Vector2& TileSize);
	// 타일의 텍스쳐를 선택
	bool SetTileTexture(CTexture* Texture);
	// 해당 좌표의 타일의 텍스쳐를 선택
	bool SetTileTexture(CTexture* Texture, int IndexX, int IndexY);
	void SetTileTexture(const Vector2& Pos, CTexture* Texture);
	void ChangeTileProperty(const Vector2& Pos, ETileProperty TileProperty);
	void ChangeTileSideCollision(const Vector2& Pos, bool SideCollision);
	void SetTileFrame(const Vector2& Pos, const Vector2& Start, const Vector2& End);
	CTile* GetTile(const Vector2& Pos);
	CTile* GetTile(int Index);
	CTile* GetTile(int IndexX, int IndexY);
	int GetTileIndexX(float PosX);
	int GetTileIndexY(float PosY);
	int GetTileIndex(const Vector2& Pos);
	int GetOriginTileIndexX(float PosX);
	int GetOriginTileIndexY(float PosY);
	void TileImageClear();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

