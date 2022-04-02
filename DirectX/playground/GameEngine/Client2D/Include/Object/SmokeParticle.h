#pragma once

#include "GameObject/GameObject.h"


class CSmokeParticle :
	public CGameObject
{
	friend class CScene;

protected:
	CSmokeParticle();
	CSmokeParticle(const CSmokeParticle& obj);
	virtual ~CSmokeParticle();

private:
	CSharedPtr<class CParticleComponent> m_Particle;

public:
	virtual bool Init();
	void SetMoveDir(float x, float y, float z);
	void SetMoveDir(const Vector3& dir);
	void SetMoveAngle(float x, float y, float z);
	void SetMoveAngle(const Vector3& angle);
	virtual CSmokeParticle* Clone();
};

