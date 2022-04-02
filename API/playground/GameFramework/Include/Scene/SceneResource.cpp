
#include "SceneResource.h"
#include "../Resource/ResourceManager.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{

	{
		auto iter = m_mapAnimationSequence.begin();
		auto iterEnd = m_mapAnimationSequence.end();

		for (; iter != iterEnd;)
		{

			std::string Name = iter->first;

			iter = m_mapAnimationSequence.erase(iter);

			CResourceManager::GetInst()->ReleaseAnimationSeequence(Name);
		}
	}

	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();

		// 씬 리소스는 결국 리소스 매니저에 있는 리소스를 참조하고있을 뿐이다. 따라서 리소스 매니저에서 제거한다.
		for (; iter != iterEnd;)
		{

			std::string Name = iter->first;

			iter = m_mapTexture.erase(iter);

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}



	{
		auto iter = m_mapSound.begin();
		auto iterEnd = m_mapSound.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			// earse를 하면 SharedPtr이므로 자동으로 객체가 제거되며 카운트가
			// 1 감소한다. erase는 지운 다음 iterator를 반환하므로 ++을 생략가능
			iter = m_mapSound.erase(iter);

			CResourceManager::GetInst()->ReleaseSound(Name);
		}
	}
}

// 텍스쳐를 이름과 파일이름 그리고 경로로 불러온다.
bool CSceneResource::LoadTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	// 만약 이미 존재하는 이름이라면 반환.
	if (FindTexture(Name))
		return true;

	// 씬 리소스도 리소스를 추가할때는 매니저를 통해서 추가한다.
	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	// 텍스쳐를 가지고 와서
	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	// 현재 맵에 새롭게 추가한다.
	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// 텍스쳐를 이름과 함께 절대 경로로 가져온다.
bool CSceneResource::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// 낱장으로 된 텍스쳐들을 가져온다.
bool CSceneResource::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// 텍스쳐의 칼라키를 설정한다. (해당 색깔 스킵기능)
void CSceneResource::SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index)
{
	CResourceManager::GetInst()->SetTextureColorKey(Name, r, g, b, Index);
}

void CSceneResource::SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
	CResourceManager::GetInst()->SetTextureColorKey(Name, ColorKey, Index);
}

// 텍스쳐를 찾아온다.
CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

// 애니메이션 시퀀스를 텍스쳐의 이름만 전달받아서 만든다.
bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName)
{
	// 만약 이미 존재하는 시퀀스라면 반환.
	if (FindAnimationSequence(SequenceName))
		return true;

	// 리소스매니저를 통해서 새로운 시퀀스를 만든다.
	if (!CResourceManager::GetInst()->CreateAnimationSequence(SequenceName, TextureName))
		return false;

	// 해당 시퀀스를 가지고오고
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	// 해쉬맵에 등록
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// 애니메이션 시퀀스를 이름과 텍스쳐이름 파일이름 경로까지 지정해서 불러온다.
bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	if (FindAnimationSequence(SequenceName))
		return true;

	// 텍스쳐를 불러온다.
	LoadTexture(TextureName, FileName, PathName);

	// 리소스매니저를 통해 시퀀스를 만든다.
	if (!CResourceManager::GetInst()->CreateAnimationSequence(SequenceName, TextureName))
		return false;

	// 해당 시퀀스를 찾아오고
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	// 맵에 등록시킨다.
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	if (FindAnimationSequence(SequenceName))
		return true;
		
	// 낱장으로 분리된 모든 텍스쳐들을 가져온다.
	LoadTexture(TextureName, vecFileName, PathName);

	// 애니메이션 시퀀스를 등록한다.
	if (!CResourceManager::GetInst()->CreateAnimationSequence(SequenceName, TextureName))
		return false;

	// 애니메이션 시퀀스를 가져오고
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	// 맵에 넣는다.
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// 시퀀스에 프레임을 시작위치와 크기를 정해서 넣는다.
void CSceneResource::AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(StartPos, Size);
}

// 시퀀스에 프레임을 시작위치와 크기를 정해서 넣는다.
void CSceneResource::AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(PosX, PosY, SizeX, SizeY);
}

// 현재 등록된 시퀀스를 찾는다.
CAnimationSequence* CSceneResource::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

// 사온드를 로드한다.
bool CSceneResource::LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileName, const std::string& PathName)
{
	// 이미 존재할 경우 로드할 필요는 없다.
	if (FindSound(Name))
		return true;

	// 리소스매니저를 호출해 로드한다.
	if (!CResourceManager::GetInst()->LoadSound(
		GroupName, Loop, Name, FileName, PathName
	))
		return false;

	CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSceneResource::SetVolume(int Volume)
{
	return CResourceManager::GetInst()->SetVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
	return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundResume(Name);
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}
