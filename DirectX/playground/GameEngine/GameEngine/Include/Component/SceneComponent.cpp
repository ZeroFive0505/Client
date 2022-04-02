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

	// �� ������Ʈ�� �����Ǹ��ڸ� Ʈ�������� ���Ӱ� �����ȴ�.
	m_Transform = new CTransform;

	// Ʈ�������� ������ �� �� ������Ʈ.
	m_Transform->m_Owner = this;
	// Ʈ������ �ʱ�ȭ
	m_Transform->Init();

	// ���� ������ �� ������Ʈ���� �θ�� ���� �������� ����.
	m_Parent = nullptr;

	m_LayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com) :
	CComponent(com)
{
	// Ʈ�������� �� ������Ʈ�� ���ϰ��踦 �״�� �����´�.

	// ���� ������ Ʈ�������� �ϴ� �����ؿ´�.
	m_Transform = com.m_Transform->Clone();

	// �ϴ� Ʈ�������� ������ ���δ� �ʱ�ȭ�Ѵ�.
	// ���� �� ������Ʈ�� �ֻ�� ��Ʈ ������Ʈ�̴�.
	// ���� ��Ʈ �� ������Ʈ�� Ʈ������ ������Ʈ�� �θ�� nullptr�̴�.
	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	// Ʈ������ ������Ʈ�� ���� �� ������Ʈ�� �Ҽ�
	m_Transform->m_Owner = this;
	
	// ���Ӱ� ����� ������Ʈ�̹Ƿ� �ϴ� �θ�� nullptr
	// Ʈ������ ������Ʈ�� ���� ����
	m_Parent = nullptr;

	m_vecChild.clear();

	size_t size = com.m_vecChild.size();

	m_LayerName = com.m_LayerName;

	// ���� ���� ����� ��� �ڽ��� ���ϰ��踦 �����Ѵ�.
	for (size_t i = 0; i < size; i++)
	{
		// ��� ������Ʈ �Ʒ��� �ִ� ������Ʈ���� �����´�.
		CSceneComponent* cloneComponent = com.m_vecChild[i]->Clone();

		// ���Ӱ� �����Ǵ� �� ������Ʈ�� �θ𰡵ȴ�.
		cloneComponent->m_Parent = this;

		// ������Ʈ �ڽĿ� �߰��Ѵ�.
		m_vecChild.push_back(cloneComponent);

		// �ڽ� ������Ʈ�� Ʈ�������� �θ� �缳���Ѵ�.
		cloneComponent->m_Transform->m_Parent = m_Transform;

		// �߰��Ѵ�.
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

		// �ڽ��� DelteChild�� ȣ���Ͽ� ��� ���������� Ž���ؼ� �����Ѵ�.
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

		// �ڽ��� DelteChild�� ȣ���Ͽ� ��� ���������� Ž���ؼ� �����Ѵ�.
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
	// ���� Ʈ������ ������Ʈ�� ������Ʈ�Ѵ�.
	m_Transform->Update(deltaTime);

	size_t size = m_vecChild.size();

	// �Ҽӵ� �ڽĵ��� ��� ������Ʈ �Ѵ�.
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
	// ������ ������۸� �̿��ؼ� ���� ������ ����
	m_Transform->SetTransform();

	// �ִϸ��̼��� �ϴ� ��Ȱ��ȭ ��Ų��.
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

	// ���� �� ������Ʈ�� ������ ���̾��� �̸��� �����Ѵ�.
	int length = (int)m_LayerName.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_LayerName.c_str(), sizeof(char), length, pFile);

	m_Transform->Save(pFile);

	// ��� �ڽ� ������Ʈ�� �ִ����� �����´�.
	int childCount = (int)m_vecChild.size();

	fwrite(&childCount, sizeof(int), 1, pFile);

	// ��� �ڽ� ������Ʈ���� �����Ѵ�.
	for (int i = 0; i < childCount; i++)
	{
		// ������Ʈ ����ÿ� ������Ʈ Ÿ���� ���� �����Ѵ�.
		size_t typeID = m_vecChild[i]->GetTypeID();
		fwrite(&typeID, sizeof(size_t), 1, pFile);
		
		// �ڽ� ������Ʈ ����
		m_vecChild[i]->Save(pFile);
	}
}

void CSceneComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);

	fread(&m_Render, sizeof(bool), 1, pFile);

	// ���̾��� �̸��� �������� �Ҵ��Ѵ�.
	int length = 0;
	char layerName[256] = {};
	fread(&length, sizeof(int), 1, pFile);
	fread(layerName, sizeof(char), length, pFile);

	m_LayerName = layerName;

	m_Transform->Load(pFile);;

	int childCount = 0;

	fread(&childCount, sizeof(int), 1, pFile);

	// ��ϵ� �ڽ��� ����ŭ �ݺ��ϸ鼭 �ε��Ѵ�.
	for (int i = 0; i < childCount; i++)
	{
		// Ÿ�� ���̵� �޾ƿ���
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, pFile);

		// ���� ������Ʈ�� �ش� Ÿ�� ���̵��� ������Ʈ�� �߰��ϰ� ��ȯ�޴´�.
		CComponent* component = CSceneManager::GetInst()->CallCreateComponent(m_Object, typeID);

		// ������ �ε�
		component->Load(pFile);

		// �߰��Ѵ�.
		m_vecChild.push_back((CSceneComponent*)component);
	}
}
