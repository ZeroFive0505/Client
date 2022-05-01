#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "ConstantBuffer.h"
#include "Mesh2DShader.h"
#include "PosMeshShader.h"
#include "ColliderShader.h"
#include "ColliderPixelShader.h"
#include "WidgetShader.h"
#include "ProgressBarShader.h"
#include "LineMeshShader.h"
#include "NumberShader.h"
#include "ParticleUpdateShader.h"
#include "ParticleRenderShader.h"
#include "TileMapShader.h"
#include "Standard3DShader.h"
#include "AnimationUpdateShader.h"


CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Init()
{
	// ���� �⺻ ���̴��� �����. (Į�� �޽�)
	if (!CreateShader<CColorMeshShader>("ColorMeshShader"))
		return false;

	// �⺻ ���̴� (�ؽ�ó �޽�)
	if (!CreateShader<CMesh2DShader>("Mesh2DShader"))
		return false;

	// ������ ��Ʈ(���̾� ������ �޽�)�� ���� ���̴�
	if (!CreateShader<CPosMeshShader>("PosMeshShader"))
		return false;

	// �浹ü ���� ���̴�
	if (!CreateShader<CColliderShader>("ColliderShader"))
		return false;

	if (!CreateShader<CStandard3DShader>("Standard3DShader"))
		return false;

	if (!CreateShader<CColliderPixelShader>("ColliderPixelShader"))
		return false;

	if (!CreateShader<CWidgetShader>("WidgetShader"))
		return false;

	if (!CreateShader<CProgressBarShader>("ProgressBarShader"))
		return false;

	if (!CreateShader<CNumberShader>("NumberShader"))
		return false;

	// ��ƼŬ�� ����� ���� ���̴�
	if (!CreateShader<CParticleUpdateShader>("ParticleUpdateShader"))
		return false;

	// ��ƼŬ ������ ���� ���̴�
	if (!CreateShader<CParticleRenderShader>("ParticleRenderShader"))
		return false;

	if (!CreateShader<CLineMeshShader>("LineShader"))
		return false;

	if (!CreateShader<CTileMapShader>("TileMapShader"))
		return false;

	if (!CreateShader<CAnimationUpdateShader>("AnimationUpdateShader"))
		return false;


	// ������� ����
	// Ʈ������ ���۴� 0�� ��������
	if (!CreateConstantBuffer("TransformCBuffer", sizeof(sTransformCBuffer), 0, (int)Buffer_Shader_Type::Graphic))
		return false;

	// ���׸����� 1�� ��������
	if (!CreateConstantBuffer("MaterialCBuffer", sizeof(sMaterialCBuffer), 1, (int)Buffer_Shader_Type::Graphic))
		return false;

	// �ִϸ��̼� �¿����� ���� ���۴� 2�� ��������
	if (!CreateConstantBuffer("Standard2DCBuffer", sizeof(sStandard2DCBuffer), 2, (int)Buffer_Shader_Type::Graphic))
		return false;

	if (!CreateConstantBuffer("GlobalCBuffer", sizeof(sGlobalCBuffer), 3, (int)Buffer_Shader_Type::All))
		return false;

	if (!CreateConstantBuffer("PaperBurnCBuffer", sizeof(sPaperBurnCBuffer), 4, (int)Buffer_Shader_Type::Pixel))
		return false;

	// �ִϸ��̼� UV��ǥ�� �����ϱ����� ������ 10�� �������� �̿�
	if (!CreateConstantBuffer("Animation2DCBuffer", sizeof(sAnimation2DCBuffer), 10, (int)Buffer_Shader_Type::Graphic))
		return false;

	// �浹ü ���� �����ϱ� ���� ������� 11�� �������� �̿�
	if (!CreateConstantBuffer("ColliderCBuffer", sizeof(sColliderCBuffer), 11, (int)Buffer_Shader_Type::Graphic))
		return false;

	if (!CreateConstantBuffer("WidgetCBuffer", sizeof(sWidgetCBuffer), 11, (int)Buffer_Shader_Type::Graphic))
		return false;

	if (!CreateConstantBuffer("ParticleCBuffer", sizeof(sParticleCBuffer), 11, (int)Buffer_Shader_Type::Compute))
		return false;

	if (!CreateConstantBuffer("ProgressBarCBuffer", sizeof(sProgressBarCBuffer), 12, (int)Buffer_Shader_Type::Graphic))
		return false;

	if (!CreateConstantBuffer("TileMapCBuffer", sizeof(sTileMapCBuffer), 11, (int)Buffer_Shader_Type::Graphic))
		return false;

	if (!CreateConstantBuffer("AnimationCBuffer", sizeof(sAnimationCBuffer), 11, (int)Buffer_Shader_Type::Compute))
		return false;

	if (!CreateConstantBuffer("LightCBuffer", sizeof(sLightBuffer), 5, (int)Buffer_Shader_Type::Pixel))
		return false;


	return true;
}

CShader* CShaderManager::FindShader(const std::string& name)
{
	auto iter = m_mapShader.find(name);

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& name)
{
	auto iter = m_mapShader.find(name);

	if (iter != m_mapShader.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapShader.erase(iter);
	}
}

bool CShaderManager::CreateConstantBuffer(const std::string& name, int size, int registerNum, int constantBufferShaderType)
{
	CConstantBuffer* buffer = FindConstantBuffer(name);

	if (buffer)
		return true;

	buffer = new CConstantBuffer;

	buffer->SetName(name);

	if (!buffer->Init(size, registerNum, constantBufferShaderType))
	{
		SAFE_RELEASE(buffer);
		return false;
	}

	m_mapConstantBuffer.insert(std::make_pair(name, buffer));

	return true;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& name)
{
	auto iter = m_mapConstantBuffer.find(name);

	if (iter == m_mapConstantBuffer.end())
		return nullptr;

	return iter->second;
}
