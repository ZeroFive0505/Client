#include "FlowField.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"


CFlowField::CFlowField()
{
	m_Resolution = 1;

	sResolution rs = CEngine::GetInst()->GetResolution();

	m_Cols = (int)rs.width / m_Resolution;
	m_Rows = (int)rs.height / m_Resolution;

	m_Field = new Vector2[m_Rows * m_Cols];

	MakePerlinField();
}

CFlowField::CFlowField(const CFlowField& obj)
{
}

CFlowField::~CFlowField()
{
	SAFE_DELETE_ARRAY(m_Field);
}

Vector2 CFlowField::LookUp(const Vector2& location)
{
	int col = (int)location.x / m_Resolution;
	int row = (int)location.y / m_Resolution;

	col = max(0, min(col, m_Cols - 1));
	row = max(0, min(row, m_Rows - 1));

	return m_Field[row * m_Rows + col];
}

void CFlowField::MakeRandomField()
{
	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;

	for (int i = 0; i < m_Rows; i++)
	{
		for (int j = 0; j < m_Cols; j++)
		{
			Vector2 dir = Vector2(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f);

			m_Field[i * m_Rows + j] = dir;
		}
	}
}

void CFlowField::MakePerlinField()
{
	int perlinSize = CEngine::GetInst()->GetPerlinNoise2DWidth();

	for (int i = 0; i < m_Rows; i++)
	{
		int indexY = 0;
		for (int j = 0; j < m_Cols; j++)
		{
			float noise = CEngine::GetInst()->GetNoise2D(i % perlinSize + 200, j % perlinSize + 500);
			float theta = MapValue(noise, 0.0f, 1.0f, 0.0f, 2.0f * PI);
			m_Field[i * m_Rows + j] = Vector2(cos(theta), sin(theta));
		}
			
	}
}

void CFlowField::Update(float deltaTime)
{
}

void CFlowField::PostUpdate(float deltaTime)
{
}
