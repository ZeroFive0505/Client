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

    // 만약 루트 컴포넌트가 있을시
    if (obj.m_RootComponent)
    {
        // 루트 컴포넌트를 복사한다.
        m_RootComponent = obj.m_RootComponent->Clone();

        // 복사한 루트 컴포넌트를 소유하고 있는 게임오브젝트를 설정한다.
        m_RootComponent->SetGameObject(this);

        // 복사한 루트 컴포넌트의 씬 컴포넌트를 지정한다.
        m_RootComponent->SetSceneComponent(this);
    }

    // 오브젝트 컴포넌트를 비운다.
    m_vecObjectComponent.clear();

    size_t size = obj.m_vecObjectComponent.size();

    // 오브젝트 컴포넌트를 모두 복사한다.
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

    // 루트 컴포넌트 기준으로 모든 이름을 가져온다.
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
    // 먼저 레퍼런스 정보를 저장한다.
    CRef::Save(pFile);

    // 만약 루트 컴포넌트가 있다면
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
        // 저장시에 타입 아이디도 같이 저장한다.
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

        // 컴포넌트도 씬모드와 게임오브젝트와 같은 이유
        // 이 오브젝트에 타입 아이디에 맞는 컴포넌트를 추가한다.
        CSceneManager::GetInst()->CallCreateComponent(this, typeID);

        // 최초 생성되는 컴포넌트이기에 루프 컴포넌트이다.
        // 파일의 값으로 초기화한다.
        m_RootComponent->Load(pFile);
    }

    int objComponentCount = 0;

    fread(&objComponentCount, sizeof(int), 1, pFile);

    for (int i = 0; i < objComponentCount; i++)
    {
        size_t typeID = 0;
        fread(&typeID, sizeof(size_t), 1, pFile);

        // 컴포넌트또한 마찬가지 이유로 이번에는 반환받아온다.
        CComponent* component = CSceneManager::GetInst()->CallCreateComponent(this, typeID);

        // 반환받아 받아온 컴포넌트를 파일에 입력된 데이터를 기준으로 초기화한다.
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
