#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

/// <summary>
/// 타일맵에 소속된 하나의 타일
/// </summary>
class CTile
{
	// 타일맵은 타일을 자유롭게 생성이 가능해야한다.
	friend class CTileMap;
	
private:
	CTile();
	~CTile();

private:
	class CScene* m_Scene;


	// 현재 타일의 위치
	Vector2 m_Pos;
	// 타일의 크기
	Vector2 m_Size;
	// 타일의 X좌표
	int m_IndexX;
	// 타일의 Y좌표
	int m_IndexY;
	// 타일의 1차원 배열에서의 인덱스 값
	int m_Index;
	// 텍스쳐
	CSharedPtr<CTexture> m_Texture;
	ETileProperty m_Property;
	Vector2 m_StartFrame;
	Vector2 m_EndFrame;
	bool m_SideCollision;

public:
	void SetSideCollision(bool SideCollision)
	{
		m_SideCollision = SideCollision;
	}

	void SetStartFrame(const Vector2& Frame)
	{
		m_StartFrame = Frame;
	}

	void SetEndFrame(const Vector2& Frame)
	{
		m_EndFrame = Frame;
	}

public:
	bool GetSideCollision() const
	{
		return m_SideCollision;
	}

	ETileProperty GetTileProperty() const
	{
		return m_Property;
	}

	Vector2 GetPos() const
	{
		return m_Pos;
	}

	Vector2 GetSize() const
	{
		return m_Size;
	}

public:
	void SetTileInfo(const Vector2& Pos, const Vector2& Size,
		int IndexX, int IndexY, int Index, CTexture* Texture)
	{
		m_Pos = Pos;
		m_Size = Size;
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
		m_Texture = Texture;
	}

	void SetTileTexture(CTexture* Texture)
	{
		m_Texture = Texture;
	}

	void SetTileProperty(ETileProperty Property)
	{
		m_Property = Property;
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void PrevRender();
	void Render(HDC hDC);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

