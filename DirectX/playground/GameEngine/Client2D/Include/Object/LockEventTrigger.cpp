#include "LockEventTrigger.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "RCGPlayer.h"
#include "ChainLockWall.h"
#include "../ClientManager.h"
#include "../RCGGameManager.h"
#include "SchoolBoy.h"
#include "SchoolGirl.h"
#include "MT.h"
#include "Cheerleader.h"

CLockEventTrigger::CLockEventTrigger() :
    m_Trigger(nullptr)
{
    SetTypeID<CLockEventTrigger>();
}

CLockEventTrigger::CLockEventTrigger(const CLockEventTrigger& obj) :
    CGameObject(obj)
{
    m_Trigger = (CColliderBox2D*)FindComponent("LockTrigger");
}

CLockEventTrigger::~CLockEventTrigger()
{
}

bool CLockEventTrigger::Init()
{
    m_Trigger = CreateComponent<CColliderBox2D>("LockTrigger");

    m_Trigger->SetCollisionProfile("SideWall");

    m_Trigger->SetExtent(100.0f, 100.0f);

    m_Trigger->SetPivot(0.5f, 0.5f, 0.0f);

    m_Trigger->AddCollisionCallback(Collision_State::Begin, this, &CLockEventTrigger::OnPlayerTrigger);

    m_EnemiesToSpawn = 0;

    m_TimeToSpawn = 1.5f;

    m_SpawnCnt = 0;

    m_ElapsedTime = 0.0f;

    m_EventOn = false;

    m_EventEnd = false;

    return true;
}

void CLockEventTrigger::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);

    if (m_EventOn)
    {
        if (CClientManager::GetInst()->GetManager()->CanSpawnEnemy())
        {
            m_ElapsedTime += deltaTime;

            if (m_EnemiesToSpawn > m_SpawnCnt && m_ElapsedTime >= m_TimeToSpawn)
            {

                CRCGEnemy* enemy;
                int random = rand() % 4;

                switch (random)
                {
                case 0:
                    enemy = m_Scene->CreateGameObject<CSchoolBoy>("SchoolBoy " + std::to_string(m_SpawnCnt + 1));
                    ((CSchoolBoy*)enemy)->SetPursueState();
                    break;
                case 1:
                    enemy = m_Scene->CreateGameObject<CSchoolGirl>("SchoolGirl " + std::to_string(m_SpawnCnt + 1));
                    ((CSchoolGirl*)enemy)->SetPursueState();
                    break;
                case 2:
                    enemy = m_Scene->CreateGameObject<CMT>("MT " + std::to_string(m_SpawnCnt + 1));
                    ((CMT*)enemy)->SetPursueState();
                    break;
                case 3:
                    enemy = m_Scene->CreateGameObject<CCheerleader>("Cheerleader " + std::to_string(m_SpawnCnt + 1));
                    ((CCheerleader*)enemy)->SetPursueState();
                    break;
                default:
                    enemy = m_Scene->CreateGameObject<CSchoolBoy>("SchoolBoy " + std::to_string(m_SpawnCnt + 1));
                    ((CSchoolBoy*)enemy)->SetPursueState();
                    break;
                }

                Vector3 spawnPos = CClientManager::GetInst()->GetManager()->GetRandomSpanwPoint();
                enemy->SetWorldPos(spawnPos);
                enemy->StartSpawn();
                m_Enemies.push_back(enemy);
                CClientManager::GetInst()->GetManager()->AddEnemy(enemy);

                size_t size = m_vecChainWall.size();

                for (size_t i = 0; i < size; i++)
                {
                    if (m_vecChainWall[i]->GetRootComponent()->TypeCheck<CColliderBox2D>())
                    {
                        enemy->AddWallCollisionCallback((CColliderBox2D*)m_vecChainWall[i]->GetRootComponent());
                    }
                }

                m_SpawnCnt++;
                m_ElapsedTime = 0.0f;
            }
        }

        if (m_SpawnCnt == m_EnemiesToSpawn && CClientManager::GetInst()->GetManager()->GetEnemies().empty())
        {
            m_EventEnd = true;
            m_EventOn = false;

            size_t size = m_vecChainWall.size();

            for (size_t i = 0; i < size; i++)
            {
                m_vecChainWall[i]->StartFadeOut();
            }
        }
    }

    size_t size = m_vecChainWall.size();

    int count = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (m_vecChainWall[i]->FadeOutComplete())
            count++;
    }

    if (count == 4)
    {
        for (size_t i = 0; i < size; i++)
        {
            m_vecChainWall[i]->Destroy();
        }

        CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

        player->GetCameraPoint()->StartFollow();

        m_vecChainWall.clear();
    }
}

void CLockEventTrigger::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CLockEventTrigger* CLockEventTrigger::Clone()
{
    return new CLockEventTrigger(*this);
}

void CLockEventTrigger::OnPlayerTrigger(const sCollisionResult& result)
{
    if (result.dest->GetGameObject()->TypeCheck<CRCGPlayer>())
    {
        sResolution rs = CEngine::GetInst()->GetResolution();
        CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

        CChainLockWall* wall = m_Scene->CreateGameObject<CChainLockWall>("TopWall");

        m_vecChainWall.push_back(wall);

        wall->SetAsTop();

        wall->SetWorldPos(player->GetWorldPos().x, player->GetWorldPos().y + (float)rs.height * 0.4f, 0.0f);

        wall = m_Scene->CreateGameObject<CChainLockWall>("BottomWall");

        m_vecChainWall.push_back(wall);

        wall->SetAsBottom();

        wall->SetWorldPos(player->GetWorldPos().x, player->GetWorldPos().y - (float)rs.height * 0.4f, 0.0f);

        wall = m_Scene->CreateGameObject<CChainLockWall>("LeftWall");

        m_vecChainWall.push_back(wall);

        wall->SetAsLeft();

        wall->SetWorldPos(player->GetWorldPos().x - (float)rs.width * 0.5f, player->GetWorldPos().y, 0.0f);

        wall = m_Scene->CreateGameObject<CChainLockWall>("RightWall");

        m_vecChainWall.push_back(wall);

        wall->SetAsRight();

        wall->SetWorldPos(player->GetWorldPos().x + (float)rs.width * 0.5f, player->GetWorldPos().y, 0.0f);

        size_t size = m_vecChainWall.size();

        for (size_t i = 0; i < size; i++)
        {
            m_vecChainWall[i]->StartFadeIn();
        }

        player->GetCameraPoint()->StopFollow();

        m_EnemiesToSpawn = rand() % 4 + 2;

        m_EventOn = true;

        CClientManager::GetInst()->GetManager()->SetEventTrigger(this);

        const std::list<CRCGEnemy*> enemies = CClientManager::GetInst()->GetManager()->GetEnemies();

        for (const auto& enemy : enemies)
        {
            size_t size = m_vecChainWall.size();

            for (size_t i = 0; i < size; i++)
            {
                if (m_vecChainWall[i]->GetRootComponent()->TypeCheck<CColliderBox2D>())
                {
                    enemy->AddWallCollisionCallback((CColliderBox2D*)m_vecChainWall[i]->GetRootComponent());
                }
            }
        }

        m_Trigger->Enable(false);
    }
}
