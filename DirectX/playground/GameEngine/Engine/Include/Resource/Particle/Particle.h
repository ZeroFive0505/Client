#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"
#include "../Shader/ParticleUpdateShader.h"
#include "../Shader/ParticleConstantBuffer.h"

class CParticle :
	public CRef
{
	friend class CParticleManager;

private:
	CParticle();
	CParticle(const CParticle& particle);
	~CParticle();

private:
	class CScene* m_Scene;
	// 구조화 버퍼로 컴퓨트 쉐이더에서 계산된 값이 여기에 저장된다.
	std::vector<class CStructuredBuffer*> m_vecStructuredBuffer;
	CSharedPtr<CMaterial> m_Material;
	// 파티클의 위치 계산을 위한 쉐이더
	CSharedPtr<CParticleUpdateShader> m_UpdateShader;
	sParticleInfo m_Info;
	sParticleInfoShared m_InfoShared;
	CParticleConstantBuffer* m_CBuffer;
	float m_SpawnTime;
	bool m_2D;
	int m_SpawnCountMax;

public:
	inline CMaterial* CloneMaterial() const
	{
		return m_Material->Clone();
	}

	inline CParticleUpdateShader* GetUpdateShader() const
	{
		return m_UpdateShader;
	}

	inline CParticleUpdateShader* CloneUpdateShader() const
	{
		return m_UpdateShader->Clone();
	}

	inline CParticleConstantBuffer* CloneConstantBuffer() const
	{
		return m_CBuffer->Clone();
	}

	inline float GetSpawnTime() const
	{
		return m_SpawnTime;
	}

public:
	bool Init();

public:
	void AddStructuredBuffer(const std::string& name, unsigned int size, unsigned int count,
		int registerNum, bool isDynamic = false,
		int structuredBufferShaderType = (int)Buffer_Shader_Type::Compute);

	// 구조화 버퍼의 최대 크기가 바뀔때마다 이 함수를 호출
	bool ResizeBuffer(const std::string& name, unsigned int size, unsigned int count,
		int registerNum, bool isDynamic = false,
		int structuredBufferShaderType = (int)Buffer_Shader_Type::Compute);

	// 구조화 버퍼를 복사해준다.
	void CloneStructedBuffer(std::vector<CStructuredBuffer*>& vecBuffer);
	
public:
	inline CParticleConstantBuffer* GetCBuffer() const
	{
		return m_CBuffer;
	}

	inline int GetSpawnCountMax() const
	{
		return m_SpawnCountMax;
	}

public:
	inline void SetMaterial(CMaterial* material)
	{
		m_Material = material;
	}

	inline void SetSpawnTime(float spawnTime)
	{
		m_SpawnTime = spawnTime;
	}

	inline void SetSpawnEnable(unsigned int enable)
	{
		m_CBuffer->SetSpawnEnable(enable);
	}

	inline void SetStartMin(const Vector3& startMin)
	{
		m_CBuffer->SetStartMin(startMin);
	}

	inline void SetStartMax(const Vector3& startMax)
	{
		m_CBuffer->SetStartMax(startMax);
	}

	void SetSpawnCountMax(unsigned int count);

	inline void SetScaleMin(const Vector3& scaleMin)
	{
		m_CBuffer->SetScaleMin(scaleMin);
	}

	inline void SetScaleMax(const Vector3& scaleMax)
	{
		m_CBuffer->SetScaleMax(scaleMax);
	}

	inline void SetLifeTimeMin(float Min)
	{
		m_CBuffer->SetLifeTimeMin(Min);
	}

	inline void SetLifeTimeMax(float Max)
	{
		m_CBuffer->SetLifeTimeMax(Max);
	}

	inline void SetColorMin(const Vector4& colorMin)
	{
		m_CBuffer->SetColorMin(colorMin);
	}

	inline void SetColorMax(const Vector4& colorMax)
	{
		m_CBuffer->SetColorMax(colorMax);
	}

	inline void SetSpeedMin(float speedMin)
	{
		m_CBuffer->SetSpeedMin(speedMin);
	}

	inline void SetSpeedMax(float speedMax)
	{
		m_CBuffer->SetSpeedMax(speedMax);
	}

	inline void SetMove(bool isMoving)
	{
		m_CBuffer->SetMove(isMoving);
	}

	inline void SetGravity(bool gravityEnable)
	{
		m_CBuffer->SetGravity(gravityEnable);
	}

	inline void SetMoveDir(const Vector3& moveDir)
	{
		m_CBuffer->SetMoveDir(moveDir);
	}

	inline void Set2D(bool is2D)
	{
		m_2D = is2D;
		m_CBuffer->Set2D(is2D);
	}

	inline void SetMoveAngle(const Vector3& moveAngle)
	{
		m_CBuffer->SetMoveAngle(moveAngle);
	}
};

