#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"
#include "Tile.h"

class CTileMapComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CTileMapComponent();
	CTileMapComponent(const CTileMapComponent& com);
	virtual ~CTileMapComponent();

protected:
	CSharedPtr<CSpriteMesh> m_Mesh;
	CSharedPtr<CMesh> m_WireMesh;
	CSharedPtr<CMaterial> m_BackMaterial;
	CSharedPtr<CMaterial> m_TileMaterial;
	std::vector<CTile*> m_vecTile;
	std::vector<sTileInfo> m_vecTileInfo;
	class CTileMapConstantBuffer* m_TileCBuffer;
	class CStructuredBuffer* m_TileInfoBuffer;
	Tile_Shape m_TileShape;
	int m_CountX;
	int m_CountY;
	int m_Count;
	int m_RenderCount;
	float m_ScaleX;
	float m_ScaleY;
	float m_OffsetX;
	float m_OffsetY;
	Vector3 m_TileSize;
	Vector4 m_TileColor[(int)Tile_Type::End];
	bool m_EditMode;
	bool m_SortDisable;

public:
	inline void EnableEditMode(bool mode)
	{
		m_EditMode = mode;
	}

	inline void SetSortDisable(bool disable)
	{
		m_SortDisable = disable;
	}

public:
	inline CMaterial* GetBackMaterial() const
	{
		return m_BackMaterial;
	}

	inline CMaterial* GetTileMaterial() const
	{
		return m_TileMaterial;
	}

	inline Tile_Shape GetTileShape() const
	{
		return m_TileShape;
	}

	inline int GetTileCountX() const
	{
		return m_CountX;
	}

	inline int GetTileCountY() const
	{
		return m_CountY;
	}

	inline const Vector3& GetTileSize() const
	{
		return m_TileSize;
	}

	inline float GetTileOffsetX() const
	{
		return m_OffsetX;
	}

	inline float GetTileOffsetY() const
	{
		return m_OffsetY;
	}

public:
	void SetBackMaterial(CMaterial* material);
	void SetTileMaterial(CMaterial* material);

	inline void SetScaleX(float x)
	{
		m_ScaleX = x;
	}

	inline void SetScaleY(float y)
	{
		m_ScaleY = y;
	}

	inline void SetOffsetX(float x)
	{
		size_t size = m_vecTile.size();

		if ((int)size == 0)
			return;

		m_OffsetX = x;

		for (size_t i = 0; i < size; i++)
		{
			m_vecTile[i]->SetOffsetX(m_OffsetX);
		}
	}

	inline void SetOffsetY(float y)
	{
		size_t size = m_vecTile.size();

		if ((int)size == 0)
			return;

		m_OffsetY = y;

		for (size_t i = 0; i < size; i++)
		{
			m_vecTile[i]->SetOffsetY(m_OffsetY);
		}
	}

public:
	void SetBackBaseColor(const Vector4& color);
	void SetBackBaseColor(float r, float g, float b, float a);
	void SetBackRenderState(class CRenderState* state);
	void SetBackRenderState(const std::string& name);
	void SetBackTransparency(bool enable);
	void SetBackOpacity(float opacity);
	void AddBackOpacity(float opacity);

public:
	void AddBackTexture(int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void AddBackTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddBackTextureFullPath(int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath);
	void AddBackTexture(int registerNum, int shaderType, const std::string& name, 
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);


	void SetBackTexture(int index, int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void SetBackTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetBackTextureFullPath(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath);
	void SetBackTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

public:
	void CreateTile(Tile_Shape shape, int countX, int countY, const Vector3& size);
	void SetTileDefaultFrame(const Vector2& start, const Vector2& end);
	void SetTileDefaultFrame(float startX, float startY, float endX, float endY);
	void SetTileFrame(int indexX, int indexY, float startX, float startY, float endX, float endY);
	void SetTileFrame(int index, float startX, float startY, float endX, float endY);
	void SetTileFrame(const Vector3& pos, float startX, float startY, float endX, float endY);
	void SetTileOpacity(int indexX, int indexY, float opacity);
	void SetTileOpacity(int index, float opacity);
	void SetTileOpacity(const Vector3& pos, float opacity);
	void SetTileColor(Tile_Type type, float r, float g, float b, float a);
	void SetTileColor(Tile_Type type, const Vector4& color);

private:
	int GetTileRenderIndexX(const Vector3& pos);
	int GetTileRenderIndexY(const Vector3& pos);

public:
	int GetTileIndexX(const Vector3& pos);
	int GetTileIndexY(const Vector3& pos);
	int GetTileIndex(const Vector3& pos);
	CTile* GetTile(const Vector3& pos);
	CTile* GetTile(int x, int y);
	CTile* GetTile(int index);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTileMapComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void SetWorldInfo();
};

