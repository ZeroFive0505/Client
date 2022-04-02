#pragma once

#include "GameObject/GameObject.h"

class CDragObject :
	public CGameObject
{
	friend class CScene;

protected:
	CDragObject();
	CDragObject(const CDragObject& obj);
	virtual ~CDragObject();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	Vector2 m_StartPos;
	Vector2 m_EndPos;

public:
	inline Vector2 GetStartPos() const
	{
		return m_StartPos;
	}

	inline Vector2 GetEndPos() const
	{
		return m_EndPos;
	}

public:
	void SetStartPos(const Vector2& pos);
	void SetEndPos(const Vector2& pos);

	void SetStartPosX(float x);
	void SetStartPosY(float y);

	void SetEndPosX(float x);
	void SetEndPosY(float y);

public:
	inline bool IsPointInStart(const Vector2& mousePoint)
	{
		Vector2 minOffsetBox = m_StartPos - 2.0f;;
		Vector2 maxOffsetBox = m_StartPos + 2.0f;

		if (minOffsetBox.x <= mousePoint.x && mousePoint.x <= maxOffsetBox.x &&
			minOffsetBox.y <= mousePoint.y && mousePoint.y <= maxOffsetBox.y)
			return true;
		else
			return false;
	}

	inline bool IsPointInEnd(const Vector2& mousePoint)
	{
		Vector2 minOffsetBox = m_EndPos - 2.0f;;
		Vector2 maxOffsetBox = m_EndPos + 2.0f;

		if (minOffsetBox.x <= mousePoint.x && mousePoint.x <= maxOffsetBox.x &&
			minOffsetBox.y <= mousePoint.y && mousePoint.y <= maxOffsetBox.y)
			return true;
		else
			return false;
	}


public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CDragObject* Clone();
};