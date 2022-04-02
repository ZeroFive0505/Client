#include "RaceScene.h"
#include "SceneResource.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Object/Player.h"
#include "../Input.h"
#include "../Math.h"
#include "../Timer.h"
#include "../Scene/CollisionHash.h"
#include "../Object/Pipe.h"
#include "../Object/ItemBox.h"
#include "../Object/Coin.h"
#include "../Map/TileMap.h"
#include "../UI/RaceGUI.h"
#include "../Object/RampHorizontal.h"
#include "../Object/RampVertical.h"
#include "../Object/OilSlick.h"
#include "../Object/CoinEffect.h"
#include "../Object/CollisionEffect.h"
#include "../Object/Lakitu.h"
#include "../AI/AIGradientDescent.h"
#include "../Scene/SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../Object/Thwomp.h"
#include "../Object/Flower.h"


CRaceScene::CRaceScene() :
	m_CircuitTexture(nullptr),
	m_OriginalCircuit(nullptr),
	m_EdgeTexture(nullptr),
	m_SkybackTexture(nullptr),
	m_SkyFrontTexture(nullptr),
	m_GreenShell(nullptr),
	m_RedShell(nullptr),
	m_Banana(nullptr),
	m_DriverArray{},
	m_Player(nullptr)
{
	m_CurrentRaceTime = 0.0f;
	m_ItemUseAt = 0.0f;
	m_PlayerAccelTime = 0.0f;
	m_WaitTime = 0.0f;
	m_RaceFinished = false;
	m_TimeAfterRaceFinish = 0.0f;
	m_IntroTime = 0.0f;
	m_Intro = true;
	m_LookBack = false;
	m_MainBgmStart = false;
}

CRaceScene::~CRaceScene()
{
	SAFE_DELETE(m_Lakitu);
}

bool CRaceScene::Init()
{
	// 보이지 않는 플레이어를 만들어낸다.
	// 처음 인트로와 게임이 끝나고 나서 이용한다.
	m_InvisiblePlayer = new CDriver("Invisible", this, Vector2(0.0f, 0.0f), 0.0f, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, EDriverControlType::DISABLED, CKartProperties::INVISIBLE,
		EMenuPlayer::MARIO, true);

	GetSceneResoruce()->SoundStop("player_circuit");

	LoadSound();

	LoadItem();

	// 김수한무를 초기화한다.
	m_Lakitu = new CLakitu;
	m_Lakitu->Init(this);


	return true;
}

bool CRaceScene::Render(HDC hDC)
{
	// 비율을 구한다.
	const float NORMAL_WIDTH = 512.0f;
	const float SCALE = CGameManager::GetInst()->GetResolution().width / NORMAL_WIDTH;

	// 화면에 그려질 텍스쳐들
	CTexture circuitTexture, skyFrontTexture, skyBackTexture, miniMap;

	// 만약 인트로와 레이스가 끝나지 않았다면
	// 현재 플레이어 기준으로 화면을 그려낸다.
	if (!m_RaceFinished && !m_Intro)
		SetSkyTexture(m_Player, skyBackTexture, skyFrontTexture);
	else
		SetSkyTexture(m_InvisiblePlayer, skyBackTexture, skyFrontTexture);

	// 위와 마찬가지
	if (!m_RaceFinished && !m_Intro)
		SetCircuitTexture(m_Player, circuitTexture);
	else
		SetCircuitTexture(m_InvisiblePlayer, circuitTexture);

	// 미니맵을 갱신해서 그린다.
	SetMiniMapTexture(miniMap);


	Resolution rs = CGameManager::GetInst()->GetResolution();
	
	// 현재화면의 넓이와 텍스쳐의 비율로 스케일을 설정한다.
	float backFactor = rs.width / (float)skyBackTexture.GetWidth();
	float frontFactor = rs.width / (float)skyFrontTexture.GetWidth();
	skyBackTexture.SetScale(backFactor, backFactor);
	skyFrontTexture.SetScale(frontFactor, frontFactor);

	// 먼저 최상단부터 하늘을 그려낸다.
	float currentHeight = 0;
	skyBackTexture.SetColorKey(255, 0, 255);
	skyFrontTexture.SetColorKey(255, 0, 255);
	skyBackTexture.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)skyBackTexture.GetWidth(), (float)skyBackTexture.GetHeight()));
	skyFrontTexture.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)skyFrontTexture.GetWidth(), (float)skyFrontTexture.GetHeight()));

	// 그 비율만큼 내려와 서킷을 그려낸다.
	currentHeight += rs.height * SKY_HEIGHT_PCT;
	circuitTexture.SetColorKey(255, 0, 255);
	circuitTexture.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)circuitTexture.GetWidth(), (float)circuitTexture.GetHeight()));

	// 화면에 그려내질 오브젝트들을 담을 벡터
	std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>> wallObjects;

	// 만약 게임이 끝나거나 인트로중이라면 
	// 보이지 않는 플레이어 기준으로 그려낸다.
	if (!m_RaceFinished && !m_Intro)
		RenderableWallObjects(SCALE, m_Player, wallObjects);
	else
		RenderableWallObjects(SCALE, m_InvisiblePlayer, wallObjects);

	if (!m_RaceFinished && !m_Intro)
		RenderableItems(SCALE, m_Player, wallObjects);
	else
		RenderableItems(SCALE, m_InvisiblePlayer, wallObjects);

	if (!m_RaceFinished && !m_Intro)
		RenderableDrivers(SCALE, m_Player, wallObjects);
	else
		RenderableDrivers(SCALE, m_InvisiblePlayer, wallObjects);

	// 만약 플레이어가 뒤를 돌아본다면 정면을 화면에 그려낸다.
	if (!m_RaceFinished && !m_Intro && !m_LookBack)
		m_Player->Render(SCALE, wallObjects);
	else if (m_LookBack)
		m_Player->RenderFront(SCALE, wallObjects);

	// z값을 기준으로 화면에 그려낸다.
	std::sort(wallObjects.begin(), wallObjects.end(),
		[](const std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>& lhs,
			const std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>& rhs)
	{
		return lhs.first.first > rhs.first.first;
	});


	// 현재 플레이어와 그려낼 대상의 거리를 계산하여 사이즈를 조절
	Vector2 playerPos;
	if (!m_RaceFinished)
		playerPos = Vector2(m_Player->GetPos().x * MAP_ASSETS_WIDTH, m_Player->GetPos().y * MAP_ASSETS_HEIGHT);
	else
		playerPos = Vector2(m_InvisiblePlayer->GetPos().x * MAP_ASSETS_WIDTH, m_InvisiblePlayer->GetPos().y * MAP_ASSETS_HEIGHT);

	// 모든 오브젝트들을 그려낸다.
	for (const auto& kv : wallObjects)
	{
		if (kv.second == m_Player && !m_RaceFinished)
		{
			// 플레이어의 경우 그냥 그려낸다.
			kv.first.second->Render(hDC, Vector2(0.0f, 0.0f), Vector2((float)kv.first.second->GetWidth(), (float)kv.first.second->GetHeight()));
		}
		else
		{
			// 아닐 경우 거리에 따라 적절한 비율로 조절된다.
			Vector2 otherPos = Vector2(kv.second->GetPos().x * MAP_ASSETS_WIDTH, kv.second->GetPos().y * MAP_ASSETS_HEIGHT);

			float dist = Distance(playerPos, otherPos);

			float distFactor = dist / 64.0f;

			kv.first.second->RenderByFactor(hDC, Vector2(kv.first.second->GetPos().x, kv.first.second->GetPos().y),
				Vector2(0.0f, 0.0f), Vector2((float)kv.first.second->GetWidth(),
					(float)kv.first.second->GetHeight()), distFactor);
		}
	}


	// 만약 플레이어가 지면에 닿아있고 속도가 일정 수준 이상이라면 파티클을 그려낸다.
	if (m_Player->GetHeight() <= 0.0f &&
		m_Player->m_SpeedForward > m_Player->m_Kart->m_MaxNormalLinearSpeed / 4 && !m_RaceFinished)
	{
		bool IsSmall = m_Player->m_Animator.GetSmallTime() > 0 || m_Player->m_Animator.GetSmashTime() > 0;

		m_Player->m_Animator.DrawParticles(hDC, m_Player->GetTexture(), IsSmall, m_Player->GetPos());
	}

	// 서킷 비율만큼 내려와 미니맵을 그린다.
	currentHeight += rs.height * CIRCUIT_HEIGHT_PCT;

	miniMap.SetColorKey(255, 0, 255);
	miniMap.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)miniMap.GetWidth(), (float)miniMap.GetHeight()));


	// 미니맵의 경우 y 좌표로 정렬해서 그린다.
	std::sort(m_DriverArray, m_DriverArray + (int)EMenuPlayer::END, [](const CSharedPtr<CDriver>& lhs, const CSharedPtr<CDriver>& rhs)
	{
		return lhs->GetPos().y < rhs->GetPos().y;
	});

	// 미니맵에 그려낼 캐릭터들
	for (const auto& driver : m_DriverArray)
	{
		// 약 보이지 않고 플레이거 아니라면 그려내지 않는다.
		if (!driver->IsVisible() && driver != m_Player)
			continue;

		// 텍스쳐를 각도에 맞게 가져온다.
		CTexture miniDriver = driver->m_Animator.GetMinimapTexture(driver->GetAngle() + driver->m_SpeedTurn * 0.2f, SCALE);
		Vector2 mapPosition = MinimapToScreen(driver->GetPos());

		miniDriver.SetColorKey(255, 0, 255);
		miniDriver.SetPivot(miniDriver.GetWidth() / 2.0f, miniDriver.GetHeight() * 0.9f);
		miniDriver.SetPos(mapPosition.x * rs.width, mapPosition.y * rs.height);
		miniDriver.Scale(0.5f, 0.5f);
		miniDriver.Render(hDC, Vector2(miniDriver.GetPos().x, miniDriver.GetPos().y), Vector2(0.0f, 0.0f), Vector2((float)miniDriver.GetWidth(), (float)miniDriver.GetHeight()));
	}


	// UI를 렌더링한다.
	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j];
				}

				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			m_UIArray[i]->Render(hDC);
			i++;
		}
	}

	m_Lakitu->Render(hDC);

	return false;
}

bool CRaceScene::Update(float deltaTime)
{
	if (!m_Start)
	{
		m_Start = true;

		// 게임이 시잘될때 키셋을 설정하고 콜백함수를 묶는다.
		CInput::GetInst()->SetPlayModeKeySet();

		CInput::GetInst()->SetCallback<CRaceScene>("ItemUseForward", KeyState_Down, this, &CRaceScene::UseItemForward);
		CInput::GetInst()->SetCallback<CRaceScene>("ItemUseBackward", KeyState_Down, this, &CRaceScene::UseItemBackWard);
		CInput::GetInst()->SetCallback<CRaceScene>("BackToTitle", KeyState_Down, this, &CRaceScene::BackToTitle);
		CInput::GetInst()->SetCallback<CRaceScene>("Feather", KeyState_Down, this, &CRaceScene::GetFeather);
		GetSceneResoruce()->SoundPlay("Fanfare");
	}

	// 인트로 중일떄
	if (m_Intro)
	{
		// 인트로 시간을 누적시키고
		m_IntroTime += deltaTime;

		// 아직 충분히 시간이 되지 않았다면
		if (m_IntroTime < 1.5f && !m_RaceStart)
		{
			float d = (m_IntroTime) / 1.5f;
			Vector2 pos = m_Player->GetPos();
			// 보이지 않는 캐릭터를 현재 플레이어의 위치에서 더한 곳에서 시작한다.
			m_InvisiblePlayer->SetPos(pos + Vector2(0.0f, ANIMATION_FORWARD_DIST * -1.0f) * (1.0f - d));
			// 보이지 않는 플레이어의 경우 현재 플레이어의 각도 -M_PI_2에서 180도 시계방향으로 돌린 위치에서 바라봐야함
			m_InvisiblePlayer->SetAngle((float)M_PI_2);
		}
		// 만약 충분한시간이 지났다면
		else if (m_IntroTime > 1.5f && !m_RaceStart)
		{
			// 보이지 않는 플레이어의 위치를 현재 플레이어의 위치로 바꾼다.
			m_InvisiblePlayer->SetPos(m_Player->GetPos());
			// 일단 각도가 플레이어가 현재 바라보고 있는 각도라 설정한다.
			m_InvisiblePlayer->SetAngle((float)-M_PI_2);

			if (m_IntroTime < 3.0f)
			{
				// 만약 충분한 시간이 지나지 않았다면 서서히 돌아야한다.
				float d = (m_IntroTime - 1.5f) / (3.0f - 1.5f);
				// 만약 3처에 가까운 시간이 됬다면 d의 값은 1에 가까워지고 M_PI * -0.5f가 된다 따라서 현재 플레이어가 바라보고 있는 각도가 된다.
				m_InvisiblePlayer->SetAngle((float)M_PI * (0.5f - d));
			}
			// 만약 아직 김수한무가 시작 카운트를 세지 않았다면
			else if (m_Lakitu->IsSleeping())
			{
				// 이제 인트로는 끝이난다.
				m_Intro = false;
				// 키 할당
				CInput::GetInst()->SetCallback<CRaceScene>("LookBack", KeyState_Hold, this, &CRaceScene::LookBack);
				CInput::GetInst()->SetCallback<CRaceScene>("LookBack", KeyState_Up, this, &CRaceScene::LookForward);
				CInput::GetInst()->SetCallback<CRaceScene>("ToggleAuto", KeyState_Down, this, &CRaceScene::ToggleAutoDriver);
				// 엔진 소리 시작
				GetSceneResoruce()->PlayEngine("Engine", true);
				GetSceneResoruce()->UpdateEngineSound("Engine", m_Player, m_Player->GetPos(),
					m_Player->GetHeight(), m_Player->m_SpeedForward, m_Player->m_SpeedTurn);
				GetSceneResoruce()->UpdateListener(m_Player->GetPos(), m_Player->GetAngle(), m_Player->GetHeight());
				// 김수한무 카운트 시작
				m_Lakitu->ShowRaceStart();
			}
		}
	}


	// 레이스가 시작된 후
	if (m_RaceStart && !m_RaceFinished)
	{
		// 만약 메인 bgm이 시작되지 않았다면 재생
		if (!m_MainBgmStart)
		{
			m_MainBgmStart = true;
			GetSceneResoruce()->SoundPlay("Main");
		}

		// 레이스 시간 누적
		m_CurrentRaceTime += deltaTime;

		// 현재 플레이어가 뒤를 돌아볼 시에는
		if (m_LookBack)
		{
			// 특정위치 만큼 앞으로 움직이고
			m_InvisiblePlayer->SetPos(m_Player->GetPos() + Vector2(cosf(m_Player->GetAngle()), sinf(m_Player->GetAngle())) * 0.03f);
			// 각도를 180도 틀어서 반대 방향을 보게 한다.
			m_InvisiblePlayer->SetAngle(m_Player->GetAngle() - (float)M_PI);
		}

		// 모든 캐릭터를 순회하면서 갱신
		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			m_DriverArray[i]->Update(deltaTime);
		}

		GetSceneResoruce()->UpdateEngineSound("Engine", m_Player, m_Player->GetPos(),
			m_Player->GetHeight(), m_Player->m_SpeedForward, m_Player->m_SpeedTurn);
		GetSceneResoruce()->UpdateListener(m_Player->GetPos(), m_Player->GetAngle(), m_Player->GetHeight());

		// CPU 아이템 사용
		if (m_CurrentRaceTime > m_ItemUseAt)
		{
			// 만약 아이템을 썼다면 다음 아이템은 충분한 시간이 흐른 후에 쓴다.
			m_ItemUseAt = m_CurrentRaceTime + TIME_BETWEEN_ITEM_CHECKS;


			for (const auto& driver : m_DriverArray)
			{
				// 플레이어가 아닌 CPU의 경우
				if (driver != m_Player && m_Player->GetItem() != EItem::NONE)
				{
					// 확률을 구해 아이템을 사용한다.
					float r = rand() / (float)RAND_MAX;

					// 아이템 사용확률을 반환받고
					std::pair<float, bool> prob = CItem::GetItemUseProb(driver, m_Positions);

					// 조건을 만족한다면 아이템을 사용한다.
					if (r < prob.first / driver->m_ItemProbModifier)
					{
						CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");
						CItem::UseItem(driver, m_Positions, prob.second, this, gui);
					}
				}
			}
		}

		// 순위를 정렬한다.
		auto iter = m_Positions.begin();

		// 레이스를 끝낸 유저는 정렬할 필요가 없으니 넘어간다.
		while (iter != m_Positions.end() && (*iter)->GetLaps() > MAX_LAPS)
		{
			iter++;
		}

		// 현재 반복자 위치에서부터 정렬한다.
		std::sort(iter, m_Positions.end(), []
		(const CDriver* lhs, const CDriver* rhs) {
			// 만약 같은 랩이라면
			if (lhs->GetLaps() == rhs->GetLaps())
			{
				// 변화량이 더 큰 쪽이 순위가 더 높아야한다.
				return lhs->GetLastGradient() < rhs->GetLastGradient();
			}
			// 아니라면 랩 수로 정렬
			else
			{
				return lhs->GetLaps() > rhs->GetLaps();
			}
		});

		// 순위를 순회하면서 순위를 설정한다.
		for (size_t i = 0; i < m_Positions.size(); i++)
		{
			m_Positions[i]->m_Rank = i;

			if (m_Positions[i] == m_Player)
			{
				CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");
				gui->SetRank(i + 1);
			}
		}

		// 벽 오브젝트 갱신
		for (const auto& obj : m_WallObjList)
			obj->Update(deltaTime);

		// 아이템 갱신
		for (const auto& item : m_ItemList)
			item->Update(deltaTime);

		// 바닥 오브젝트 갱신
		for (const auto& floor : m_SpecialFloor)
			floor->Update(deltaTime);

		// 만약 아이템 목록중에 다 썼거나 존재하지 않는 아이템이 존재한다면 제거한다.
		m_ItemList.erase(std::remove_if(m_ItemList.begin(), m_ItemList.end(), [](const CSharedPtr<CItem>& item) {return !item ||
			item->GetUsed(); }), m_ItemList.end());

		// 충돌테스트
		CCollisionHash::GetInst()->ResetDynamic();

		// 아이템은 동적 충돌체 목록에 등록한다.
		for (const auto& item : m_ItemList)
		{
			CCollisionHash::GetInst()->RegisterDynamic((CWallObject*)item);
		}

		// 캐릭터도 마찬가지
		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			CCollisionHash::GetInst()->RegisterDriver(m_DriverArray[i]);
		}

		CollisionData data;

		// 모든 캐릭터를 순회하면서 만약 충돌이 일어났다면
		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			if (CCollisionHash::GetInst()->Collide(m_DriverArray[i], data))
			{
				// 모멘텀과 속도 회전값을 구한다.
				m_DriverArray[i]->m_CollisionMomentum = data.m_Momentum;
				m_DriverArray[i]->m_SpeedForward *= data.m_SpeedFactor;
				m_DriverArray[i]->m_SpeedTurn *= data.m_SpeedFactor;

				// 충돌의 타입에 따라 정의한다.
				switch (data.type)
				{
				case ECollisionType::HIT:
					m_DriverArray[i]->ApplyHit();
					break;
				case ECollisionType::SMASH:
					m_DriverArray[i]->ApplySmash();
					break;
				default:
					// 만약 캐릭터간의 충돌일시
					if (!m_DriverArray[i]->IsImmune())
					{
						m_DriverArray[i]->AddCoin(-1);
					}

					if (m_DriverArray[i] == m_Player && m_DriverArray[i]->CanDrive() &&
						m_DriverArray[i]->m_SpeedForward != 0.0f)
					{
						// 충돌 사운드 재생.
						AddCollisionEffect(m_Player->GetPos());
						GetSceneResoruce()->SoundPlay("Crash");
					}

					// 충돌 이펙트 생성
					AddCollisionEffect(m_DriverArray[i]->GetPos());
					break;
				}
			}
		}


		// 만약 바닥 오브젝트가 새롭게 갱신됬을 시
		bool hasChanged = CFloorObject::ApplyAllChanges();

		if (hasChanged)
		{

			if (m_FloorObjectUpdated)
			{
				{
					// 기존의 텍스쳐를 복사해온다. (바닥 오브젝트가 아무것도 안 그려진)
					m_CircuitTexture = m_OriginalCircuit->Clone();
					// 바닥오브젝트들을 순회한다.
					auto iter = m_vecFloor.begin();
					auto iterEnd = m_vecFloor.end();

					// 모든 바닥 오브젝트들을 순회하면서 
					for (; iter != iterEnd; iter++)
					{
						// 위치와 크기를 구하고
						Vector2 pos = (*iter)->GetPos();
						Vector2 size = (*iter)->GetSize();
						CTexture* texture = (*iter)->GetCurrentTexture();

						// 텍스쳐를 그려낸다.
						if (texture != nullptr)
						{
							// 화면에 그리는 것이아니라 복사된 비트맵 위에다 그려낸다.
							TransparentBlt(m_CircuitTexture->GetTextureDC(), (int)pos.x, (int)pos.y, (int)size.x, (int)size.y,
								texture->GetTextureDC(), 0, 0, texture->GetWidth(), texture->GetHeight(), texture->GetColorKey());

							// 그리고 삭제한다.
							SAFE_RELEASE(texture);
						}
					}

					m_FloorObjectUpdated = false;
				}

				// 다시 색 배열을 설정
				m_CircuitTexture->SetPixelArray();
			}
		}


		// UI를 그려낸다.
		{
			for (int i = 0; i < m_UICount;)
			{
				if (!m_UIArray[i]->IsActive())
				{
					m_UICount--;

					for (int j = i; j < m_UICount; j++)
					{
						m_UIArray[j] = m_UIArray[j];
					}

					continue;
				}
				else if (!m_UIArray[i]->GetVisibility())
				{
					i++;
					continue;
				}

				m_UIArray[i]->Update(deltaTime);
				i++;
			}
		}

	}
	// 레이스가 끝났을 시
	else if (m_RaceFinished)
	{
		if (m_LookBack)
			m_LookBack = false;


		m_TimeAfterRaceFinish += deltaTime;

		m_CurrentRaceTime += deltaTime;

		for (const auto& obj : m_WallObjList)
			obj->Update(deltaTime);

		for (const auto& item : m_ItemList)
			item->Update(deltaTime);

		for (const auto& floor : m_SpecialFloor)
			floor->Update(deltaTime);

		m_ItemList.erase(std::remove_if(m_ItemList.begin(), m_ItemList.end(), [](const CSharedPtr<CItem>& item) {return !item ||
			item->GetUsed(); }), m_ItemList.end());

		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			m_DriverArray[i]->Update(deltaTime);
		}


		// 서서히 돈다.
		float turnPct = std::fminf(m_TimeAfterRaceFinish / ANIMATION_TURN_TIME, 1.0f);
		// turnPct는 시간이 지남에 따라 1.0이 될 것이다.
		// 따라서 아래의 각도 값은 최종적으로 180가 될 것이다.
		float animTurnAngle = turnPct * (float)M_PI;
		// 아래의 값도 최종적으로 1.0이 될 것이다.
		float zoomOutPct = std::fminf(m_TimeAfterRaceFinish / ANIMATION_ZOOM_OUT_TIME, 1.0f);

		// 보이지 않는 플레이어의 위치는 현재 플레이어의 위치에서 zoomOutPct의 값에 비례해서 전방에 위치하게 된다.
		m_InvisiblePlayer->SetPos(m_Player->GetPos() + Vector2(cosf(m_Player->GetAngle()), sinf(m_Player->GetAngle())) * 0.04f * zoomOutPct);

		// 각도는 플레이어가 바라보고있는 각도에서 +180도
		m_InvisiblePlayer->SetAngle(m_Player->GetAngle() + animTurnAngle);


		// 순위를 정렬한다.
		auto iter = m_Positions.begin();

		// 레이스를 끝낸 유저는 정렬할 필요가 없으니 넘어간다.
		while (iter != m_Positions.end() && (*iter)->GetLaps() > MAX_LAPS)
		{
			iter++;
		}

		// 현재 반복자 위치에서부터 정렬한다.
		std::sort(iter, m_Positions.end(), []
		(const CDriver* lhs, const CDriver* rhs) {
			// 만약 같은 랩이라면
			if (lhs->GetLaps() == rhs->GetLaps())
			{
				// 변화량이 더 큰 쪽이 순위가 더 높아야한다.
				return lhs->GetLastGradient() < rhs->GetLastGradient();
			}
			// 아니라면 랩 수로 정렬
			else
			{
				return lhs->GetLaps() > rhs->GetLaps();
			}
		});

		m_Lakitu->Update(deltaTime);

	}
	else
	{
		// 아직 레이스가 시작안했고 카운트만 시작했을 경우에는
		if (!m_RaceStart && m_Lakitu->HasRaceStarted())
		{
			m_RaceStart = true;
			CInput::GetInst()->SetCallback<CRaceScene>("Drift", KeyState_Down, this, &CRaceScene::DriftStart);
			for (int i = 0; i < (int)EMenuPlayer::END; i++)
			{
				// CPU들중에 몇몇은 정상적으로 출발하고
				// 그러지 못하는 CPU를 설정한다.
				if (m_DriverArray[i] != m_Player)
				{
					float speedPercent = ((75 + rand() % 25) / 100.0f);
					float turnPercent = ((rand() % 15) / 100.0f);

					if (speedPercent <= 0.95f)
					{
						m_DriverArray[i]->m_SpeedForward = m_DriverArray[i]->m_Kart->m_MaxNormalLinearSpeed * speedPercent;
						m_DriverArray[i]->m_SpeedTurn = m_DriverArray[i]->m_Kart->m_MaxTurningAngularSpeed * turnPercent;
					}
					else
						m_DriverArray[i]->ApplyHit();
				}
				else
				{
					// 만약 플레이어가 적당한 시간만큼 악셀을 했다면 시작 부스터
					if (1.0f <= m_PlayerAccelTime && m_PlayerAccelTime < 1.5f)
					{
						m_Player->m_SpeedForward = m_Player->m_Kart->m_MaxSpeedUpLinearSpeed;
						GetSceneResoruce()->SoundPlay("Mushroom");
					}
					else if (m_PlayerAccelTime >= 1.5f)
						m_Player->ApplyHit();
				}
			}
		}


		// 악셀 시간을 누적한다.
		if (CInput::GetInst()->GetKeyState('X', EKeyState::KeyState_Hold))
			m_PlayerAccelTime += deltaTime;
		else
		{
			m_PlayerAccelTime -= deltaTime;
			if (m_PlayerAccelTime < 0.0f)
				m_PlayerAccelTime = 0.0f;
		}
	}

	// 김수한무 갱신
	m_Lakitu->Update(deltaTime);


	// 만약 한명의 드라이버를 제외하고 모든 드라이버가 골에 들어왔다면
	// 기다리는 시간을 잰다.
	if (m_WaitTime == 0.0f && m_Positions[m_Positions.size() - 2]->GetLaps() > MAX_LAPS)
	{
		m_WaitTime = m_CurrentRaceTime + 10.0f;
	}

	// 만약 플레이어가 다 돌지 못핬거나, 현재 레이스 시간이 누적된 기다리는 시간보다 길거나
	// 플레이어가 아직 운전이 가능한 상태라고 레이스가 끝나지 않았다면
	if ((m_Player->GetLaps() > MAX_LAPS ||
		(m_WaitTime != 0.0f && m_CurrentRaceTime > m_WaitTime &&
			m_Player->CanDrive() && m_Player->GetHeight() <= 0.0f)) && !m_RaceFinished)
	{
		// 레이스 종료
		m_RaceFinished = true;

		// 충돌체를 리스트를 전부 비운다.
		CCollisionHash::GetInst()->ResetStatic();
		CCollisionHash::GetInst()->ResetDynamic();

		// 소리 정지
		// 소리 조절
		GetSceneResoruce()->StopAllMusic();
		GetSceneResoruce()->SoundStop("Engine");
		GetSceneResoruce()->SoundPlay("Goal");
		// 일정 순위 이내라면
		if (m_Player->GetRank() <= 3)
		{
			GetSceneResoruce()->SoundPlay("Win");
		}
		else
		{
			GetSceneResoruce()->SoundPlay("Lose");
		}

		for (const auto& driver : m_DriverArray)
			driver->Reset();

		CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");

		gui->RaceFinished();

		// 김수한무 종료 애니메이션
		m_Lakitu->ShowRaceFinish();


		// 플레이어를 자동 AI모드로 변경
		m_Player->m_ControlType = EDriverControlType::AI_GRADIENT;
	}

	return false;
}

// 현재 씬에 코인 이펙트를 생성한다.
void CRaceScene::AddCoinEffect(const CDriver* driver, int amount, bool positive)
{
	float delay = 0.0f;

	for (int i = 0; i < amount; i++)
	{
		// 이펙트를 생성할때 약간의 딜레이를 준다.
		AddItem(new CCoinEffect(driver, this, "Coin Effect", delay, positive));

		// 코인이 늘어날 경우 0.3초
		if (positive)
		{
			delay += 0.3f;
		}
		// 줄어들 경우는 0.1초
		else
		{
			delay += 0.1f;
		}
	}
}

// 충돌효과도 아이템의 일종으로 추가된다.
void CRaceScene::AddCollisionEffect(const Vector2& pos)
{
	AddItem(new CCollisionEffect(pos, this, "Collision Effect"));
}

// 서킷 초기화 코드 외부에서 씬을 전환하고 이 코드를 호출한다.
bool CRaceScene::CircuitInit(ECCOption cc, ECupSelect cup, const std::string& course, EMenuPlayer player)
{
	std::string path = "Course/";

	switch (cup)
	{
	case ECupSelect::MUSHROOM:
		path += "Mushroom/";
		break;
	case ECupSelect::FLOWER:
		path += "Flower/";
		break;
	case ECupSelect::STAR:
		path += "Star/";
		break;
	case ECupSelect::SEPCIAL:
		path += "Special/";
		break;
	}

	path += course + "/";

	{
		std::string bgmPath = "bgm/";
		std::string finalLapPath = "bgm/";

		switch (cup)
		{
		case ECupSelect::MUSHROOM:
			bgmPath += "Mushroom/";
			finalLapPath += "Mushroom/";
			break;
		case ECupSelect::FLOWER:
			bgmPath += "Flower/";
			finalLapPath += "Flower/";
			break;
		case ECupSelect::STAR:
			bgmPath += "Star/";
			finalLapPath += "Star/";
			break;
		case ECupSelect::SEPCIAL:
			bgmPath += "Special/";
			finalLapPath += "Special/";
			break;
		}


		bgmPath += course + "/";
		finalLapPath += course + "/";

		finalLapPath += "final_lap.ogg";
		bgmPath += "music.ogg";

		GetSceneResoruce()->LoadSound("BGM", true, "Main", bgmPath.c_str());
		GetSceneResoruce()->LoadSound("BGM", true, "FinalLap", finalLapPath.c_str());
	}

	{
		std::wstring temp;
		temp.assign(path.begin(), path.end());
		temp += L"circuit.bmp";
		GetSceneResoruce()->LoadTexture("Circuit", temp.c_str());
		m_OriginalCircuit = GetSceneResoruce()->FindTexture("Circuit");
		m_OriginalCircuit->SetName("Circuit");
	}

	{
		std::wstring temp;
		temp.assign(path.begin(), path.end());
		temp += L"skyfront.bmp";
		GetSceneResoruce()->LoadTexture("SkyFront", temp.c_str());
		m_SkyFrontTexture = GetSceneResoruce()->FindTexture("SkyFront");
		m_SkyFrontTexture->SetPixelArray();
		m_SkyFrontTexture->SetName("SkyFront");
	}

	{
		std::wstring temp;
		temp.assign(path.begin(), path.end());
		temp += L"skyback.bmp";
		GetSceneResoruce()->LoadTexture("SkyBack", temp.c_str());
		m_SkybackTexture = GetSceneResoruce()->FindTexture("SkyBack");
		m_SkybackTexture->SetPixelArray();
		m_SkybackTexture->SetName("SkyBack");
	}

	{
		std::wstring temp;
		temp.assign(path.begin(), path.end());
		temp += L"edge.bmp";
		GetSceneResoruce()->LoadTexture("Edge", temp.c_str());
		m_EdgeTexture = GetSceneResoruce()->FindTexture("Edge");
		m_EdgeTexture->SetPixelArray();
		m_EdgeTexture->SetName("Edge");
	}

	switch (cc)
	{
	case ECCOption::CC50:
	{
		float speedMultiplier = 1.75f;
		float playerMultiplier = 1.15f;
		CKartProperties::SetScaleFactor(speedMultiplier, playerMultiplier);
	}
	break;
	case ECCOption::CC100:
	{
		float speedMultiplier = 2.1f;
		float playerMultiplier = 1.09f;
		CKartProperties::SetScaleFactor(speedMultiplier, playerMultiplier);
	}
	break;
	case ECCOption::CC150:
	{
		float speedMultiplier = 2.5f;
		float playerMultiplier = 1.04f;
		CKartProperties::SetScaleFactor(speedMultiplier, playerMultiplier);
	}
	break;
	case ECCOption::CC200:
	{
		float speedMultiplier = 2.8f;
		float playerMultiplier = 1.0f;
		CKartProperties::SetScaleFactor(speedMultiplier, playerMultiplier);
	}
	break;
	}

	// CPU의 경우 다양한 초기값을 가지도록 섞는다.
	unsigned int modifiersIndexer[(unsigned int)EMenuPlayer::END] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	std::shuffle(modifiersIndexer, modifiersIndexer + 8, randGen);

	for (int i = 0; i < (int)EMenuPlayer::END; i++)
	{
		CDriver* driver = new CDriver(DRIVER_ASSET_NAMES[i], this, Vector2(0.0f, 0.0f), (float)M_PI_2 * -1.0f, MAP_ASSETS_WIDTH,
			MAP_ASSETS_HEIGHT, EDriverControlType::AI_GRADIENT, *DRIVER_PROPERTIES[i], EMenuPlayer(i),
			m_Positions, true, FAR_VISIONS[(int)cc][modifiersIndexer[i]],
			ITEM_PROB_MODS[(int)cc][modifiersIndexer[i]], IMPEDIMENTS[(int)cc][modifiersIndexer[i]]);
		driver->SetCCOption(cc);
		m_DriverArray[i] = driver;
		m_Positions[i] = driver;
	}


	switch (cup)
	{
	case ECupSelect::MUSHROOM:
		path = "Mushroom/";
		break;
	case ECupSelect::FLOWER:
		path = "Flower/";
		break;
	case ECupSelect::STAR:
		path = "Star/";
		break;
	case ECupSelect::SEPCIAL:
		path = "Special/";
		break;
	}

	path += course + "/";

	CTileMap* tileMap = CreateMap<CTileMap>("TileMap");

	std::string wallPath;

	wallPath = path;
	wallPath += "walls.txt";

	tileMap->LoadWallFile(wallPath.c_str());

	std::string floorPath;

	floorPath = path;
	floorPath += "floors.txt";

	tileMap->LoadFloorfile(floorPath.c_str());

	std::string tilePath;

	tilePath = path;

	tilePath += "tile.map";

	tileMap->LoadFile(tilePath.c_str());

	std::list<std::pair<EWallObjectType, Vector2>> wallObjs = tileMap->GetWallObjects();

	{
		auto iter = wallObjs.begin();
		auto iterEnd = wallObjs.end();

		for (; iter != iterEnd; iter++)
		{
			switch (iter->first)
			{
			case EWallObjectType::GREEN_PIPE:
			{
				CPipe* greenPipe = new CPipe(iter->second, iter->first, this, "Green Pipe");
				m_WallObjList.push_back(greenPipe);
			}
			break;
			case EWallObjectType::ORNAGE_PIPE:
			{
				CPipe* orangePipe = new CPipe(iter->second, iter->first, this, "Orange Pipe");
				m_WallObjList.push_back(orangePipe);
			}
			break;
			case EWallObjectType::THWOMP:
			{
				CThwomp* thwomp = new CThwomp(iter->second, iter->first, this, "Thwomp");
				m_WallObjList.push_back(thwomp);
			}
				break;
			case EWallObjectType::SUPER_THWOMP:
			{
				CThwomp* thwomp = new CThwomp(iter->second, iter->first, this, "Super Thwomp");
				m_WallObjList.push_back(thwomp);
			}
				break;
			case EWallObjectType::FLOWER:
			{
				CFlower* flower = new CFlower(iter->second, iter->first, this, "Flower");
				m_WallObjList.push_back(flower);
			}
				break;
			}
		}
	}

	std::list<std::pair<std::pair<EFloorObjectType, Vector2>, EFloorObjectOrientation>> floorObjs = tileMap->GetFloorObjects();

	{
		auto iter = floorObjs.begin();
		auto iterEnd = floorObjs.end();

		for (; iter != iterEnd; iter++)
		{
			switch (iter->first.first)
			{
			case EFloorObjectType::ACCEL_PANEL:
			{
				CTile* tile = tileMap->GetTile(iter->first.second);

				CTile* tile1 = tileMap->GetTile(iter->first.second);

				int indexX = tile1->GetIndexX();
				int indexY = tile1->GetIndexY();

				CTile* tile2 = tileMap->GetTile(indexX + 1, indexY);
				CTile* tile3 = tileMap->GetTile(indexX, indexY + 1);
				CTile* tile4 = tileMap->GetTile(indexX + 1, indexY + 1);

				tile1->SetTileProperty(ETileProperty::ACCEL_PANEL);
				tile2->SetTileProperty(ETileProperty::ACCEL_PANEL);
				tile3->SetTileProperty(ETileProperty::ACCEL_PANEL);
				tile4->SetTileProperty(ETileProperty::ACCEL_PANEL);

			}
			break;
			case EFloorObjectType::ITEM_BOX:
			{
				CItemBox* box = new CItemBox(iter->first.second, iter->second, this);
				CTile* tile1 = tileMap->GetTile(iter->first.second);

				int indexX = tile1->GetIndexX();
				int indexY = tile1->GetIndexY();

				CTile* tile2 = tileMap->GetTile(indexX + 1, indexY);
				CTile* tile3 = tileMap->GetTile(indexX, indexY + 1);
				CTile* tile4 = tileMap->GetTile(indexX + 1, indexY + 1);

				tile1->SetTileProperty(ETileProperty::OTHER);
				tile2->SetTileProperty(ETileProperty::OTHER);
				tile3->SetTileProperty(ETileProperty::OTHER);
				tile4->SetTileProperty(ETileProperty::OTHER);

				m_SpecialFloor.push_back(box);
				m_vecFloor.push_back(box);
			}
			break;
			case EFloorObjectType::OIL_SLICK:
			{
				COilSlick* oil = new COilSlick(iter->first.second, iter->second, this);
				CTile* tile1 = tileMap->GetTile(iter->first.second);

				int indexX = tile1->GetIndexX();
				int indexY = tile1->GetIndexY();

				CTile* tile2 = tileMap->GetTile(indexX + 1, indexY);
				CTile* tile3 = tileMap->GetTile(indexX, indexY + 1);
				CTile* tile4 = tileMap->GetTile(indexX + 1, indexY + 1);

				tile1->SetTileProperty(ETileProperty::OIL_SLICK);
				tile2->SetTileProperty(ETileProperty::OIL_SLICK);
				tile3->SetTileProperty(ETileProperty::OIL_SLICK);
				tile4->SetTileProperty(ETileProperty::OIL_SLICK);

				m_vecFloor.push_back(oil);
			}
			break;
			case EFloorObjectType::COIN:
			{
				CCoin* coin = new CCoin(iter->first.second, iter->second, this);
				CTile* tile = tileMap->GetTile(iter->first.second);

				tile->SetTileProperty(ETileProperty::OTHER);

				m_SpecialFloor.push_back(coin);
				m_vecFloor.push_back(coin);
			}
			break;
			case EFloorObjectType::RAMP_HORIZONTAL:
			{
				CRampHorizontal* ramp = new CRampHorizontal(iter->first.second, iter->second, this);
				CTile* tile = tileMap->GetTile(iter->first.second);

				tile->SetTileProperty(ETileProperty::RAMP_HORIZONTAL);

				SAFE_RELEASE(ramp);

			}
			break;
			case EFloorObjectType::RAMP_VERTICAL:
			{
				CRampVertical* ramp = new CRampVertical(iter->first.second, iter->second, this);
				CTile* tile = tileMap->GetTile(iter->first.second);

				tile->SetTileProperty(ETileProperty::RAMP_VERTICAL);

				SAFE_RELEASE(ramp);

			}
			break;
			}
		}
	}

	for (const auto& obj : m_vecFloor)
	{
		obj->SetState(EFloorObjectState::INITIAL);
	}



	tileMap->LoadAI(path.c_str());
	CFloorObject::SetScene(this);
	CFloorObject::ApplyAllChanges();

	if (m_FloorObjectUpdated)
	{
		m_CircuitTexture = m_OriginalCircuit->Clone();

		auto iter = m_vecFloor.begin();
		auto iterEnd = m_vecFloor.end();

		for (; iter != iterEnd; iter++)
		{
			Vector2 pos = (*iter)->GetPos();
			Vector2 size = (*iter)->GetSize();
			CTexture* texture = (*iter)->GetCurrentTexture();
			if (texture != nullptr)
			{
				TransparentBlt(m_CircuitTexture->GetTextureDC(), (int)pos.x, (int)pos.y, (int)size.x, (int)size.y,
					texture->GetTextureDC(), 0, 0, texture->GetWidth(), texture->GetHeight(), texture->GetColorKey());

				SAFE_RELEASE(texture);
			}

		}

		m_FloorObjectUpdated = false;
	}

	m_CircuitTexture->SetPixelArray();


	m_PlayerSelect = player;

	m_DriverArray[(int)m_PlayerSelect]->m_Kart = &m_DriverArray[(int)m_PlayerSelect]->m_Kart->MakePlayer();



	m_Player = m_DriverArray[(int)m_PlayerSelect];



	// 초기에는 플레이어를 꼴지에 배치한다.
	std::swap(m_DriverArray[(int)m_PlayerSelect], m_DriverArray[(int)EMenuPlayer::END - 1]);
	std::random_shuffle(m_DriverArray, m_DriverArray + ((int)EMenuPlayer::END - 1));

	// 랭크도 설정한다.
	for (int i = 0; i < (int)EMenuPlayer::END; i++)
	{
		m_Positions[i] = m_DriverArray[i];
	}


	for (int i = 0; i < (int)EMenuPlayer::END; i++)
	{
		m_DriverArray[i]->SetControlType(EDriverControlType::AI_GRADIENT);

		Vector2 pos = tileMap->GetPlayerInitPos(i + 1);
		pos.x /= MAP_ASSETS_WIDTH;
		pos.y /= MAP_ASSETS_HEIGHT;
		m_DriverArray[i]->SetPos(pos);
	}

	m_Player->SetControlType(EDriverControlType::PLAYER);

	CCollisionHash::GetInst()->ResetDynamic();
	CCollisionHash::GetInst()->ResetStatic();

	for (const auto& obj : m_WallObjList)
	{
		CCollisionHash::GetInst()->RegisterStatic(obj);
	}


	CRaceGUI* GUI = CreateUIWindow<CRaceGUI>("Race GUI");

	for (size_t i = 0; i < m_Positions.size(); i++)
	{
		m_Positions[i]->m_Rank = i;

		if (m_Positions[i] == m_Player)
		{
			CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");
			gui->SetRank(i + 1);
		}
	}

	return true;
}

void CRaceScene::Mode7(const Vector2& cameraPos, const float angle, const float fovHalf, const float clipNear, const float clipFar, const Vector2& circuitSize, const bool perspective, CTexture& sampleMap)
{
	float fovMin = angle - fovHalf;
	float fovMax = angle + fovHalf;

	Vector2 trigMin = Vector2(cosf(fovMin), sinf(fovMin));
	Vector2 trigMax = Vector2(cosf(fovMax), sinf(fovMax));

	// 4개의 프러스텀 포인트
	Vector2 far1 = cameraPos + trigMin * clipFar;
	Vector2 far2 = cameraPos + trigMax * clipFar;
	Vector2 near1 = cameraPos + trigMin * clipNear;
	Vector2 near2 = cameraPos + trigMax * clipNear;

	int width = (int)circuitSize.x;
	int mapHeight = (int)circuitSize.y;

	COLORREF* sampleColorArray = new COLORREF[width * mapHeight];

	for (int y = 0; y < mapHeight; y++)
	{
		float sampleDepth = (float)y / (float)mapHeight;

		Vector2 start, end;

		// 샘플링 시작
		if (perspective)
		{
			start = near1 + (far1 - near1) / sampleDepth;
			end = near2 + (far2 - near2) / sampleDepth;
		}
		else
		{
			start = near1 + (far1 - near1) * (1.0f - sampleDepth);
			end = near2 + (far2 - near2) * (1.0f - sampleDepth);
		}

		for (int x = 0; x < width; x++)
		{
			float sampleWidth = (float)x / (float)width;
			Vector2 sample = start + (end - start) * sampleWidth;
			COLORREF color;
			// 만약 범위내에 있다면
			if (sample.x >= 0.0f && sample.x <= 1.0f &&
				sample.y >= 0.0f && sample.y <= 1.0f)
			{
				color = SampleMap(sample);
			}
			// 만약 바깥으로 나갔다면
			else
			{
				sample.x = fmodf(sample.x, MAP_EDGES_SIZE / (float)MAP_ASSETS_WIDTH);

				sample.y = fmodf(sample.y, MAP_EDGES_SIZE / (float)MAP_ASSETS_HEIGHT);

				if (sample.x < 0.0f)
					sample.x += MAP_EDGES_SIZE / (float)MAP_ASSETS_WIDTH;
				if (sample.y < 0.0f)
					sample.y += MAP_EDGES_SIZE / (float)MAP_ASSETS_HEIGHT;

				sample.x *= MAP_ASSETS_WIDTH / (float)MAP_EDGES_SIZE;
				sample.y *= MAP_ASSETS_HEIGHT / (float)MAP_EDGES_SIZE;

				color = SampleEdge(sample);
			}

			sampleColorArray[y * width + x] = color;
		}
	}

	sampleMap.Create(circuitSize.x, circuitSize.y, sampleColorArray);

	SAFE_DELETE_ARRAY(sampleColorArray);
}

// 서킷을 샘플링한다.
COLORREF CRaceScene::SampleMap(const Vector2& pos)
{
	// 서킷의 크기를 구하고
	float xSize = (float)m_CircuitTexture->GetWidth();
	float ySize = (float)m_CircuitTexture->GetHeight();

	// 전달받은 위치의 값 (0 ~ 1) 사이의 값을 서킷 텍스쳐의 크기로 바꿨을때 최소값을 대입한다.
	unsigned int px = min(m_CircuitTexture->GetWidth() - 1, (unsigned int)(pos.x * xSize));
	unsigned int py = min(m_CircuitTexture->GetHeight() - 1, (unsigned int)(pos.y * ySize));

	// 현재 서킷의 색깔 배열을 조회해서 색을 반환한다.
	return m_CircuitTexture->PixelArrayAt(px, py);
}

COLORREF CRaceScene::SampleMap(unsigned int x, unsigned int y)
{
	float xSize = (float)m_CircuitTexture->GetWidth();
	float ySize = (float)m_CircuitTexture->GetHeight();

	unsigned int px = min(m_CircuitTexture->GetWidth() - 1, (unsigned int)(x * xSize));
	unsigned int py = min(m_CircuitTexture->GetHeight() - 1, (unsigned int)(y * ySize));

	return m_CircuitTexture->PixelArrayAt(px, py);
}

COLORREF CRaceScene::SampleEdge(const Vector2& pos)
{
	float xSize = (float)m_EdgeTexture->GetWidth();
	float ySize = (float)m_EdgeTexture->GetHeight();

	unsigned int px = min(m_EdgeTexture->GetWidth() - 1, (unsigned int)(pos.x * xSize));
	unsigned int py = min(m_EdgeTexture->GetHeight() - 1, (unsigned int)(pos.y * ySize));

	return m_EdgeTexture->PixelArrayAt(px, py);
}

COLORREF CRaceScene::SampleEdge(unsigned int x, unsigned int y)
{
	float xSize = (float)m_EdgeTexture->GetWidth();
	float ySize = (float)m_EdgeTexture->GetHeight();

	unsigned int px = min(m_EdgeTexture->GetWidth() - 1, (unsigned int)(x * xSize));
	unsigned int py = min(m_EdgeTexture->GetHeight() - 1, (unsigned int)(y * ySize));

	return m_EdgeTexture->PixelArrayAt(px, py);
}

// 서킷의 텍스쳐를 샘플링하는 함수
void CRaceScene::SetCircuitTexture(CSharedPtr<CDriver> player, CTexture& circuitTexture)
{
	// 먼저 카메라의 좌표를 구해온다.
	float camX = GetCamera()->GetPos().x;
	float camY = GetCamera()->GetPos().y;

	// 만약 플레이어가 뒤를 돌아보지 않을 시에는
	if (!m_LookBack)
	{
		// 플레이의 위치에서 카메라의 거리만큼 뺀 거리를 카메라읭 위치로 잡는다.
		camX = player->GetPos().x - cosf(player->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_WIDTH);
		camY = player->GetPos().y - sinf(player->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_HEIGHT);
	}
	else
	{
		// 만약 뒤를 돌아보고 있을 경우는 보이지 않는 플레이어가 위치의 중심
		camX = m_InvisiblePlayer->GetPos().x;
		camY = m_InvisiblePlayer->GetPos().y;
	}
	
	// 위치를 설정한다.
	GetCamera()->SetPos(camX, camY);

	// 현재 화면의 해상도를 얻어오고
	Resolution rs = CGameManager::GetInst()->GetResolution();

	// 서킷의 크기를 정한다.
	Vector2 circuitSize((float)rs.width, rs.height * CIRCUIT_HEIGHT_PCT);

	// 모드7을 적용할때 현재 카메라와 타겟이 되는 물체의 각도 그리고 fov, clip near, clip far 이러한 값들을 전달한다.
	Mode7(GetCamera()->GetPos(), m_LookBack ? m_InvisiblePlayer->GetAngle() : player->GetAngle(), MODE7_FOV_HALF, MODE7_CLIP_NEAR,
		MODE7_CLIP_FAR, circuitSize, true, circuitTexture);
}

// 하늘의 텍스쳐를 설정한다.
void CRaceScene::SetSkyTexture(CSharedPtr<CDriver> player, CTexture& back, CTexture& front)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	// 현재 하늘의 텍스쳐에서 일부분만 샘플링한다.
	unsigned int textureWidth = (unsigned int)(rs.width / SKY_SCALE);
	unsigned int textureHeight = (unsigned int)(rs.height * SKY_HEIGHT_PCT / SKY_SCALE);

	// 높이와 너비를 구한다.
	unsigned int skyBackWidth = m_SkybackTexture->GetWidth();
	unsigned int skyBackHeight = m_SkybackTexture->GetHeight();
	skyBackWidth /= 2;

	// 높이와 너비를 구한다.
	unsigned int skyFrontWidth = m_SkyFrontTexture->GetWidth();
	unsigned int skyFrontHeight = m_SkyFrontTexture->GetHeight();
	skyFrontWidth /= 2;

	// 텍스쳐의 스케일을 구한다.
	float textureScale = textureHeight / (SKY_CUT_PCT * skyBackHeight);
	textureHeight = (int)(textureHeight / textureScale);
	textureWidth = (int)(textureWidth / textureScale);

	// 잘리는 픽셀의 수
	int backCutPixels = skyBackHeight - textureHeight;
	int frontCutPixels = skyFrontHeight - textureHeight;

	// 현재 플레이어가 바라보고 있는 각도를 0 ~ 2pi 범위 안에서 구한다.
	float modAngle = fmodf(m_LookBack ? m_InvisiblePlayer->GetAngle() : player->GetAngle(), 2.0f * (float)M_PI);

	// 만약 벗어났을 경우
	if (modAngle < 0.0f)
		modAngle += 2.0f * (float)M_PI;

	// 라디안을 각도로 바꾼다. skybackWidth는 한바퀴를 2pi로 생각해서 skybackWidth / 2pi를 곱한다.
	unsigned int sampleBackX = (int)(modAngle * skyBackWidth / (2.0f * (float)M_PI));

	// 전방의 각도를 구한다. pi
	float angleFront = fmodf(modAngle, (float)M_PI);

	// skyFront의 경우 0 ~ pi까지 범위를 가진다 따라서 skyBack과 skyFront는 다른 회전량을 가진다.
	unsigned int sampleFrontX = (int)(angleFront * skyFrontWidth / (float)M_PI);

	COLORREF* skyBackColors = new COLORREF[(skyBackHeight - backCutPixels) * textureWidth];

	// 높이의 경우 잘린 픽셀수만큼 덜 반복한다.
	for (unsigned int y = 0; y < skyBackHeight - backCutPixels; y++)
	{
		for (unsigned int x = 0; x < textureWidth; x++)
		{
			COLORREF color;
			// sampleBackX부터 시작해서 샘플링을 시작한다.
			color = m_SkybackTexture->PixelArrayAt(x + sampleBackX, y + backCutPixels);
			skyBackColors[y * textureWidth + x] = color;
		}
	}

	COLORREF* skyFrontColors = new COLORREF[(skyFrontHeight - frontCutPixels) * textureWidth];

	for (unsigned int y = 0; y < skyFrontHeight - frontCutPixels; y++)
	{
		for (unsigned int x = 0; x < textureWidth; x++)
		{
			COLORREF color;
			color = m_SkyFrontTexture->PixelArrayAt(x + sampleFrontX, y + frontCutPixels);
			skyFrontColors[y * textureWidth + x] = color;
		}
	}

	back.Create((float)textureWidth, (float)skyBackHeight - backCutPixels, skyBackColors);
	front.Create((float)textureWidth, (float)skyFrontHeight - frontCutPixels, skyFrontColors);

	SAFE_DELETE_ARRAY(skyBackColors);
	SAFE_DELETE_ARRAY(skyFrontColors);
}

void CRaceScene::SetMiniMapTexture(CTexture& miniMap)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	// 미니맵의 경우 카메라의 위치는 따로 구해준다.
	float minDist = MINIMAP_POS_DISTANCE - 1.0f - 0.05f;
	float maxDist = MINIMAP_POS_DISTANCE + 0.15f;

	// 270도 범위를 미니맵 FOV값으로 
	Mode7(Vector2(0.5f, MINIMAP_POS_DISTANCE), 3.0f * (float)M_PI_2, MINIMAP_FOV_HALF,
		minDist / cosf(MINIMAP_FOV_HALF), maxDist / cosf(MINIMAP_FOV_HALF),
		Vector2((float)rs.width, (float)rs.height * MINIMAP_HEIGHT_PCT), false, miniMap);
}

// 미니맵에서 스크린 좌표로
Vector2 CRaceScene::MinimapToScreen(const Vector2& position)
{
	Vector2 bottom = Vector2(MINIMAP_BOTTOM_X, MINIMAP_BOTTOM_Y);
	Vector2 top = Vector2(MINIMAP_TOP_X, MINIMAP_TOP_Y);

	// 선형 보간식
	Vector2 middleLeft = top + (bottom - top) * position.y;

	// 오른쪽의 경우 1.0f 에서 왼쪽의 값을 제거한다.
	Vector2 middleRight = Vector2(1.0f - middleLeft.x, middleLeft.y);

	// 다시한번 선형 보간식
	Vector2 ret = middleLeft + (middleRight - middleLeft) * position.x;

	return ret;
}

// 현재 좌표에서 상단 스크린의 좌표로 전환
bool CRaceScene::MapToScreen(CSharedPtr<CDriver> target, const Vector2& mapCoords, Vector2& screenCoords, float& z)
{
	Vector2 cameraPosition;

	cameraPosition.x = target->GetPos().x - cosf(target->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_WIDTH);
	cameraPosition.y = target->GetPos().y - sinf(target->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_HEIGHT);

	// 현재 상대적인 좌표에서 카메라의 위치를 다시한번 적용한다.
	Vector2 relativePoint = mapCoords - cameraPosition;


	// 길이를 구한다.
	float relativeLength = sqrtf(relativePoint.x * relativePoint.x + relativePoint.y * relativePoint.y);

	// 플레이어의 앞방향 벡터를 구한다.
	Vector2 forward = Vector2(cosf(target->GetAngle()), sinf(target->GetAngle()));

	// cos^2 + sin^2 = 1
	// sin = sqrt(1 - cos^2)
	float cosFov = cosf(MODE7_FOV_HALF);
	float sinFov = sqrtf(1.0f - cosFov * cosFov);

	// 내적으로 코사인
	// z의 값 플레이어와의 깊이감은 현재 전방 벡터에 카메라의 위치를 고려해준다.
	z = (forward.x * relativePoint.x + forward.y * relativePoint.y);
	float cos = z / relativeLength;
	// 외적으로 사인
	float sin = (forward.x * relativePoint.y - forward.y * relativePoint.x) / relativeLength;

	float y = MODE7_CLIP_FAR * cosf(MODE7_FOV_HALF) / (relativeLength * cos);

	float x = (sin + sinFov) / (2.0f * sinFov);

	screenCoords = Vector2(x, y);

	return x >= -0.1f && x < 1.1f && y >= 0.0f && y <= 2.0f;
}

// 그려낼 수 있는 드라이버들을 전부 가져온다.
void CRaceScene::RenderableDrivers(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	for (int i = 0; i < (int)EMenuPlayer::END; i++)
	{
		if (m_DriverArray[i] == target || !m_DriverArray[i]->IsVisible())
			continue;

		// 반지름 좌표를 구한다.
		Vector2 radius;
		if (m_LookBack)
			radius = Vector2(cosf(m_InvisiblePlayer->GetAngle()), sinf(m_InvisiblePlayer->GetAngle())) * m_DriverArray[i]->GetVisualRadius();
		else
			radius = Vector2(cosf(target->GetAngle()), sinf(target->GetAngle())) * m_DriverArray[i]->GetVisualRadius();

		Vector2 screen;

		float z;

		// 상대적인 위치를 전달한다. 현재 위치에서 플레이어가 바라보고있는 방향의 좌표를 빼면 
		// 상대적인 위치를 구할 수 있다. 카메라와 비슷한 원리.
		Vector2 driverPos = m_DriverArray[i]->GetPos();

		if (MapToScreen(m_LookBack ? m_InvisiblePlayer : target, driverPos - radius, screen, z))
		{
			float driverAngle = 0.0f;
			// 각도차이를 구한다.
			if (m_LookBack)
				driverAngle = m_InvisiblePlayer->GetAngle() - m_DriverArray[i]->GetAngle();
			else
				driverAngle = target->GetAngle() - m_DriverArray[i]->GetAngle();

			// 구한 각도차이로 어떤 모습을 그려낼지를 정한다.
			m_DriverArray[i]->m_Animator.SetViewTexture(driverAngle);
			CTexture* tex = m_DriverArray[i]->GetTexture();

			// 구한 스크린 좌표를 현재 해상도와 서킷의 비율, 하늘의 비율에 알맞게 조절한다.
			screen.x *= rs.width;
			screen.y *= rs.height * CIRCUIT_HEIGHT_PCT;
			screen.y += rs.height * SKY_HEIGHT_PCT;
			screen.y -= m_DriverArray[i]->m_Animator.GetSpriteMovementSpeed();

			if (m_DriverArray[i]->m_Animator.GetSmallTime() > 0.0f)
			{
				Vector2 playerPos;

				if (m_LookBack)
				{
					playerPos = Vector2(m_InvisiblePlayer->GetPos().x * 1024, m_InvisiblePlayer->GetPos().y * 1024);
				}
				else
				{
					playerPos = Vector2(target->GetPos().x * 1024, target->GetPos().y * 1024);
				}

				Vector2 pos = Vector2(m_DriverArray[i]->GetPos().x * 1024, m_DriverArray[i]->GetPos().y * 1024);

				float dist = Distance(playerPos, pos);

				screen.x += 64.0f / (dist);
				screen.y += 64.0f / (dist);
			}

			// 현재 깊이 z에 비례해서 스케일을 조절한다.
			float scale = 1.0f / (3.6f * logf(1.02f + 0.8f * z));

			// 만약 점프하고 있다면
			if (m_DriverArray[i]->GetHeight() > 0.0f)
			{
				// 그림자도 추가한다.
				CTexture* shadow = m_DriverArray[i]->GetShadow();
				// 그림자는 무엇보다도 가장 나중에 그려야하므로 큰 값을 넣는다.
				float zShadow = z + 10000.0f;
				// 그림자의 스케일을 한다.
				shadow->SetScale(m_DriverArray[i]->GetTexture()->GetScale() * CIRCUIT_HEIGHT_PCT * 2.0f);
				// y축의 스케일은 깊이값 z의 영향을 받는다.
				shadow->Scale(scale * screenScale, scale * screenScale / fmaxf(1.0f, z * 5.0f));
				shadow->SetPos(screen);

				vecRenderable.push_back(std::make_pair(std::make_pair(zShadow, shadow), (CWallObject*)m_DriverArray[i]));
			}

			// 현재 화면의 좌표에서 높이 만큼 빼준다.
			screen.y -= m_DriverArray[i]->GetHeight() * scale * screenScale;
			// 스케일 설정
			tex->Scale(scale * screenScale, scale * screenScale);
			tex->SetPos(screen);
			// 텍스쳐를 렌더링할 벡터에 추가
			vecRenderable.push_back(std::make_pair(std::make_pair(z, tex), (CWallObject*)m_DriverArray[i]));
		}
	}
}

// 드라이버와 똑같은 방법으로 그려낸다.
void CRaceScene::RenderableWallObjects(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	for (const auto& obj : m_WallObjList)
	{
		Vector2 radius = Vector2(cosf(m_LookBack ? m_InvisiblePlayer->GetAngle() : target->GetAngle()),
			sinf(m_LookBack ? m_InvisiblePlayer->GetAngle() : target->GetAngle())) * obj->GetVisualRadius();

		Vector2 screen;

		float z;

		if (MapToScreen(m_LookBack ? m_InvisiblePlayer : target, obj->GetPos() - radius, screen, z))
		{
			CTexture* tex = obj->GetTexture();
			tex->SetScale(CIRCUIT_HEIGHT_PCT, CIRCUIT_HEIGHT_PCT);
			screen.x *= rs.width;
			screen.y *= rs.height * CIRCUIT_HEIGHT_PCT;
			screen.y += rs.height * SKY_HEIGHT_PCT;

			float scale = 1.0f / (3.6f * logf(1.02f + 0.8f * z));

			if (obj->GetHeight() > 0.0f)
			{
				CSharedPtr<CTexture> shadow = obj->GetShadow();

				float zShadow = z + 10000.0f;

				shadow->SetScale(CIRCUIT_HEIGHT_PCT, CIRCUIT_HEIGHT_PCT);

				shadow->Scale(scale * screenScale, scale * screenScale / fmaxf(1.0f, z * 5.0f));

				shadow->SetPos(screen);

				vecRenderable.push_back(std::make_pair(std::make_pair(zShadow, shadow), obj));
			}

			screen.y -= obj->GetHeight() * scale * screenScale;
			tex->Scale(scale * screenScale, scale * screenScale);
			tex->SetPos(screen);
			vecRenderable.push_back(std::make_pair(std::make_pair(z, tex), obj));
		}
	}
}

// 아이템도 드라이버와 벽과 같이 똑같은 방법으로 그려낸다.
void CRaceScene::RenderableItems(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	for (const auto& item : m_ItemList)
	{
		Vector2 radius = Vector2(cosf(m_LookBack ? m_InvisiblePlayer->GetAngle() : target->GetAngle()),
			sinf(m_LookBack ? m_InvisiblePlayer->GetAngle() : target->GetAngle())) * item->GetVisualRadius();

		Vector2 screen;

		float z;

		if (MapToScreen(m_LookBack ? m_InvisiblePlayer : target, item->GetPos() - radius, screen, z))
		{
			CTexture* tex = item->GetTexture();
			if (tex)
			{
				tex->SetScale(CIRCUIT_HEIGHT_PCT, CIRCUIT_HEIGHT_PCT);

				screen.x *= rs.width;
				screen.y *= rs.height * CIRCUIT_HEIGHT_PCT;
				screen.y += rs.height * SKY_HEIGHT_PCT;
				float scale = 1.0f / (3.6f * logf(1.02f + 0.8f * z));

				screen.y -= item->GetHeight() * scale * screenScale;
				tex->Scale(scale * screenScale, scale * screenScale);
				tex->SetPos(screen);
				vecRenderable.push_back(std::make_pair(std::make_pair(z, tex), (CWallObject*)item));
			}
		}
	}
}

void CRaceScene::SetFloorObjectUpdated(bool val)
{
	m_FloorObjectUpdated = true;
}

void CRaceScene::CollideWithFloor(const CSharedPtr<CDriver>& driver)
{
	for (const auto& obj : m_SpecialFloor)
	{
		if (obj->CollidesWith(driver))
		{
			obj->InteractWith(driver, m_CurrentRaceTime);
		}
	}
}


void CRaceScene::LoadSound()
{
	GetSceneResoruce()->Load3DSound("Engine", true, "Engine", "sfx/engine.ogg");


	GetSceneResoruce()->LoadSound("Effect", true, "Star", "sfx/star.ogg");
	GetSceneResoruce()->LoadSound("Effect", true, "Skid", "sfx/skid.ogg");
	GetSceneResoruce()->LoadSound("Effect", true, "Brake", "sfx/brake.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Crash", "sfx/thudpipe.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Mushroom", "sfx/boost.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Thunder", "sfx/lightning.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Jump", "sfx/jump.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Landing", "sfx/landing.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Hit", "sfx/spinout.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Shrink", "sfx/shrink.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Grow", "sfx/grow.ogg");
	GetSceneResoruce()->LoadSound("Effect", true, "RedShell", "sfx/redshell.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "CpuItem", "sfx/cputhrow.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Throw", "sfx/throw.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "UseItem", "sfx/itemdestroy.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Coin", "sfx/coin.ogg");
	GetSceneResoruce()->LoadSound("Effect", true, "ItemReel", "sfx/itemreel.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "GotItem", "sfx/gotitem.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Fall", "sfx/fall.ogg");
	GetSceneResoruce()->LoadSound("Effect", true, "WrongDir", "sfx/lakituwarning.ogg");
	GetSceneResoruce()->LoadSound("Effect", true, "Feather", "sfx/feather.ogg");

	GetSceneResoruce()->LoadSound("BGM", false, "Goal", "sfx/goal.ogg");
	GetSceneResoruce()->LoadSound("BGM", false, "Fanfare", "sfx/racefanfare.ogg");
	GetSceneResoruce()->LoadSound("BGM", false, "RaceStart", "sfx/racestart.ogg");
	GetSceneResoruce()->LoadSound("BGM", false, "LakituFinalLap", "sfx/finallap.ogg");
	GetSceneResoruce()->LoadSound("BGM", false, "Win", "sfx/win.ogg");
	GetSceneResoruce()->LoadSound("BGM", false, "Lose", "sfx/lose.ogg");
}

void CRaceScene::LoadItem()
{
	GetSceneResoruce()->LoadTexture("GreenShell", TEXT("WallObjects/GreenShell.bmp"));

	m_GreenShell = GetSceneResoruce()->FindTexture("GreenShell");

	GetSceneResoruce()->LoadTexture("RedShell", TEXT("WallObjects/RedShell.bmp"));

	m_RedShell = GetSceneResoruce()->FindTexture("RedShell");

	GetSceneResoruce()->LoadTexture("Banana", TEXT("WallObjects/Banana.bmp"));

	m_Banana = GetSceneResoruce()->FindTexture("Banana");


	for (int i = 0; i < 3; i++)
	{
		std::string name;
		TCHAR filePath[MAX_PATH] = {};

		name = "Coin_" + std::to_string(i + 1);

		wsprintf(filePath, TEXT("UI/Effect/Coin/Coin_%d.bmp"), i + 1);

		GetSceneResoruce()->LoadTexture(name, filePath);

		m_Coins[i] = GetSceneResoruce()->FindTexture(name);
	}

	for (int i = 0; i < 4; i++)
	{
		std::string name;
		TCHAR filePath[MAX_PATH] = {};

		name = "Collision_" + std::to_string(i + 1);

		wsprintf(filePath, TEXT("UI/Effect/Collision/Collision_%d.bmp"), i + 1);

		GetSceneResoruce()->LoadTexture(name, filePath);

		m_CollisionParticles[i] = GetSceneResoruce()->FindTexture(name);
	}
}

void CRaceScene::ToggleAutoDriver(float deltaTime)
{
	if (m_Player->m_ControlType == EDriverControlType::PLAYER)
		m_Player->m_ControlType = EDriverControlType::AI_GRADIENT;
	else
		m_Player->m_ControlType = EDriverControlType::PLAYER;
}

void CRaceScene::GetFeather(float deltaTime)
{
	m_Player->PickUpItem(EItem::FEATHER);
}

void CRaceScene::UseItemForward(float deltaTime)
{
	if (m_RaceFinished)
		return;
	CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");

	gui->ForceStop();

	CItem::UseItem(m_Player, m_Positions, true, this, gui);
}

void CRaceScene::UseItemBackWard(float deltaTime)
{
	if (m_RaceFinished)
		return;
	CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");

	gui->ForceStop();

	CItem::UseItem(m_Player, m_Positions, false, this, gui);
}

void CRaceScene::LookBack(float deltaTime)
{
	if (m_RaceFinished)
		return;

	m_LookBack = true;
}

void CRaceScene::LookForward(float deltaTime)
{
	if (m_RaceFinished)
		return;

	m_LookBack = false;
}

void CRaceScene::BackToTitle(float deltaTime)
{
	GetSceneResoruce()->StopAllMusic();
	CSceneManager::GetInst()->CreateScene<CTitleScene>();
}
