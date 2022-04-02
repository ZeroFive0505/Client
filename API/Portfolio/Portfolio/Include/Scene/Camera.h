#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"

class CCamera
{
	friend class CScene;

private:
	CCamera();
	~CCamera();

private:
	Vector2 m_Pos;
	Vector2 m_Resolution;
	Vector2 m_WordResolution;
	CSharedPtr<CGameObject> m_Target;
	Vector2 m_TargetOffset;
	Vector2 m_TargetPivot;


	bool m_InfiniteScroll;

public:
	inline Vector2 GetPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetResolution() const
	{
		return m_Resolution;
	}

	inline Vector2 GetWorldResolution() const
	{
		return m_WordResolution;
	}

	inline Vector2 GetTargetOffset() const
	{
		return m_TargetOffset;
	}

	inline Vector2 GetTargetPivot() const
	{
		return m_TargetPivot;
	}

public:
	inline void SetPos(const Vector2& pos)
	{
		m_Pos = pos;
	}

	inline void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	inline void SetResolution(const Vector2& resolution)
	{
		m_Resolution = resolution;
	}

	inline void SetResolution(float x, float y)
	{
		m_Resolution = Vector2(x, y);
	}

	inline void SetWorldResolution(const Vector2& resolution)
	{
		m_WordResolution = resolution;
	}

	inline void SetWorldResolution(float x, float y)
	{
		m_WordResolution = Vector2(x, y);
	}

	inline void SetTargetOffset(const Vector2& targetOffset)
	{
		m_TargetOffset = targetOffset;
	}

	inline void SetTargetOffset(float x, float y)
	{
		m_TargetOffset = Vector2(x, y);
	}

	inline void SetTargetPivot(const Vector2& targetPivot)
	{
		m_TargetPivot = targetPivot;
	}

	inline void SetTargetPivot(float x, float y)
	{
		m_TargetPivot = Vector2(x, y);
	}

	inline void SetTarget(CGameObject* target)
	{
		m_Target = target;
	}

	inline void SetInfinite(bool val)
	{
		m_InfiniteScroll = val;
	}

	inline void Move(const Vector2& movePos)
	{
		m_Pos += movePos;
	}


public:
	bool Init();
	void Update(float deltaTime);
};

