#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"
#include "Tile.h"


/// <summary>
/// MapBase�� ��ӹ޾� ���� Ÿ�ϸ� ����� ��
/// </summary>
class CTileMap :
	public CMapBase
{
	// ���� ���ó� �����Ӱ� ������ �����ؾ��Ѵ�.
	friend class CScene;

protected:
	CTileMap();
	virtual ~CTileMap();

protected:
	// Ÿ�ϸʿ� �ִ� Ÿ�ϵ�
	std::vector<CTile*> m_vecTile;
	// Ÿ���� �ؽ���
	CSharedPtr<CTexture> m_TileTexture;
	// Ÿ���� ���� ����
	int m_TileCountX;
	// Ÿ���� ���� ����
	int m_TileCountY;
	// Ÿ���� ũ��
	Vector2 m_TileSize;

	/// <summary>
	/// �ݺ����� ȿ�������� �������ؼ� ���� ��ǥ�� �� ��ǥ�� ���Ѵ�.
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
	// Ÿ���� ����
	bool CreateTile(int CountX, int CountY, const Vector2& TileSize);
	// Ÿ���� �ؽ��ĸ� ����
	bool SetTileTexture(CTexture* Texture);
	// �ش� ��ǥ�� Ÿ���� �ؽ��ĸ� ����
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

