#pragma once

#include "Engine.h"

class CFlowField
{
public:
	CFlowField();
	CFlowField(const CFlowField& obj);
	virtual  ~CFlowField();

private:
	Vector2* m_Field;
	int m_Cols;
	int m_Rows;
	int m_Resolution;

public:
	Vector2 LookUp(const Vector2& location);

	void MakeRandomField();
	void MakePerlinField();

public:
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
};

