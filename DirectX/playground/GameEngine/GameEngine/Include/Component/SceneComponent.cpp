#include "SceneComponent.h"
#include "../Render/RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	// 씬 컴포넌트는 생성되마자마 트랜스폼이 새롭게 생성된다.
	m_Transform = new CTransform;

	// 트랜스폼의 주인은 이 씬 컴포넌트.
	m_Transform->m_Owner = this;
	// 트랜스폼 초기화
	m_Transform->Init();

	// 새로 생성된 씬 컴포넌트에게 부모는 아직 존재하지 않음.
	m_Parent = nullptr;

	m_LayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com) :
	CComponent(com)
{
	// 트랜스폼은 씬 컴포넌트의 상하관계를 그대로 가져온다.

	// 복사 생성시 트랜스폼을 일단 복사해온다.
	m_Transform = com.m_Transform->Clone();

	// 일단 트랜스폼의 정보를 전부다 초기화한다.
	// 현재 씬 컴포넌트는 최상단 루트 컴포넌트이다.
	// 따라서 루트 씬 컴포넌트의 트랜스폼 컴포넌트의 부모는 nullptr이다.
	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	// 트랜스폼 컴포넌트는 현재 이 컴포넌트의 소속
	m_Transform->m_Owner = this;
	
	// 새롭게 복사된 컴포넌트이므로 일단 부모는 nullptr
	// 트랜스폼 컴포넌트와 같은 이유
	m_Parent = nullptr;

	m_vecChild.clear();

	size_t size = com.m_vecChild.size();

	m_LayerName = com.m_LayerName;

	// 이제 복사 대상의 모든 자식의 상하관계를 복사한다.
	for (size_t i = 0; i < size; i++)
	{
		// 상대 컴포넌트 아래에 있는 컴포넌트들을 가져온다.
		CSceneComponent* cloneComponent = com.m_vecChild[i]->Clone();

		// 새롭게 생성되는 이 컴포넌트가 부모가된다.
		cloneComponent->m_Parent = this;

		// 컴포넌트 자식에 추가한다.
		m_vecChild.push_back(cloneComponent);

		// 자식 컴포넌트의 트랜스폼의 부모를 재설정한다.
		cloneComponent->m_Transform->m_Parent = m_Transform;

		// 추가한다.
		m_Transform->m_vecChild.push_back(cloneComponent->m_Transform);
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::SetSceneComponent(CGameObject* obj)
{
	obj->AddSceneComponent(this);

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->SetSceneComponent(obj);
	}
}

void CSceneComponent::GetAllSceneComponentsName(std::vector<sFindComponentName>& vecNames)
{
	sFindComponentName name;

	name.name = m_Name;
	
	if (m_Parent)
		name.parentName = m_Parent->GetName();

	vecNames.push_back(name);

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->GetAllSceneComponentsName(vecNames);
	}
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->Destroy();
	}
}

void CSceneComponent::SetScene(CScene* scene)
{
	CComponent::SetScene(scene);

	m_Transform->m_Scene = scene;

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->SetScene(scene);
	}
}

void CSceneComponent::SetGameObject(CGameObject* obj)
{
	CComponent::SetGameObject(obj);

	m_Transform->m_Object = obj;

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->SetGameObject(obj);
	}
}

void CSceneComponent::AddChild(CSceneComponent* child)
{
	child->m_Parent = this;

	m_vecChild.push_back(child);

	child->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(child->m_Transform);
}

bool CSceneComponent::DeleteChild(CSceneComponent* child)
{
	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		if (m_vecChild[i] == child)
		{
			auto iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);

			return true;
		}

		// 자식의 DelteChild를 호출하여 모든 계층구조를 탐색해서 삭제한다.
		if (m_vecChild[i]->DeleteChild(child))
			return true;
	}

	return false;
}

bool CSceneComponent::DeleteChild(const std::string& name)
{
	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		if (m_vecChild[i]->GetName() == name)
		{
			auto iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);

			return true;
		}

		// 자식의 DelteChild를 호출하여 모든 계층구조를 탐색해서 삭제한다.
		if (m_vecChild[i]->DeleteChild(name))
			return true;
	}

	return false;
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& name)
{
	if (m_Name == name)
		return this;

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		CSceneComponent* comp = m_vecChild[i]->FindComponent(name);

		if (comp)
			return comp;
	}

	return nullptr;
}

void CSceneComponent::Start()
{
	CComponent::Start();

	m_Transform->Start();
	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->Start();
	}
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(float deltaTime)
{
	// 먼저 트랜스폼 컴포넌트를 업데이트한다.
	m_Transform->Update(deltaTime);

	size_t size = m_vecChild.size();

	// 소속된 자식들을 모두 업데이트 한다.
	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->Update(deltaTime);
	}
}

void CSceneComponent::PostUpdate(float deltaTime)
{
	m_Transform->PostUpdate(deltaTime);

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->PostUpdate(deltaTime);
	}
}

void CSceneComponent::CheckCollision()
{
	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->CheckCollision();
	}
}

void CSceneComponent::PrevRender()
{
	if (m_Render)
		CRenderManager::GetInst()->AddRenderList(this);

	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecChild[i]->PrevRender();
	}
}

void CSceneComponent::Render()
{
	// 렌더시 상수버퍼를 이용해서 월드 정보를 전달
	m_Transform->SetTransform();

	// 애니메이션을 일단 비활성화 시킨다.
	CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
	CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);

	fwrite(&m_Render, sizeof(bool), 1, pFile);

	// 현재 씬 컴포넌트의 렌더링 레이어의 이름을 저장한다.
	int length = (int)m_LayerName.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_LayerName.c_str(), sizeof(char), length, pFile);

	m_Transform->Save(pFile);

	// 몇개의 자식 컴포넌트가 있는지를 가져온다.
	int childCount = (int)m_vecChild.size();

	fwrite(&childCount, sizeof(int), 1, pFile);

	// 모든 자식 컴포넌트들을 저장한다.
	for (int i = 0; i < childCount; i++)
	{
		// 컴포넌트 저장시에 컴포넌트 타입을 같이 저장한다.
		size_t typeID = m_vecChild[i]->GetTypeID();
		fwrite(&typeID, sizeof(size_t), 1, pFile);
		
		// 자식 컴포넌트 저장
		m_vecChild[i]->Save(pFile);
	}
}

void CSceneComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);

	fread(&m_Render, sizeof(bool), 1, pFile);

	// 레이어의 이름을 가져오고 할당한다.
	int length = 0;
	char layerName[256] = {};
	fread(&length, sizeof(int), 1, pFile);
	fread(layerName, sizeof(char), length, pFile);

	m_LayerName = layerName;

	m_Transform->Load(pFile);;

	int childCount = 0;

	fread(&childCount, sizeof(int), 1, pFile);

	// 기록된 자식의 수만큼 반복하면서 로드한다.
	for (int i = 0; i < childCount; i++)
	{
		// 타입 아이디를 받아오고
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, pFile);

		// 현재 오브젝트에 해당 타입 아이디의 컴포넌트를 추가하고 반환받는다.
		CComponent* component = CSceneManager::GetInst()->CallCreateComponent(m_Object, typeID);

		// 데이터 로드
		component->Load(pFile);

		// 추가한다.
		m_vecChild.push_back((CSceneComponent*)component);
	}
}
