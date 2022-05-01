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
	// 가장 기본 쉐이더를 만든다. (칼라 메쉬)
	if (!CreateShader<CColorMeshShader>("ColorMeshShader"))
		return false;

	// 기본 쉐이더 (텍스처 메쉬)
	if (!CreateShader<CMesh2DShader>("Mesh2DShader"))
		return false;

	// 프레임 렉트(와이어 프레임 메쉬)를 위한 쉐이더
	if (!CreateShader<CPosMeshShader>("PosMeshShader"))
		return false;

	// 충돌체 전용 쉐이더
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

	// 파티클의 계산을 위한 쉐이더
	if (!CreateShader<CParticleUpdateShader>("ParticleUpdateShader"))
		return false;

	// 파티클 렌더를 위한 쉐이더
	if (!CreateShader<CParticleRenderShader>("ParticleRenderShader"))
		return false;

	if (!CreateShader<CLineMeshShader>("LineShader"))
		return false;

	if (!CreateShader<CTileMapShader>("TileMapShader"))
		return false;

	if (!CreateShader<CAnimationUpdateShader>("AnimationUpdateShader"))
		return false;


	// 상수버퍼 생성
	// 트랜스폼 버퍼는 0번 레지스터
	if (!CreateConstantBuffer("TransformCBuffer", sizeof(sTransformCBuffer), 0, (int)Buffer_Shader_Type::Graphic))
		return false;

	// 마테리얼은 1번 레지스터
	if (!CreateConstantBuffer("MaterialCBuffer", sizeof(sMaterialCBuffer), 1, (int)Buffer_Shader_Type::Graphic))
		return false;

	// 애니메이션 온오프를 위한 버퍼는 2번 레지스터
	if (!CreateConstantBuffer("Standard2DCBuffer", sizeof(sStandard2DCBuffer), 2, (int)Buffer_Shader_Type::Graphic))
		return false;

	if (!CreateConstantBuffer("GlobalCBuffer", sizeof(sGlobalCBuffer), 3, (int)Buffer_Shader_Type::All))
		return false;

	if (!CreateConstantBuffer("PaperBurnCBuffer", sizeof(sPaperBurnCBuffer), 4, (int)Buffer_Shader_Type::Pixel))
		return false;

	// 애니메이션 UV좌표를 전달하기위한 버퍼은 10번 레지스터 이용
	if (!CreateConstantBuffer("Animation2DCBuffer", sizeof(sAnimation2DCBuffer), 10, (int)Buffer_Shader_Type::Graphic))
		return false;

	// 충돌체 색을 지정하기 위한 상수버퍼 11번 레지스터 이용
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
