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
	// 현재 카메라의 위치.
	Vector2 m_Pos;
	// 카메라의 크기
	Vector2 m_Resolution;
	// 전체 월드의 크기
	Vector2 m_WorldResolution;
	// 현재 카메라가 바라보고 있는 대상
	CSharedPtr<CGameObject> m_Target;
	// 대상에서 얼마나 오프셋이 있는지.
	Vector2 m_TargetOffset;
	// 타겟을 어디에 고정 시킬건지.
	Vector2 m_TargetPivot;

public:
	Vector2 GetPos() const
	{
		return m_Pos;
	}

	Vector2 GetResolution() const
	{
		return m_Resolution;
	}

	Vector2 GetWorldResolution() const
	{
		return m_WorldResolution;
	}

	Vector2 GetTargetOffset() const
	{
		return m_TargetOffset;
	}

	Vector2 GetTargetPivot() const
	{
		return m_TargetPivot;
	}

public:
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetResolution(const Vector2& Resolution)
	{
		m_Resolution = Resolution;
	}

	void SetResoluton(float x, float y)
	{
		m_Resolution = Vector2(x, y);
	}

	void SetWorldResolution(const Vector2& Resolution)
	{
		m_WorldResolution = Resolution;
	}

	void SetWorldResolution(float x, float y)
	{
		m_WorldResolution = Vector2(x, y);
	}

	void SetTargetOffset(const Vector2& TargetOffset)
	{
		m_TargetOffset = TargetOffset;
	}

	void SetTargetOffset(float x, float y)
	{
		m_TargetOffset = Vector2(x, y);
	}

	void SetTargetPivot(const Vector2& TargetPivot)
	{
		m_TargetPivot = TargetPivot;
	}

	void SetTargetPivot(float x, float y)
	{
		m_TargetPivot = Vector2(x, y);
	}

	void SetTarget(CGameObject* Target)
	{
		m_Target = Target;
	}

	void Move(const Vector2& MovePos)
	{
		m_Pos += MovePos;
	}

	
public:
	bool Init();
	void Update(float DeltaTime);
};

