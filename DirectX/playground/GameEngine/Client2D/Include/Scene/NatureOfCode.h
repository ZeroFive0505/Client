#pragma once

#include "Scene/SceneMode.h"

class CNatureOfCode :
	public CSceneMode
{
public:
	CNatureOfCode();
	~CNatureOfCode();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	void ApplyGravity(float deltaTime);
	void ApplyFriction(float deltaTime);
	void ApplyAttract(float deltaTime);
	void ApplyRotation(float deltaTime);

private:
	void RadiusUp(float deltaTime);
	void RadiusDown(float deltaTime);

private:
	void SpawnVehicle(float deltaTime);

private:
	class CAttractor* m_Attractor;
	std::vector<class CMover*> m_vecMover;
	class CLiquid* m_Liquid;
	std::vector<class CPendulum*> m_vecPendulum;
	std::vector<class CLine*> m_vecLine;
	std::vector<class CVehicle*> m_vecVehicle;
	class CFlowField* m_Field;
	class CPath* m_Path;

	float m_FrictionConst;

	float m_ElapsedTime;

	float m_Radius;

	float m_G;
};

