#include "Scene.h"
#include "../PathManager.h"
#include "SceneManager.h"
#include "../GameObject/SkyObject.h"
#include "../Component/SceneComponent.h"
#include "../Input.h"
#include "../Collision/Collision.h"

CScene::CScene()
{
	m_Mode = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_Viewport = new CViewport;
	m_NavManager = new CNavigationManager;
	m_Nav3DManager = new CNavigarion3DManager;
	m_LightManager = new CLightManager;

	m_Mode->m_Scene = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_Viewport->m_Scene = this;
	m_NavManager->m_Scene = this;
	m_LightManager->m_Scene = this;
	m_Nav3DManager->m_Scene = this;

	m_Start = false;

	m_Collision->Init();
	m_CameraManager->Init();
	m_Viewport->Init();
	m_NavManager->Init();
	m_Nav3DManager->Init();
	m_LightManager->Init();

	m_Change = true;

	m_SkyObject = new CSkyObject;

	m_SkyObject->SetName("Sky");
	m_SkyObject->SetScene(this);

	m_SkyObject->Init();
}

CScene::~CScene()
{
	SAFE_DELETE(m_NavManager);
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_Nav3DManager);
	m_LightManager->Destroy();
	SAFE_DELETE(m_LightManager);
}

void CScene::Start()
{
	m_Mode->Start();


	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Start();
	}

	m_Start = true;

	m_SkyObject->Start();

	m_CameraManager->Start();
	m_Collision->Start();
	m_Viewport->Start();

	if (m_Mode->GetPlayerObject())
	{
		CCameraComponent* camera = m_Mode->GetPlayerObject()->FindComponentByType<CCameraComponent>();

		if (camera)
		{
			m_CameraManager->SetCurrentCamera(camera);
		}
	}

	m_NavManager->Start();

	m_Nav3DManager->Start();
}

void CScene::Update(float deltaTime)
{
	m_Mode->Update(deltaTime);

	m_SkyObject->Update(deltaTime);

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

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
			iter++;
			continue;
		}

		(*iter)->Update(deltaTime);
		iter++;
	}

	m_CameraManager->Update(deltaTime);

	m_Viewport->Update(deltaTime);

	m_NavManager->Update(deltaTime);

	m_Nav3DManager->Update(deltaTime);

	m_LightManager->Update(deltaTime);
}

void CScene::PostUpdate(float deltaTime)
{
	m_Mode->PostUpdate(deltaTime);

	m_SkyObject->PostUpdate(deltaTime);

	// m_ObjList.sort(CScene::YSort);

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

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
			iter++;
			continue;
		}

		(*iter)->PostUpdate(deltaTime);
		iter++;
	}

	m_CameraManager->PostUpdate(deltaTime);

	m_Viewport->PostUpdate(deltaTime);

	// 모든 PostUpdate가 끝나면 Min, Max 회전 이후 축의 정보들이 계산이 되어있다. 따라서 PostUpdate 루프가 끝나고 충돌을 계산한다.
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	m_RenderComponentList.clear();

	// PostUpdate이후 컬링 여부를 판단하여 컬링이 안된 오브젝트의 모든 컴포넌트를 추가한다.
	for (; iter != iterEnd; iter++)
	{
		(*iter)->AddCollision();

		const std::list<CSceneComponent*>& componentList = (*iter)->GetSceneComponents();

		for (auto c : componentList)
		{
			if (c->GetRender() && !c->GetCulling())
				m_RenderComponentList.push_back(c);
		}
	}

	// 출력되는 물체들을 정렬한다.
	if (m_RenderComponentList.size() >= 2)
		m_RenderComponentList.sort(SortRenderList);

	// 추가된 충돌체들로 충돌을 처리한다.
	m_Collision->Collision(deltaTime);
}

void CScene::Save(const char* fileName, const std::string& pathName)
{
	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (info)
		strcpy_s(fullPath, info->pathMultiByte);

	strcat_s(fullPath, fileName);

	SaveFullPath(fullPath);
}

void CScene::SaveFullPath(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "wb");

	if (!pFile)
		return;

	size_t sceneModeType = m_Mode->GetTypeID();

	fwrite(&sceneModeType, sizeof(size_t), 1, pFile);

	size_t objCount = m_ObjList.size();

	fwrite(&objCount, sizeof(size_t), 1, pFile);

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		size_t objType = (*iter)->GetTypeID();

		fwrite(&objType, sizeof(size_t), 1, pFile);

		(*iter)->Save(pFile);
	}

	fclose(pFile);
}

void CScene::Load(const char* fileName, const std::string& pathName)
{
	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (info)
		strcpy_s(fullPath, info->pathMultiByte);

	strcat_s(fullPath, fileName);

	LoadFullPath(fullPath);
}

void CScene::LoadFullPath(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "rb");

	if (!pFile)
		return;

	// 로드하기전 먼저 오브젝트 리스트를 초기화한다.
	m_ObjList.clear();

	size_t sceneModeType = 0;

	fread(&sceneModeType, sizeof(size_t), 1, pFile);

	// SceneMode를 생성
	// 씬의 타입은 클라이언트, 에디터에서 새롭게 정의된 타입일 수도 있다.
	// 따라서 콜백함수를 묶고 호출해서 클라이언트 에디터에서 조건문으로 알맞은 씬 타입을 만들어낸다.
	CSceneManager::GetInst()->CallCreateSceneMode(this, sceneModeType);

	size_t objCount = m_ObjList.size();

	fread(&objCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < objCount; i++)
	{
		size_t objType = 0;
		fread(&objType, sizeof(size_t), 1, pFile);

		// 게임 오브젝트도 마찬가지로 클라이언트에서(예: Player2D) 또는 에디터(DragObject)등에서 
		// 엔진에서 정의되지 않은 타입의 오브젝트가 있을 수 있다.
		// 따라서 콜백함수를 호출하여 클라이언트, 에디터에서 알맞은 타입으로 만들어 낸다.
		CGameObject* obj = CSceneManager::GetInst()->CallCreateObject(this, objType);

		obj->Load(pFile);
	}

	fclose(pFile);
}

bool CScene::Picking(CGameObject*& result, Vector3& hitPoint)
{
	CCameraComponent* camera = m_CameraManager->GetCurrentCamera();

	sRay ray = CInput::GetInst()->GetRay(camera->GetViewMatrix());

	auto iter = m_RenderComponentList.begin();
	auto iterEnd = m_RenderComponentList.end();

	for (; iter != iterEnd; iter++)
	{
		sSphereInfo info = (*iter)->GetSphereInfo();

		if (!(*iter)->IsPickable())
			continue;

		if (CCollision::CollisionRayToSphere(hitPoint, ray, info))
		{
			result = (*iter)->GetGameObject();
			return true;
		}
	}

	return false;
}

bool CScene::SortRenderList(CSceneComponent* src, CSceneComponent* dest)
{
	// 정렬시 카메라 기준으로 정렬한다.
	sSphereInfo srcInfo = src->GetSphereInfoViewSpace();
	sSphereInfo destInfo = dest->GetSphereInfoViewSpace();

	// 카메라 기준으로 이동했기에 중심점의 위치가 거리가 된다.
	// 거기서 반지름을 빼서 위치를 구한다.
	return srcInfo.center.Length() - srcInfo.radius > destInfo.center.Length() - destInfo.radius;
}
