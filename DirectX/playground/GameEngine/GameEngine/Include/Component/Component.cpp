#include "Component.h"

CComponent::CComponent() :
	m_Scene(nullptr),
	m_Object(nullptr)
{
}

CComponent::CComponent(const CComponent& com)
{
	*this = com;
	// com���� �������� ���� ���簡�Ǹ� �ȵȴ�.
	// ���� �ʱ�ȭ
	m_RefCount = 0;

	// ���� �����ϸ鼭 ���� ���� �Ҽӵ�����
	// ���� ������Ʈ�� �ƴϴ�.
	m_Object = nullptr;
}

CComponent::~CComponent()
{
}

void CComponent::SetScene(CScene* scene)
{
	m_Scene = scene;
}

void CComponent::SetGameObject(CGameObject* obj)
{
	m_Object = obj;
}

void CComponent::Destroy()
{
	CRef::Destroy();
}

void CComponent::Start()
{
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Save(FILE* pFile)
{
	CRef::Save(pFile);

	// ������Ʈ�� Ÿ���� �����Ѵ�.
	fwrite(&m_ComponentType, sizeof(Component_Type), 1, pFile);
}

void CComponent::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_ComponentType, sizeof(Component_Type), 1, pFile);
}
