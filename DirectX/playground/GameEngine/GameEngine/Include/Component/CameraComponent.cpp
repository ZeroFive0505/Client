#include "CameraComponent.h"
#include "../Engine.h"
#include "../Device.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();
	m_Render = false;
	m_Shake = false;
	m_TraumaAmount = 20.0f;
	m_RecoverySpeed = 20.0f;
	m_CameraType = Camera_Type::Camera2D;
	m_ViewAngle = 90.0f;
	m_Distance = 1000.0f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& com) :
	CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_matView = com.m_matView;
	m_matProj = com.m_matProj;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_RS = com.m_RS;
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::CreateProjectionMatrix()
{
	switch (m_CameraType)
	{
	case Camera_Type::Camera2D:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.0f, (float)m_RS.width, 0.0f, (float)m_RS.height, 0.0f, 1000.0f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), m_RS.width / (float)m_RS.height, 0.1f, m_Distance);
		break;
	case Camera_Type::CameraUI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.0f, (float)m_RS.width, 0.0f, (float)m_RS.height, 0.0f, 1000.0f);
		break;
	}
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();
	
	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	m_RS = CDevice::GetInst()->GetResolution();

	return true;
}

void CCameraComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	/*
	ī�޶� ��������� ��ȯ�� �Ǹ� ī�޶� �����ϴ� x, y, z ���� ������ ��� ��ġ�ϰ� �ȴ�.
	��, ���� x : (1, 0, 0) y: (0, 1, 0) z : (0, 0, 1) ���� ������ �ȴٴ� ���̴�.
	���� ��� : ����� �����ϴ� 3���� ���� ��� ���ο� ���� ������ �ϴ� ����� ���Ѵ�.
	1 0 0
	0 1 0 
	0 0 1

	��������� ��ġ ��İ� ������� ����.

	ī�޶��� x�� * ����� = 1, 0, 0
	ī�޶��� y�� * ����� = 0, 1, 0
	ī�޶��� z�� * ����� = 0, 0, 1

	x, y, z   0 0 0    1 0 0
	x, y, z * 0 0 0  = 0 1 0
	x, y, z   0 0 0    0 0 1

	���� ȸ�� ��� (��ġ ���)
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1

	���� �̵� ��� (ī�޶� �̵��� ��ŭ �ٽ� ���ش�.)
	1  0  0  0
	0  1  0  0 
	0  0  1  0
	-x -y -z 1

	1  0  0  0   Xx Yx Zx 0
	0  1  0  0 * Xy Yy Zy 0
	0  0  1  0   Xz Yz Zz 0
	-x -y -z 1   0  0  0  1

	�ֵ� �� ���
	Xx    Yx   Zx  0
	Xy    Yy   Zy  0
	Xz    Yz   Zz  0
	-X.P -Y.P -Z.P 1
	*/
}

void CCameraComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);

	m_matView.Identity();

	/*
	i�� 0�϶�
	Xx Xy Xz 0
	0  1  0  0
	0  0  1  0
	0  0  0  1

	i�� 1�϶�
	Xx Xy Xz 0
	Yx Yy Yz 0
	0  0  1  0
	0  0  0  1

	i�� 2�϶�
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	*/

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &axis, sizeof(Vector3));
	}


	/*
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1

	��ġ�ϸ�
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1
	*/

	m_matView.Transpose();

	Vector3 pos = GetWorldPos() * -1.0f;

	if (m_Shake)
	{
		float seed = (float)rand() / (float)RAND_MAX;

		int x = int(seed * CEngine::GetInst()->GetPerlinNoise2DWidth());
		int y = int(seed * CEngine::GetInst()->GetPerlinNoise2DHeight());

		float seed1 = CEngine::GetInst()->GetNoise2D(x, y);
		float seed2 = CEngine::GetInst()->GetNoise2D(x * 3, y * 5);

		pos.x = pos.x + (seed1 * 2.0f - 1.0f) * m_Trauma;
		pos.y = pos.y + (seed2 * 2.0f - 1.0f) * m_Trauma;

		m_Trauma = std::fmax(0.0f, m_Trauma - m_RecoverySpeed * deltaTime);
		if (m_Trauma <= 0.0f)
			m_Shake = false;
	}


	/*
	Xx    Yx   Zx  0
	Xy    Yy   Zy  0
	Xz    Yz   Zz  0
	-X.P -Y.P -Z.P 1
	*/

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 axis = GetWorldAxis((AXIS)i);

		m_matView[3][i] = pos.Dot(axis);
	}
}

void CCameraComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* pFile)
{
	fwrite(&m_CameraType, sizeof(Camera_Type), 1, pFile);
	fwrite(&m_matView, sizeof(Matrix), 1, pFile);
	fwrite(&m_matProj, sizeof(Matrix), 1, pFile);
	fwrite(&m_ViewAngle, sizeof(float), 1, pFile);
	fwrite(&m_Distance, sizeof(float), 1, pFile);
	fwrite(&m_RS, sizeof(sResolution), 1, pFile);

	CSceneComponent::Save(pFile);
}

void CCameraComponent::Load(FILE* pFile)
{
	fread(&m_CameraType, sizeof(Camera_Type), 1, pFile);
	fread(&m_matView, sizeof(Matrix), 1, pFile);
	fread(&m_matProj, sizeof(Matrix), 1, pFile);
	fread(&m_ViewAngle, sizeof(float), 1, pFile);
	fread(&m_Distance, sizeof(float), 1, pFile);
	fread(&m_RS, sizeof(sResolution), 1, pFile);

	CSceneComponent::Load(pFile);
}
