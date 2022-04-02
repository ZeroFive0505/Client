#include "Component.h"

CComponent::CComponent() :
	m_Scene(nullptr),
	m_Object(nullptr)
{
}

CComponent::CComponent(const CComponent& com)
{
	*this = com;
	// com에서 참조수가 같이 복사가되면 안된다.
	// 따라서 초기화
	m_RefCount = 0;

	// 씬은 복사하면서 같은 씬에 소속되지만
	// 게임 오브젝트는 아니다.
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

	// 컴포넌트의 타입을 저장한다.
	fwrite(&m_ComponentType, sizeof(Component_Type), 1, pFile);
}

void CComponent::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_ComponentType, sizeof(Component_Type), 1, pFile);
}
