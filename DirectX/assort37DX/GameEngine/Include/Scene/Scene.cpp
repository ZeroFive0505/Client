
#include "Scene.h"
#include "../PathManager.h"
#include "SceneManager.h"
#include "../Component/CameraComponent.h"

CScene::CScene()
{
	m_Mode = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_Viewport = new CViewport;
	m_NavManager = new CNavigationManager;
	m_LightManager = new CLightManager;

	m_Mode->m_Scene = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_Viewport->m_Scene = this;
	m_NavManager->m_Scene = this;
	m_LightManager->m_Scene = this;

	m_Start = false;

	m_Collision->Init();
	m_CameraManager->Init();
	m_Viewport->Init();
	m_NavManager->Init();
	m_LightManager->Init();

	m_Change = true;
}

CScene::~CScene()
{
	m_ObjList.clear();
	SAFE_DELETE(m_NavManager);
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Resource);
	m_LightManager->Destroy();
	SAFE_DELETE(m_LightManager);
}

void CScene::Start()
{
	m_Mode->Start();

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_Start = true;

	m_CameraManager->Start();
	m_Collision->Start();
	m_Viewport->Start();
	m_LightManager->Start();

	if (m_Mode->GetPlayerObject())
	{
		CCameraComponent* Camera = m_Mode->GetPlayerObject()->FindComponentFromType<CCameraComponent>();

		if (Camera)
		{
			m_CameraManager->SetCurrentCamera(Camera);
		}
	}

	m_NavManager->Start();
}

void CScene::Update(float DeltaTime)
{
	m_Mode->Update(DeltaTime);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_CameraManager->Update(DeltaTime);

	m_Viewport->Update(DeltaTime);

	m_NavManager->Update(DeltaTime);

	m_LightManager->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	m_Mode->PostUpdate(DeltaTime);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	m_CameraManager->PostUpdate(DeltaTime);

	m_Viewport->PostUpdate(DeltaTime);

	// 충돌체들을 충돌 영역에 포함시킨다.
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}

	// 포함된 충돌체들을 이용해서 충돌처리를 진행한다.
	m_Collision->Collision(DeltaTime);
}

void CScene::Save(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CScene::SaveFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	size_t	SceneModeType = m_Mode->GetTypeID();

	fwrite(&SceneModeType, sizeof(size_t), 1, File);

	size_t	ObjCount = m_ObjList.size();

	fwrite(&ObjCount, sizeof(size_t), 1, File);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		size_t	ObjType = (*iter)->GetTypeID();

		fwrite(&ObjType, sizeof(size_t), 1, File);

		(*iter)->Save(File);
	}


	fclose(File);
}

void CScene::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CScene::LoadFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;

	m_ObjList.clear();

	size_t	SceneModeType = 0;

	fread(&SceneModeType, sizeof(size_t), 1, File);

	// SceneMode 생성
	CSceneManager::GetInst()->CallCreateSceneMode(this, SceneModeType);

	size_t	ObjCount = m_ObjList.size();

	fread(&ObjCount, sizeof(size_t), 1, File);

	for (size_t i = 0; i < ObjCount; ++i)
	{
		size_t	ObjType = 0;
		fread(&ObjType, sizeof(size_t), 1, File);

		CGameObject* Obj = CSceneManager::GetInst()->CallCreateObject(this, ObjType);

		Obj->Load(File);
	}

	fclose(File);
}
