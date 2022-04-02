#include "GameObject.h"
#include "../Scene/SceneManager.h"
#include "../PathManager.h"
#include "../Component/NavAgent.h"

CGameObject::CGameObject() :
    m_Scene(nullptr),
    m_Parent(nullptr),
    m_LifeSpan(0.0f)
{
    SetTypeID<CGameObject>();
    m_YPos = FLT_MAX;
}

CGameObject::CGameObject(const CGameObject& obj)
{
    *this = obj;

    m_RefCount = 0;

    // ���� ��Ʈ ������Ʈ�� ������
    if (obj.m_RootComponent)
    {
        // ��Ʈ ������Ʈ�� �����Ѵ�.
        m_RootComponent = obj.m_RootComponent->Clone();

        // ������ ��Ʈ ������Ʈ�� �����ϰ� �ִ� ���ӿ�����Ʈ�� �����Ѵ�.
        m_RootComponent->SetGameObject(this);

        // ������ ��Ʈ ������Ʈ�� �� ������Ʈ�� �����Ѵ�.
        m_RootComponent->SetSceneComponent(this);
    }

    // ������Ʈ ������Ʈ�� ����.
    m_vecObjectComponent.clear();

    size_t size = obj.m_vecObjectComponent.size();

    // ������Ʈ ������Ʈ�� ��� �����Ѵ�.
    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent.push_back(obj.m_vecObjectComponent[i]->Clone());

        m_vecObjectComponent[i]->SetGameObject(this);
    }
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetScene(CScene* scene)
{
    m_Scene = scene;
}

CComponent* CGameObject::FindComponent(const std::string name)
{
    {
        auto iter = m_SceneComponentList.begin();
        auto iterEnd = m_SceneComponentList.end();

        for (; iter != iterEnd; iter++)
        {
            if ((*iter)->GetName() == name)
                return *iter;
        }
    }

    {
        auto iter = m_vecObjectComponent.begin();
        auto iterEnd = m_vecObjectComponent.end();

        for (; iter != iterEnd; iter++)
        {
            if ((*iter)->GetName() == name)
                return *iter;
        }
    }

    return nullptr;
}

void CGameObject::GetAllSceneComponentsName(std::vector<sFindComponentName>& vecNames)
{
    if (!m_RootComponent)
        return;

    // ��Ʈ ������Ʈ �������� ��� �̸��� �����´�.
    m_RootComponent->GetAllSceneComponentsName(vecNames);
}

void CGameObject::AddChildGameObject(CGameObject* obj)
{
    obj->m_Parent = this;

    m_RootComponent->AddChild(obj->m_RootComponent);

    m_vecChildObject.push_back(obj);
}

void CGameObject::Destroy()
{
    CRef::Destroy();

    m_RootComponent->Destroy();

    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->Destroy();
    }
}

void CGameObject::Start()
{
    if (m_RootComponent)
        m_RootComponent->Start();


    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->Start();
    }
}

bool CGameObject::Init()
{
    return true;
}

void CGameObject::Update(float deltaTime)
{
    if (m_LifeSpan > 0.0f)
    {
        m_LifeSpan -= deltaTime;

        if (m_LifeSpan <= 0.0f)
        {
            Destroy();
            return;
        }
    }

    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->Update(deltaTime);
    }

    if (m_RootComponent)
        m_RootComponent->Update(deltaTime);
}

void CGameObject::PostUpdate(float deltaTime)
{
    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->PostUpdate(deltaTime);
    }

    if (m_RootComponent)
        m_RootComponent->PostUpdate(deltaTime);
}

void CGameObject::AddCollision()
{
    if (m_RootComponent)
        m_RootComponent->CheckCollision();
}

void CGameObject::PrevRender()
{
    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->PrevRender();
    }

    if (m_RootComponent)
        m_RootComponent->PrevRender();
}

void CGameObject::Render()
{
    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->Render();
    }

    if (m_RootComponent)
        m_RootComponent->Render();
}

void CGameObject::PostRender()
{
    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecObjectComponent[i]->PostRender();
    }

    if (m_RootComponent)
        m_RootComponent->PostRender();
}

CGameObject* CGameObject::Clone()
{
    return new CGameObject(*this);
}

void CGameObject::Save(FILE* pFile)
{
    // ���� ���۷��� ������ �����Ѵ�.
    CRef::Save(pFile);

    // ���� ��Ʈ ������Ʈ�� �ִٸ�
    if (m_RootComponent)
    {
        bool hasRootComponent = true;
        fwrite(&hasRootComponent, sizeof(bool), 1, pFile);

        size_t typeID = m_RootComponent->GetTypeID();
        fwrite(&typeID, sizeof(size_t), 1, pFile);

        m_RootComponent->Save(pFile);
    }
    else
    {
        bool hasRootComponent = false;
        fwrite(&hasRootComponent, sizeof(bool), 1, pFile);
    }

    int objComponentCount = (int)m_vecObjectComponent.size();

    fwrite(&objComponentCount, sizeof(int), 1, pFile);

    for (int i = 0; i < objComponentCount; i++)
    {
        // ����ÿ� Ÿ�� ���̵� ���� �����Ѵ�.
        size_t typeID = m_vecObjectComponent[i]->GetTypeID();
        fwrite(&typeID, sizeof(size_t), 1, pFile);

        m_vecObjectComponent[i]->Save(pFile);
    }
}

void CGameObject::Load(FILE* pFile)
{
    CRef::Load(pFile);

    bool hasRoot = false;
    fread(&hasRoot, sizeof(bool), 1, pFile);

    if (hasRoot)
    {
        size_t typeID = 0;
        fread(&typeID, sizeof(size_t), 1, pFile);

        // ������Ʈ�� ������ ���ӿ�����Ʈ�� ���� ����
        // �� ������Ʈ�� Ÿ�� ���̵� �´� ������Ʈ�� �߰��Ѵ�.
        CSceneManager::GetInst()->CallCreateComponent(this, typeID);

        // ���� �����Ǵ� ������Ʈ�̱⿡ ���� ������Ʈ�̴�.
        // ������ ������ �ʱ�ȭ�Ѵ�.
        m_RootComponent->Load(pFile);
    }

    int objComponentCount = 0;

    fread(&objComponentCount, sizeof(int), 1, pFile);

    for (int i = 0; i < objComponentCount; i++)
    {
        size_t typeID = 0;
        fread(&typeID, sizeof(size_t), 1, pFile);

        // ������Ʈ���� �������� ������ �̹����� ��ȯ�޾ƿ´�.
        CComponent* component = CSceneManager::GetInst()->CallCreateComponent(this, typeID);

        // ��ȯ�޾� �޾ƿ� ������Ʈ�� ���Ͽ� �Էµ� �����͸� �������� �ʱ�ȭ�Ѵ�.
        component->Load(pFile);

        m_vecObjectComponent.push_back((CObjectComponent*)component);
    }
}

void CGameObject::Save(const char* fullPath)
{
    FILE* pFile = nullptr;

    fopen_s(&pFile, fullPath, "wb");

    if (!pFile)
        return;

    Save(pFile);

    fclose(pFile);
}

void CGameObject::Load(const char* fullPath)
{
    FILE* pFile = nullptr;

    fopen_s(&pFile, fullPath, "rb");

    if (!pFile)
        return;

    Load(pFile);

    fclose(pFile);
}

void CGameObject::Save(const char* fileName, const std::string& pathName)
{
    char fullPath[MAX_PATH] = {};

    const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);


    if(info)
        strcpy_s(fullPath, info->pathMultiByte);

    strcat_s(fullPath, fileName);

    Save(fullPath);
}

void CGameObject::Load(const char* fileName, const std::string& pathName)
{
    char fullPath[MAX_PATH] = {};

    const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

    if (info)
        strcpy_s(fullPath, info->pathMultiByte);

    strcat_s(fullPath, fileName);

    Load(fullPath);
}

void CGameObject::Move(const Vector3& pos)
{
    size_t size = m_vecObjectComponent.size();

    for (size_t i = 0; i < size; i++)
    {
        if (m_vecObjectComponent[i]->TypeCheck<CNavAgent>())
        {
            ((CNavAgent*)*m_vecObjectComponent[i])->Move(pos);
            break;
        }
    }
}
