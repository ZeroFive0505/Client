#include "AnimationSequence2DInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Texture/Texture.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_PlayAnimation(false),
	m_CBuffer(nullptr),
	m_CurrentAnimation(nullptr)
{
	SetTypeID<CAnimationSequence2DInstance>();
}

// 애니메이션 복사
CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim)
{
	// 모든 애니메이션을 순회하면서 복사한다.
	auto iter = anim.m_mapAnimation.begin();
	auto iterEnd = anim.m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		CAnimationSequence2DData* data = new CAnimationSequence2DData;

		data->m_Sequence = iter->second->m_Sequence;
		data->m_Name = iter->second->m_Name;
		data->m_Loop = iter->second->m_Loop;
		data->m_PlayTime = iter->second->m_PlayTime;
		data->m_PlayScale = iter->second->m_PlayScale;
		data->m_Reverse = iter->second->m_Reverse;

		// 복사 대상의 현재 애니메이션을 찾았다면
		// 현재 애니메이션으로 만든다.
		if (anim.m_CurrentAnimation->m_Name == data->m_Name)
			m_CurrentAnimation = data;

		m_mapAnimation.insert(std::make_pair(iter->first, data));
	}
}

CAnimationSequence2DInstance::~CAnimationSequence2DInstance()
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& sequenceName, const std::string& name,
	bool loop, float playTime, float playScale, bool reverse, bool isMenual)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (anim)
		return;


	CAnimationSequence2D* sequence = nullptr;

	// 현재 소속씬이 있으면 소속 씬에서 애니메이션을 가져온다.
	if (m_Scene)
	{
		sequence = m_Scene->GetSceneResource()->FindAnimationSequence2D(sequenceName);
	}
	else
	{
		sequence = CResourceManager::GetInst()->FindAnimationSequence2D(sequenceName);
	}

	anim = new CAnimationSequence2DData;

	anim->m_Sequence = sequence;
	anim->m_Name = name;
	anim->m_Loop = loop;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_Reverse = reverse;
	anim->m_FrameTime = playTime / sequence->GetFrameCount();
	anim->m_IsMenual = isMenual;

	// 만약 첫번째로 추가되는 애니메이션이라면 기본 애니메이션으로 만든다.
	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, anim->m_Sequence->GetTexture()->GetName(),
				anim->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(name, anim));
}

void CAnimationSequence2DInstance::AddAnimation(const TCHAR* fileName,
	const std::string& pathName, const std::string& name,
	bool loop, float playTime, float playScale, bool reverse, bool isMenual)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (anim)
		return;

	char fileNameMultiByte[256] = {};

	int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMultiByte, length, 0, 0);

	CAnimationSequence2D* sequence = nullptr;

	if (m_Scene)
	{
		std::string sequenceName;

		m_Scene->GetSceneResource()->LoadSequence2D(sequenceName, fileNameMultiByte);
		sequence = m_Scene->GetSceneResource()->FindAnimationSequence2D(sequenceName);
	}
	else
	{
		std::string sequenceName;

		CResourceManager::GetInst()->LoadSequence2D(sequenceName, fileNameMultiByte);
		sequence = CResourceManager::GetInst()->FindAnimationSequence2D(sequenceName);
	}

	if (!sequence)
		return;

	anim = new CAnimationSequence2DData;

	anim->m_Sequence = sequence;
	anim->m_Name = name;
	anim->m_Loop = loop;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_Reverse = reverse;
	anim->m_FrameTime = playTime / sequence->GetFrameCount();
	anim->m_IsMenual = isMenual;

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, anim->m_Sequence->GetTexture()->GetName(),
				anim->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(name, anim));
}

void CAnimationSequence2DInstance::SetPlayTime(const std::string& name, float playTime)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_PlayTime = playTime;
}

void CAnimationSequence2DInstance::SetPlayScale(const std::string& name, float playScale)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_PlayScale = playScale;
}

void CAnimationSequence2DInstance::SetReverse(const std::string& name, bool reverse)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_Reverse = reverse;
}

void CAnimationSequence2DInstance::SetLoop(const std::string& name, bool loop)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (!anim)
		return;

	anim->m_Loop = loop;
}

void CAnimationSequence2DInstance::SetCurrentAnimation(const std::string& name)
{
	m_CurrentAnimation = FindAnimation(name);

	if (!m_CurrentAnimation)
		return;

	// 현재 애니메이션을 설정했으므로 프레임과 재생 시간을 초기화한다.
	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time = 0.0f;
	m_CurrentAnimation->m_Start = false;

	size_t size = m_CurrentAnimation->m_vecNotify.size();

	// 모든 콜백함수도 초기화한다.
	for (size_t i = 0; i < size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->call = false;
	}

	if (m_Owner)
	{
		m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
			m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
			m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

void CAnimationSequence2DInstance::ChangeAnimation(const std::string& name)
{
	if (m_CurrentAnimation->m_Name == name ||
		!FindAnimation(name))
		return;

	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time = 0.0f;
	m_CurrentAnimation->m_Start = false;

	size_t size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->call = false;
	}

	m_CurrentAnimation = FindAnimation(name);
	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time = 0.0f;
	m_CurrentAnimation->m_Start = false;

	if (m_Owner)
	{
		m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
			m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
			m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::CheckCurrentAnimation(const std::string& name)
{
	return m_CurrentAnimation->m_Name == name;
}


void CAnimationSequence2DInstance::Start()
{
	if (m_Owner && m_CurrentAnimation)
	{
		m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
			m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
			m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::Init()
{
	if (m_Scene)
		m_CBuffer = m_Scene->GetSceneResource()->GetAnimation2DCBuffer();

	return true;
}

void CAnimationSequence2DInstance::Update(float deltaTime)
{
	if (!m_CurrentAnimation || !m_PlayAnimation || m_CurrentAnimation->m_Sequence->GetFrameCount() == 0)
		return;

	if (m_CurrentAnimation->m_IsMenual)
		return;

	m_CurrentAnimation->m_Time += deltaTime * m_CurrentAnimation->m_PlayScale;

	m_CurrentAnimation->m_FrameTime = m_CurrentAnimation->m_PlayTime / m_CurrentAnimation->m_Sequence->GetFrameCount();

	bool animEnd = false;

	if (!m_CurrentAnimation->m_Start && m_CurrentAnimation->m_StartFunction)
	{
		m_CurrentAnimation->m_Start = true;
		m_CurrentAnimation->m_StartFunction();
	}

	if (m_CurrentAnimation->m_Time >= m_CurrentAnimation->m_FrameTime)
	{
		m_CurrentAnimation->m_Time -= m_CurrentAnimation->m_FrameTime;

		if (m_CurrentAnimation->m_Reverse)
		{
			m_CurrentAnimation->m_Frame--;

			if (m_CurrentAnimation->m_Frame < 0)
				animEnd = true;
		}
		else
		{
			m_CurrentAnimation->m_Frame++;

			if (m_CurrentAnimation->m_Frame == m_CurrentAnimation->m_Sequence->GetFrameCount())
				animEnd = true;
		}
	}

	// 호출해야할 콜백함수를 체크
	size_t size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < size; i++)
	{
		if (!m_CurrentAnimation->m_vecNotify[i]->call &&
			m_CurrentAnimation->m_vecNotify[i]->frame == m_CurrentAnimation->m_Frame)
		{
			m_CurrentAnimation->m_vecNotify[i]->call = true;
			m_CurrentAnimation->m_vecNotify[i]->callBack();
		}
	}

	if (animEnd)
	{
		if (m_CurrentAnimation->m_Loop)
		{
			if (m_CurrentAnimation->m_Reverse)
				m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;
			else
				m_CurrentAnimation->m_Frame = 0;
		}
		else
		{
			if (m_CurrentAnimation->m_Reverse)
				m_CurrentAnimation->m_Frame = 0;
			else
				m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;
		}

		if (m_CurrentAnimation->m_EndFunction)
			m_CurrentAnimation->m_EndFunction();

		if (m_CurrentAnimation->m_Loop)
		{
			size_t size = m_CurrentAnimation->m_vecNotify.size();

			for (size_t i = 0; i < size; i++)
			{
				m_CurrentAnimation->m_vecNotify[i]->call = false;
			}
		}
	}
}

void CAnimationSequence2DInstance::SetShader()
{
	if (!m_CurrentAnimation)
		return;

	// 커다란 텍스쳐에서 원하는 부분의 UV를 구한다.
	Vector2 startUV, endUV;
	Vector2 start = m_CurrentAnimation->m_Sequence->GetFrameData(m_CurrentAnimation->m_Frame).start;
	Vector2 frameSize = m_CurrentAnimation->m_Sequence->GetFrameData(m_CurrentAnimation->m_Frame).size;

	// 현재 애니메이션의 시작위치를 구한뒤에 크기로 나누면 UV의 시작위치가 나온다.
	startUV = start / Vector2((float)m_CurrentAnimation->m_Sequence->GetTexture()->GetWidth(),
		(float)m_CurrentAnimation->m_Sequence->GetTexture()->GetHeight());

	// 마찬가지
	endUV = (start + frameSize) / Vector2((float)m_CurrentAnimation->m_Sequence->GetTexture()->GetWidth(),
		(float)m_CurrentAnimation->m_Sequence->GetTexture()->GetHeight());

	if (m_CBuffer)
	{
		// 위에서 구한 값을 상수버퍼로 전달한다.
		m_CBuffer->SetAnimation2DType(m_CurrentAnimation->m_Sequence->GetTexture()->GetImageType());
		m_CBuffer->SetAnimationHrizontalFlip(m_HorizontalFlip);
		m_CBuffer->SetStartUV(startUV);
		m_CBuffer->SetEndUV(endUV);
		m_CBuffer->SetAnimationSize(Vector2((float)m_CurrentAnimation->m_Sequence->GetTexture()->GetWidth(),
			(float)m_CurrentAnimation->m_Sequence->GetTexture()->GetHeight()));

		m_CBuffer->UpdateCBuffer();
	}
}

void CAnimationSequence2DInstance::ResetShader()
{
}

CAnimationSequence2DInstance* CAnimationSequence2DInstance::Clone()
{
	return new CAnimationSequence2DInstance(*this);
}

void CAnimationSequence2DInstance::Save(FILE* pFile)
{
	int animCount = (int)m_mapAnimation.size();
	fwrite(&animCount, sizeof(int), 1, pFile);

	if (animCount == 0)
		return;

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	// 현재 애니메이션 인스턴스가 기지고있는 애니메이션 키와 시퀀스를 저장한다.
	for (; iter != iterEnd; iter++)
	{
		int length = (int)iter->first.length();
		fwrite(&length, sizeof(int), 1, pFile);
		fwrite(iter->first.c_str(), sizeof(char), length, pFile);

		iter->second->Save(pFile);
	}

	const std::string& currentAnimName = m_CurrentAnimation->GetName();
	int length = (int)currentAnimName.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(currentAnimName.c_str(), sizeof(char), length, pFile);

	fwrite(&m_PlayAnimation, sizeof(bool), 1, pFile);
}

void CAnimationSequence2DInstance::Load(FILE* pFile)
{
	int animCount = 0;
	fread(&animCount, sizeof(int), 1, pFile);

	if (animCount == 0)
		return;

	for (int i = 0; i < animCount; i++)
	{
		int length = 0;
		char animName[256] = {};

		fread(&length, sizeof(int), 1, pFile);
		fread(animName, sizeof(char), length, pFile);

		CAnimationSequence2DData* data = new CAnimationSequence2DData;

		data->Load(pFile);

		// 시퀀스를 이름으로 찾아온다.
		if (m_Scene)
		{
			data->m_Sequence = m_Scene->GetSceneResource()->FindAnimationSequence2D(data->m_SequenceName);
		}
		else
		{
			data->m_Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(data->m_SequenceName);
		}

		m_mapAnimation.insert(std::make_pair(animName, data));
	}

	int length = 0;
	char currentName[256] = {};

	fread(&length, sizeof(int), 1, pFile);
	fread(currentName, sizeof(char), length, pFile);

	m_CurrentAnimation = FindAnimation(currentName);

	fread(&m_PlayAnimation, sizeof(bool), 1, pFile);

	if (m_Scene)
		m_CBuffer = m_Scene->GetSceneResource()->GetAnimation2DCBuffer();
}

void CAnimationSequence2DInstance::Save(const char* fullPath)
{
	{
		FILE* pFile = nullptr;

		fopen_s(&pFile, fullPath, "wb");

		if (!pFile)
			return;

		int animCount = (int)m_mapAnimation.size();
		fwrite(&animCount, sizeof(int), 1, pFile);

		auto iter = m_mapAnimation.begin();
		auto iterEnd = m_mapAnimation.end();

		// 현재 애니메이션 인스턴스가 기지고있는 애니메이션 키와 시퀀스를 저장한다.
		for (; iter != iterEnd; iter++)
		{
			int length = (int)iter->first.length();
			fwrite(&length, sizeof(int), 1, pFile);
			fwrite(iter->first.c_str(), sizeof(char), length, pFile);

			iter->second->Save(pFile);
		}

		const std::string& currentAnimName = m_CurrentAnimation->GetName();
		int length = (int)currentAnimName.length();
		fwrite(&length, sizeof(int), 1, pFile);
		fwrite(currentAnimName.c_str(), sizeof(char), length, pFile);

		fwrite(&m_PlayAnimation, sizeof(bool), 1, pFile);

		fclose(pFile);
	}
}

void CAnimationSequence2DInstance::Load(const char* fullPath)
{
	{
		FILE* pFile = nullptr;

		fopen_s(&pFile, fullPath, "rb");

		if (!pFile)
			return;

		Load(pFile);
	}
}

CAnimationSequence2DData* CAnimationSequence2DInstance::FindAnimation(const std::string& name)
{
	auto iter = m_mapAnimation.find(name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
