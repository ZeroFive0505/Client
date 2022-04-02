#pragma once

#include "../Engine.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Component/ColliderBox2D.h"

class CTile
{
	friend class CTileMapComponent;

private:
	CTile();
	CTile(const CTile& tile);
	~CTile();

private:
	class CTileMapComponent* m_Owner;
	Tile_Shape m_Shape;
	Tile_Type m_TileType;
	Vector3 m_Pos; // ¡¬ «œ¥‹ ±‚¡ÿ
	Vector3 m_Size;
	Vector3 m_Center;
	Vector3 m_Offset;
	Matrix m_matWorld;
	CAnimationSequence2DInstance* m_AnimInstance;
	int m_IndexX;
	int m_IndexY;
	int m_Index;
	Vector2 m_FrameStart;
	Vector2 m_FrameEnd;
	float m_Opacity;

public:
	inline float GetOpacity() const
	{
		return m_Opacity;
	}

	inline Tile_Shape GetShape() const
	{
		return m_Shape;
	}

	inline Tile_Type GetTileType() const
	{
		return m_TileType;
	}

	inline const Vector3& GetSize() const
	{
		return m_Size;
	}

	inline const Vector3& GetCenter() const
	{
		return m_Center;
	}

	inline const Vector3& GetPos() const
	{
		return m_Pos;
	}

	inline const Matrix& GetWorldMatrix() const
	{
		return m_matWorld;
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

	inline const Vector2& GetFrameStart() const
	{
		return m_FrameStart;
	}

	inline const Vector2& GetFrameEnd() const
	{
		return m_FrameEnd;
	}

	inline bool GetRender() const
	{
		return m_FrameStart.x != 0.0f || m_FrameStart.y != 0.0f || m_FrameEnd.x != 0.0f || m_FrameEnd.y != 0.0f;
	}

	inline Vector3 GetOffset() const
	{
		return m_Offset;
	}

public:
	inline void SetOpacity(float opacity)
	{
		m_Opacity = opacity;
	}

	inline void SetFrameStart(const Vector2& frame)
	{
		m_FrameStart = frame;
	}

	inline void SetFrameStart(float x, float y)
	{
		m_FrameStart = Vector2(x, y);
	}

	inline void SetFrameEnd(const Vector2& frame)
	{
		m_FrameEnd = frame;
	}

	inline void SetFrameEnd(float x, float y)
	{
		m_FrameEnd = Vector2(x, y);
	}

	inline void SetIndex(int idxX, int idxY, int idx)
	{
		m_IndexX = idxX;
		m_IndexY = idxY;
		m_Index = idx;
	}

	inline void SetShape(Tile_Shape shape)
	{
		m_Shape = shape;
	}

	inline void SetTileType(Tile_Type type)
	{
		m_TileType = type;
	}

	inline void SetPos(const Vector3& pos)
	{
		m_Pos = pos;
	}

	inline void SetSize(const Vector3& size)
	{
		m_Size = size;
	}

	inline void SetSize(float x, float y, float z)
	{
		m_Size = Vector3(x, y, z);
	}

	inline void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	inline void SetOffset(const Vector3& offset)
	{
		m_Offset = offset;
	}

	inline void SetOffsetX(float x)
	{
		m_Offset.x = x;
	}

	inline void SetOffsetY(float y)
	{
		m_Offset.y = y;
	}

	void AddAnimation(const std::string& sequenceName, const std::string& name, bool loop = true, float playTime = 1.0f,
		float playScale = 1.0f, bool reverse = false);
	void AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name, bool loop = true, float playTime = 1.0f,
		float playScale = 1.0f, bool reverse = false);
	void SetPlayTime(const std::string& name, float playTime);
	void SetPlayScale(const std::string& name, float playScale);
	void SetReverse(const std::string& name, bool reverse);
	void SetLoop(const std::string& name, bool loop);
	void SetCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	bool CheckCurrentAnimation(const std::string& name);
	
public:
	void Start();
	void Update(float deltaTime, bool sortDisable);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

