#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

/// <summary>
/// Ÿ�ϸʿ� �Ҽӵ� �ϳ��� Ÿ��
/// </summary>
class CTile
{
	// Ÿ�ϸ��� Ÿ���� �����Ӱ� ������ �����ؾ��Ѵ�.
	friend class CTileMap;
	
private:
	CTile();
	~CTile();

private:
	class CScene* m_Scene;


	// ���� Ÿ���� ��ġ
	Vector2 m_Pos;
	// Ÿ���� ũ��
	Vector2 m_Size;
	// Ÿ���� X��ǥ
	int m_IndexX;
	// Ÿ���� Y��ǥ
	int m_IndexY;
	// Ÿ���� 1���� �迭������ �ε��� ��
	int m_Index;
	// �ؽ���
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

