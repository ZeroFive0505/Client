#include "AnimationSequenceInstance.h"
#include "../Resource/Shader/AnimationUpdateConstantBuffer.h"
#include "../Resource/Shader/AnimationUpdateShader.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Device.h"

CAnimationSequenceInstance::CAnimationSequenceInstance() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_BoneDataBuffer(nullptr),
	m_PlayAnimation(true),
	m_GlobalTime(0.0f),
	m_SequenceProgress(0.0f),
	m_ChangeTimeAcc(0.0f),
	m_ChangeTime(0.2f)
{
	SetTypeID<CAnimationSequenceInstance>();
}

CAnimationSequenceInstance::CAnimationSequenceInstance(const CAnimationSequenceInstance& anim)
{
	m_PlayAnimation = anim.m_PlayAnimation;

	m_GlobalTime = 0.0f;
	m_SequenceProgress = 0.0f;
	m_ChangeTimeAcc = 0.0f;
	m_ChangeTime = anim.m_ChangeTime;

	m_AnimationUpdateShader = anim.m_AnimationUpdateShader;

	if (anim.m_AnimationUpdateCBuffer)
		m_AnimationUpdateCBuffer = anim.m_AnimationUpdateCBuffer->Clone();

	if (anim.m_OutputBuffer)
		m_OutputBuffer = anim.m_OutputBuffer->Clone();

	if (anim.m_BoneBuffer)
		m_BoneBuffer = anim.m_BoneBuffer->Clone();

	m_Skeleton = anim.m_Skeleton;

	m_mapAnimation.clear();

	auto iter = anim.m_mapAnimation.begin();
	auto iterEnd = anim.m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		CAnimationSequenceData* data = new CAnimationSequenceData;

		data->m_Sequence = iter->second->m_Sequence;
		data->m_Name = iter->second->m_Name;
		data->m_Loop = iter->second->m_Loop;
		data->m_PlayTime = iter->second->m_PlayTime;
		data->m_PlayScale = iter->second->m_PlayScale;

		if (anim.m_CurrentAnimation->m_Name == data->m_Name)
			m_CurrentAnimation = data;

		m_mapAnimation.insert(std::make_pair(iter->first, data));
	}

	D3D11_BUFFER_DESC desc = {};

	desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, &m_BoneDataBuffer);
}

CAnimationSequenceInstance::~CAnimationSequenceInstance()
{
	SAFE_RELEASE(m_BoneDataBuffer);
	SAFE_DELETE(m_OutputBuffer);
	SAFE_DELETE(m_BoneBuffer);
	SAFE_DELETE(m_AnimationUpdateCBuffer);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimationSequenceInstance::AddAnimation(const std::string& sequenceName, const std::string& name, bool loop, float playTime, float playScale)
{
	CAnimationSequenceData* anim = FindAnimation(name);

	if (anim)
		return;

	CAnimationSequence* sequence = nullptr;

	if (m_Scene)
		sequence = m_Scene->GetSceneResource()->FindAnimationSequence(sequenceName);
	else
		sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);

	if (!sequence)
		return;

	anim = new CAnimationSequenceData;

	anim->m_Sequence = sequence;
	anim->m_Name = name;
	anim->m_Loop = loop;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_FrameTime = playTime / sequence->GetKeyFrameCount();

	if (m_mapAnimation.empty())
		m_CurrentAnimation = anim;

	m_mapAnimation.insert(std::make_pair(name, anim));
}

void CAnimationSequenceInstance::AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name, bool loop, float playTime, float playScale)
{
	CAnimationSequenceData* anim = FindAnimation(name);

	if (anim)
		return;

	char fileNameMultibyte[256] = {};

	int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMultibyte, length, 0, 0);

	CAnimationSequence* sequence = nullptr;

	if (m_Scene)
	{
		std::string sequenceName;

		m_Scene->GetSceneResource()->LoadAnimationSequenceMultibyte(loop, sequenceName, fileNameMultibyte);
		sequence = m_Scene->GetSceneResource()->FindAnimationSequence(sequenceName);
	}
	else
	{
		std::string sequenceName;

		CResourceManager::GetInst()->LoadAnimationSequenceMultibyte(loop, sequenceName, fileNameMultibyte);
		sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);
	}

	if (!sequence)
		return;


	anim = new CAnimationSequenceData;

	anim->m_Sequence = sequence;
	anim->m_Name = name;
	anim->m_Loop = loop;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_FrameTime = playTime / sequence->GetKeyFrameCount();

	if (m_mapAnimation.empty())
		m_CurrentAnimation = anim;

	m_mapAnimation.insert(std::make_pair(name, anim));
}

void CAnimationSequenceInstance::SetPlayTime(const std::string& name, float playTime)
{
	CAnimationSequenceData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_PlayTime = playTime;
}

void CAnimationSequenceInstance::SetPlayScale(const std::string& name, float playScale)
{
	CAnimationSequenceData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_PlayScale = playScale;
}

void CAnimationSequenceInstance::SetLoop(const std::string& name, bool loop)
{
	CAnimationSequenceData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_Loop = loop;
}

void CAnimationSequenceInstance::SetCurrentAnimation(const std::string& name)
{
	m_CurrentAnimation = FindAnimation(name);

	if (!m_CurrentAnimation)
		return;

	m_CurrentAnimation->m_Time = 0.0f;

	size_t size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->call = false;
	}
}

void CAnimationSequenceInstance::ChangeAnimation(const std::string& name)
{
	if (m_CurrentAnimation->m_Name == name)
		return;

	size_t size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->call = false;
	}

	CAnimationSequenceData* anim = FindAnimation(name);

	if (anim)
	{
		m_ChangeAnimation = anim;
		m_ChangeAnimation->m_Time = 0.0f;
	}
}

bool CAnimationSequenceInstance::CheckCurrentAnimation(const std::string& name)
{
	return m_CurrentAnimation->m_Name == name;
}

void CAnimationSequenceInstance::Start()
{
	if (m_Scene)
		m_AnimationUpdateShader = (CComputeShader*)m_Scene->GetSceneResource()->FindShader("AnimationUpdateShader");
	else
		m_AnimationUpdateShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("AnimationUpdateShader");

	m_AnimationUpdateCBuffer = new CAnimationUpdateConstantBuffer;

	m_AnimationUpdateCBuffer->Init();

	m_vecBoneMatrix.resize(m_Skeleton->GetBoneCount());

	m_OutputBuffer = new CStructuredBuffer;

	m_OutputBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount(), 0);

	m_BoneBuffer = new CStructuredBuffer;

	m_BoneBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount(), 1);

	D3D11_BUFFER_DESC desc = {};

	desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, &m_BoneDataBuffer);
}

bool CAnimationSequenceInstance::Init()
{
	return true;
}

void CAnimationSequenceInstance::Update(float deltaTime)
{
	if (!m_Skeleton || !m_CurrentAnimation || !m_PlayAnimation || m_CurrentAnimation->m_Sequence->GetKeyFrameCount() == 0)
		return;

	m_GlobalTime += deltaTime * m_CurrentAnimation->m_PlayScale;

	bool change = false;
	bool changeEnd = false;
	bool animEnd = false;

	if (m_ChangeAnimation)
	{
		change = true;
		m_ChangeTimeAcc += deltaTime;

		if (m_ChangeTimeAcc >= m_ChangeTime)
		{
			m_ChangeTimeAcc = m_ChangeTime;
			changeEnd = true;
		}
	}
	else
	{
		m_SequenceProgress = m_GlobalTime / m_CurrentAnimation->m_PlayTime;

		if (m_GlobalTime >= m_CurrentAnimation->m_PlayTime)
		{
			m_GlobalTime = m_CurrentAnimation->m_PlayTime;

			animEnd = true;
		}

		m_AnimationUpdateCBuffer->SetChangeAnimation(false);
		m_AnimationUpdateCBuffer->SetChangeRatio(0.0f);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(0);

		float animationTime = m_GlobalTime + m_CurrentAnimation->m_Sequence->m_StartTime;

		int startFrame = 0;
		int endFrame = m_CurrentAnimation->m_Sequence->m_FrameLength;

		int frameIndex = (int)(animationTime / m_CurrentAnimation->m_Sequence->m_FrameTime);
		int nextFrameIndex = frameIndex + 1;

		if (frameIndex >= m_CurrentAnimation->m_Sequence->m_FrameLength)
			frameIndex = m_CurrentAnimation->m_Sequence->m_FrameLength - 1;

		if (nextFrameIndex >= endFrame)
			nextFrameIndex = startFrame;

		float ratio = (animationTime - m_CurrentAnimation->m_Sequence->m_FrameTime * frameIndex)
			/ m_CurrentAnimation->m_Sequence->m_FrameTime;

		m_AnimationUpdateCBuffer->SetFrameCount(endFrame);
		m_AnimationUpdateCBuffer->SetCurrentFrame(frameIndex);
		m_AnimationUpdateCBuffer->SetNextFrame(nextFrameIndex);
		m_AnimationUpdateCBuffer->SetRatio(ratio);
		m_AnimationUpdateCBuffer->SetBoneCount((int)m_Skeleton->GetBoneCount());

		size_t size = m_CurrentAnimation->m_vecNotify.size();

		for (size_t i = 0; i < size; i++)
		{
			if (!m_CurrentAnimation->m_vecNotify[i]->call &&
				m_CurrentAnimation->m_vecNotify[i]->frame == frameIndex)
			{
				m_CurrentAnimation->m_vecNotify[i]->call = true;
				m_CurrentAnimation->m_vecNotify[i]->function();
			}
		}

		if (animEnd)
		{
			if (m_CurrentAnimation->m_EndFunction)
				m_CurrentAnimation->m_EndFunction();

			if (m_CurrentAnimation->m_Loop)
			{
				m_GlobalTime = 0.0f;
				size_t size = m_CurrentAnimation->m_vecNotify.size();

				for (size_t i = 0; i < size; i++)
				{
					m_CurrentAnimation->m_vecNotify[i]->call = false;
				}
			}
		}
	}

	if (change)
	{
		m_AnimationUpdateCBuffer->SetChangeRatio(m_ChangeTimeAcc / m_ChangeTime);
		m_AnimationUpdateCBuffer->SetChangeAnimation(true);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(m_ChangeAnimation->m_Sequence->m_FrameLength);
		
		m_ChangeAnimation->m_Sequence->SetChangeShader();
	}

	m_AnimationUpdateCBuffer->SetRowIndex(0);

	m_OutputBuffer->SetShader();

	m_AnimationUpdateCBuffer->UpdateCBuffer();

	m_Skeleton->SetShader();

	m_BoneBuffer->SetShader();

	m_CurrentAnimation->m_Sequence->SetShader();

	unsigned int groupX = (unsigned int)m_Skeleton->GetBoneCount() / 256 + 1;

	m_AnimationUpdateShader->Execute(groupX, 1, 1);

	m_CurrentAnimation->m_Sequence->ResetShader();

	m_BoneBuffer->ResetShader();

	m_Skeleton->ResetShader();

	m_OutputBuffer->ResetShader();

	if (change)
		m_ChangeAnimation->m_Sequence->ResetChageShader();

	if (changeEnd)
	{
		m_CurrentAnimation = m_ChangeAnimation;
		m_ChangeAnimation = nullptr;
		m_ChangeTimeAcc = 0.0f;
		m_GlobalTime = 0.0f;
	}


	// 구조화 버퍼에 있는 본 정보를 DataBuffer로 복사한다.
	CDevice::GetInst()->GetContext()->CopyResource(m_BoneDataBuffer, m_BoneBuffer->GetBuffer());

	D3D11_MAPPED_SUBRESOURCE  Map = {};

	CDevice::GetInst()->GetContext()->Map(m_BoneDataBuffer, 0, D3D11_MAP_READ, 0, &Map);

	memcpy(&m_vecBoneMatrix[0], Map.pData, sizeof(Matrix)* m_vecBoneMatrix.size());

	CDevice::GetInst()->GetContext()->Unmap(m_BoneDataBuffer, 0);
	m_Skeleton->Update(deltaTime, m_vecBoneMatrix, m_Owner->GetWorldMatrix());
}

void CAnimationSequenceInstance::SetShader()
{
	m_OutputBuffer->SetShader(106, (int)Buffer_Shader_Type::Vertex);
}

void CAnimationSequenceInstance::ResetShader()
{
	m_OutputBuffer->ResetShader(106, (int)Buffer_Shader_Type::Vertex);
}

void CAnimationSequenceInstance::SetSkeleton(CSkeleton* skeleton)
{
	m_Skeleton = skeleton;
}

CAnimationSequenceInstance* CAnimationSequenceInstance::Clone()
{
	return new CAnimationSequenceInstance(*this);
}

void CAnimationSequenceInstance::Save(FILE* pFile)
{
}

void CAnimationSequenceInstance::Load(FILE* pFile)
{
}

CAnimationSequenceData* CAnimationSequenceInstance::FindAnimation(const std::string& name)
{
	auto iter = m_mapAnimation.find(name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
