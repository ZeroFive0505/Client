#pragma once

#include "../GameInfo.h"
#include "../Map/MapInfo.h"
#include "../Resource/Texture.h"

class CTile
{
	friend class CTileMap;

private:
	CTile();
	~CTile();

private:
	class CScene* m_Scene;

	Vector2 m_Pos;

	Vector2 m_Size;

	int m_IndexX;

	int m_IndexY;

	int m_Index;

	ETileProperty m_Property;

public:
	inline ETileProperty GetTileProperty() const
	{
		return m_Property;
	}

	inline Vector2 GetPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetSize()
	{
		return m_Size;
	}

	inline int GetIndexX() const
	{
		return m_IndexX;
	}

	inline int GetIndexY() const
	{
		return m_IndexY;
	}

	inline int GetIndex() const
	{
		return m_Index;
	}

	inline void SetTileProperty(ETileProperty tileProperty)
	{
		m_Property = tileProperty;
	}

public:
	inline void SetTileInfo(const Vector2& pos, const Vector2& size,
		int indexX, int indexY, int index)
	{
		m_Pos = pos;
		m_Size = size;
		m_IndexX = indexX;
		m_IndexY = indexY;
		m_Index = index;
	}

public:
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Render(HDC hDC);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

