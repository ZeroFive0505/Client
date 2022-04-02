#pragma once

#include "ObjectComponent.h"

class CNavAgent :
	public CObjectComponent
{
	friend class CGameObject;

protected:
	CNavAgent();
	CNavAgent(const CNavAgent& com);
	virtual ~CNavAgent();

private:
	CSharedPtr<class CSceneComponent> m_UpdateComponent;
	std::list<Vector3> m_Path;
	float m_MoveSpeed;
	bool m_IsMoving;
	bool m_Interrupted;

public:
	inline void SetUpdateComponent(class CSceneComponent* updateComponent)
	{
		m_UpdateComponent = updateComponent;
	}

	bool Move(const Vector3& pos);

	inline void SetMoveSpeed(float speed)
	{
		m_MoveSpeed = speed;
	}
	
	inline void SetInterrupted(bool interrupt)
	{
		m_Interrupted = interrupt;
	}

	inline bool IsPathEmpty() const
	{
		return (int)m_Path.size() == 0;
	}

	inline bool IsMoving() const
	{
		return m_IsMoving;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CNavAgent* Clone();

private:
	void PathResult(const std::list<Vector3>& path);
};

