#include "ClientManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/StartScene.h"
#include "Scene/NatureOfCode.h"
#include "Input.h"
#include "Resource/ResourceManager.h"
#include "Widget/MouseNormal.h"
#include "Widget/MouseAttack.h"
#include "Scene/RCGStartScene.h"
#include "Component/TileMapComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "RCGGameManager.h"
#include "Object/TileMap.h"

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager() :
    m_Manager(nullptr)
{

}

CClientManager::~CClientManager()
{
    SAFE_DELETE(m_Manager);
    CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
    if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"), 1280, 720, IDI_ICON1))
    {
        CEngine::DestroyInst();
        return false;
    }

    // 클라이언트에서 씬을 생성, 오브젝트 생성시 콜백함수를 설정한다.
    CSceneManager::GetInst()->SetCreateSceneModeFunction<CClientManager>(this, &CClientManager::CreateSceneMode);
    CSceneManager::GetInst()->SetCreateObjectFunction<CClientManager>(this, &CClientManager::CreateObject);
    CSceneManager::GetInst()->SetCreateComponentFunction<CClientManager>(this, &CClientManager::CreateComponent);

    m_Manager = new CRCGGameManager();

    CInput::GetInst()->CreateKey("MoveUp", VK_UP);
    CInput::GetInst()->CreateKey("MoveDown", VK_DOWN);
    //CInput::GetInst()->CreateKey("RotationZInv", 'A');
    //CInput::GetInst()->CreateKey("RotationZ", 'D');
    CInput::GetInst()->CreateKey("MoveLeft", VK_LEFT);
    CInput::GetInst()->CreateKey("MoveRight", VK_RIGHT);
    CInput::GetInst()->CreateKey("Jump", VK_SPACE);
    CInput::GetInst()->CreateKey("ResetPos", VK_NUMPAD5);
    //CInput::GetInst()->CreateKey("Attack", VK_SPACE);
    CInput::GetInst()->CreateKey("Spawn", VK_LBUTTON);
    CInput::GetInst()->CreateKey("MoveTo", VK_RBUTTON);
    CInput::GetInst()->CreateKey("Play", 'P');
    CInput::GetInst()->CreateKey("AI", 'W');
    CInput::GetInst()->CreateKey("WeakAttack", 'Z');
    CInput::GetInst()->CreateKey("HeavyAttack", 'X');
    CInput::GetInst()->CreateKey("Special", 'C');
    CInput::GetInst()->CreateKey("Guard", 'A');
    CInput::GetInst()->CreateKey("Debug", 'Q');
    CInput::GetInst()->CreateKey("Enter", VK_RETURN);
    CInput::GetInst()->CreateKey("Exit", VK_ESCAPE);

    CResourceManager::GetInst()->CreateSoundChannelGroup("UI");
    CResourceManager::GetInst()->CreateSoundChannelGroup("BGM");
    CResourceManager::GetInst()->CreateSoundChannelGroup("SFX");

    CResourceManager::GetInst()->SetVolume("SFX", 50);
    CResourceManager::GetInst()->SetVolume("BGM", 100);

    m_Manager->LoadSounds();

    // 마우스 위젯 설정
    // CMouseNormal* mouseNormal = CEngine::GetInst()->CreateMouse<CMouseNormal>(Mouse_State::Normal, "MouseNormal");
    // CMouseAttack* mouseAttack = CEngine::GetInst()->CreateMouse<CMouseAttack>(Mouse_State::State1, "MoustAttack");


    return true;
}

void CClientManager::CreateDefaultSceneMode()
{
    CSceneManager::GetInst()->CreateSceneMode<CRCGStartScene>();
    // CSceneManager::GetInst()->CreateSceneMode<CNatureOfCode>();
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* scene, size_t type)
{
}

CGameObject* CClientManager::CreateObject(CScene* scene, size_t type)
{
    if (type == typeid(CGameObject).hash_code())
    {
        CGameObject* obj = scene->LoadGameObject<CGameObject>();
        
        return obj;
    }
    else if (type == typeid(CTileMap).hash_code())
    {
        CTileMap* obj = scene->LoadGameObject<CTileMap>();

        return obj;
    }

    return nullptr;
}

CComponent* CClientManager::CreateComponent(CGameObject* obj, size_t type)
{
    if (type == typeid(CSceneComponent).hash_code())
    {
        CComponent* component = obj->LoadComponent<CSceneComponent>();

        return component;
    }
    else if (type == typeid(CSpriteComponent).hash_code())
    {
        CComponent* component = obj->LoadComponent<CSpriteComponent>();

        return component;
    }
    else if (type == typeid(CStaticMeshComponent).hash_code())
    {
        CComponent* component = obj->LoadComponent<CStaticMeshComponent>();
        
        return component;
    }
    else if (type == typeid(CTileMapComponent).hash_code())
    {
        CComponent* component = obj->LoadComponent<CTileMapComponent>();

        return component;
    }

    return nullptr;
}
