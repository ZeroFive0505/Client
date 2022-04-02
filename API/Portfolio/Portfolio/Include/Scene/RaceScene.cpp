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
	// ������ �ʴ� �÷��̾ ������.
	// ó�� ��Ʈ�ο� ������ ������ ���� �̿��Ѵ�.
	m_InvisiblePlayer = new CDriver("Invisible", this, Vector2(0.0f, 0.0f), 0.0f, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, EDriverControlType::DISABLED, CKartProperties::INVISIBLE,
		EMenuPlayer::MARIO, true);

	GetSceneResoruce()->SoundStop("player_circuit");

	LoadSound();

	LoadItem();

	// ����ѹ��� �ʱ�ȭ�Ѵ�.
	m_Lakitu = new CLakitu;
	m_Lakitu->Init(this);


	return true;
}

bool CRaceScene::Render(HDC hDC)
{
	// ������ ���Ѵ�.
	const float NORMAL_WIDTH = 512.0f;
	const float SCALE = CGameManager::GetInst()->GetResolution().width / NORMAL_WIDTH;

	// ȭ�鿡 �׷��� �ؽ��ĵ�
	CTexture circuitTexture, skyFrontTexture, skyBackTexture, miniMap;

	// ���� ��Ʈ�ο� ���̽��� ������ �ʾҴٸ�
	// ���� �÷��̾� �������� ȭ���� �׷�����.
	if (!m_RaceFinished && !m_Intro)
		SetSkyTexture(m_Player, skyBackTexture, skyFrontTexture);
	else
		SetSkyTexture(m_InvisiblePlayer, skyBackTexture, skyFrontTexture);

	// ���� ��������
	if (!m_RaceFinished && !m_Intro)
		SetCircuitTexture(m_Player, circuitTexture);
	else
		SetCircuitTexture(m_InvisiblePlayer, circuitTexture);

	// �̴ϸ��� �����ؼ� �׸���.
	SetMiniMapTexture(miniMap);


	Resolution rs = CGameManager::GetInst()->GetResolution();
	
	// ����ȭ���� ���̿� �ؽ����� ������ �������� �����Ѵ�.
	float backFactor = rs.width / (float)skyBackTexture.GetWidth();
	float frontFactor = rs.width / (float)skyFrontTexture.GetWidth();
	skyBackTexture.SetScale(backFactor, backFactor);
	skyFrontTexture.SetScale(frontFactor, frontFactor);

	// ���� �ֻ�ܺ��� �ϴ��� �׷�����.
	float currentHeight = 0;
	skyBackTexture.SetColorKey(255, 0, 255);
	skyFrontTexture.SetColorKey(255, 0, 255);
	skyBackTexture.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)skyBackTexture.GetWidth(), (float)skyBackTexture.GetHeight()));
	skyFrontTexture.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)skyFrontTexture.GetWidth(), (float)skyFrontTexture.GetHeight()));

	// �� ������ŭ ������ ��Ŷ�� �׷�����.
	currentHeight += rs.height * SKY_HEIGHT_PCT;
	circuitTexture.SetColorKey(255, 0, 255);
	circuitTexture.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)circuitTexture.GetWidth(), (float)circuitTexture.GetHeight()));

	// ȭ�鿡 �׷����� ������Ʈ���� ���� ����
	std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>> wallObjects;

	// ���� ������ �����ų� ��Ʈ�����̶�� 
	// ������ �ʴ� �÷��̾� �������� �׷�����.
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

	// ���� �÷��̾ �ڸ� ���ƺ��ٸ� ������ ȭ�鿡 �׷�����.
	if (!m_RaceFinished && !m_Intro && !m_LookBack)
		m_Player->Render(SCALE, wallObjects);
	else if (m_LookBack)
		m_Player->RenderFront(SCALE, wallObjects);

	// z���� �������� ȭ�鿡 �׷�����.
	std::sort(wallObjects.begin(), wallObjects.end(),
		[](const std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>& lhs,
			const std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>& rhs)
	{
		return lhs.first.first > rhs.first.first;
	});


	// ���� �÷��̾�� �׷��� ����� �Ÿ��� ����Ͽ� ����� ����
	Vector2 playerPos;
	if (!m_RaceFinished)
		playerPos = Vector2(m_Player->GetPos().x * MAP_ASSETS_WIDTH, m_Player->GetPos().y * MAP_ASSETS_HEIGHT);
	else
		playerPos = Vector2(m_InvisiblePlayer->GetPos().x * MAP_ASSETS_WIDTH, m_InvisiblePlayer->GetPos().y * MAP_ASSETS_HEIGHT);

	// ��� ������Ʈ���� �׷�����.
	for (const auto& kv : wallObjects)
	{
		if (kv.second == m_Player && !m_RaceFinished)
		{
			// �÷��̾��� ��� �׳� �׷�����.
			kv.first.second->Render(hDC, Vector2(0.0f, 0.0f), Vector2((float)kv.first.second->GetWidth(), (float)kv.first.second->GetHeight()));
		}
		else
		{
			// �ƴ� ��� �Ÿ��� ���� ������ ������ �����ȴ�.
			Vector2 otherPos = Vector2(kv.second->GetPos().x * MAP_ASSETS_WIDTH, kv.second->GetPos().y * MAP_ASSETS_HEIGHT);

			float dist = Distance(playerPos, otherPos);

			float distFactor = dist / 64.0f;

			kv.first.second->RenderByFactor(hDC, Vector2(kv.first.second->GetPos().x, kv.first.second->GetPos().y),
				Vector2(0.0f, 0.0f), Vector2((float)kv.first.second->GetWidth(),
					(float)kv.first.second->GetHeight()), distFactor);
		}
	}


	// ���� �÷��̾ ���鿡 ����ְ� �ӵ��� ���� ���� �̻��̶�� ��ƼŬ�� �׷�����.
	if (m_Player->GetHeight() <= 0.0f &&
		m_Player->m_SpeedForward > m_Player->m_Kart->m_MaxNormalLinearSpeed / 4 && !m_RaceFinished)
	{
		bool IsSmall = m_Player->m_Animator.GetSmallTime() > 0 || m_Player->m_Animator.GetSmashTime() > 0;

		m_Player->m_Animator.DrawParticles(hDC, m_Player->GetTexture(), IsSmall, m_Player->GetPos());
	}

	// ��Ŷ ������ŭ ������ �̴ϸ��� �׸���.
	currentHeight += rs.height * CIRCUIT_HEIGHT_PCT;

	miniMap.SetColorKey(255, 0, 255);
	miniMap.Render(hDC, Vector2(0.0f, currentHeight), Vector2(0.0f, 0.0f), Vector2((float)miniMap.GetWidth(), (float)miniMap.GetHeight()));


	// �̴ϸ��� ��� y ��ǥ�� �����ؼ� �׸���.
	std::sort(m_DriverArray, m_DriverArray + (int)EMenuPlayer::END, [](const CSharedPtr<CDriver>& lhs, const CSharedPtr<CDriver>& rhs)
	{
		return lhs->GetPos().y < rhs->GetPos().y;
	});

	// �̴ϸʿ� �׷��� ĳ���͵�
	for (const auto& driver : m_DriverArray)
	{
		// �� ������ �ʰ� �÷��̰� �ƴ϶�� �׷����� �ʴ´�.
		if (!driver->IsVisible() && driver != m_Player)
			continue;

		// �ؽ��ĸ� ������ �°� �����´�.
		CTexture miniDriver = driver->m_Animator.GetMinimapTexture(driver->GetAngle() + driver->m_SpeedTurn * 0.2f, SCALE);
		Vector2 mapPosition = MinimapToScreen(driver->GetPos());

		miniDriver.SetColorKey(255, 0, 255);
		miniDriver.SetPivot(miniDriver.GetWidth() / 2.0f, miniDriver.GetHeight() * 0.9f);
		miniDriver.SetPos(mapPosition.x * rs.width, mapPosition.y * rs.height);
		miniDriver.Scale(0.5f, 0.5f);
		miniDriver.Render(hDC, Vector2(miniDriver.GetPos().x, miniDriver.GetPos().y), Vector2(0.0f, 0.0f), Vector2((float)miniDriver.GetWidth(), (float)miniDriver.GetHeight()));
	}


	// UI�� �������Ѵ�.
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

		// ������ ���ߵɶ� Ű���� �����ϰ� �ݹ��Լ��� ���´�.
		CInput::GetInst()->SetPlayModeKeySet();

		CInput::GetInst()->SetCallback<CRaceScene>("ItemUseForward", KeyState_Down, this, &CRaceScene::UseItemForward);
		CInput::GetInst()->SetCallback<CRaceScene>("ItemUseBackward", KeyState_Down, this, &CRaceScene::UseItemBackWard);
		CInput::GetInst()->SetCallback<CRaceScene>("BackToTitle", KeyState_Down, this, &CRaceScene::BackToTitle);
		CInput::GetInst()->SetCallback<CRaceScene>("Feather", KeyState_Down, this, &CRaceScene::GetFeather);
		GetSceneResoruce()->SoundPlay("Fanfare");
	}

	// ��Ʈ�� ���ϋ�
	if (m_Intro)
	{
		// ��Ʈ�� �ð��� ������Ű��
		m_IntroTime += deltaTime;

		// ���� ����� �ð��� ���� �ʾҴٸ�
		if (m_IntroTime < 1.5f && !m_RaceStart)
		{
			float d = (m_IntroTime) / 1.5f;
			Vector2 pos = m_Player->GetPos();
			// ������ �ʴ� ĳ���͸� ���� �÷��̾��� ��ġ���� ���� ������ �����Ѵ�.
			m_InvisiblePlayer->SetPos(pos + Vector2(0.0f, ANIMATION_FORWARD_DIST * -1.0f) * (1.0f - d));
			// ������ �ʴ� �÷��̾��� ��� ���� �÷��̾��� ���� -M_PI_2���� 180�� �ð�������� ���� ��ġ���� �ٶ������
			m_InvisiblePlayer->SetAngle((float)M_PI_2);
		}
		// ���� ����ѽð��� �����ٸ�
		else if (m_IntroTime > 1.5f && !m_RaceStart)
		{
			// ������ �ʴ� �÷��̾��� ��ġ�� ���� �÷��̾��� ��ġ�� �ٲ۴�.
			m_InvisiblePlayer->SetPos(m_Player->GetPos());
			// �ϴ� ������ �÷��̾ ���� �ٶ󺸰� �ִ� ������ �����Ѵ�.
			m_InvisiblePlayer->SetAngle((float)-M_PI_2);

			if (m_IntroTime < 3.0f)
			{
				// ���� ����� �ð��� ������ �ʾҴٸ� ������ ���ƾ��Ѵ�.
				float d = (m_IntroTime - 1.5f) / (3.0f - 1.5f);
				// ���� 3ó�� ����� �ð��� ��ٸ� d�� ���� 1�� ��������� M_PI * -0.5f�� �ȴ� ���� ���� �÷��̾ �ٶ󺸰� �ִ� ������ �ȴ�.
				m_InvisiblePlayer->SetAngle((float)M_PI * (0.5f - d));
			}
			// ���� ���� ����ѹ��� ���� ī��Ʈ�� ���� �ʾҴٸ�
			else if (m_Lakitu->IsSleeping())
			{
				// ���� ��Ʈ�δ� ���̳���.
				m_Intro = false;
				// Ű �Ҵ�
				CInput::GetInst()->SetCallback<CRaceScene>("LookBack", KeyState_Hold, this, &CRaceScene::LookBack);
				CInput::GetInst()->SetCallback<CRaceScene>("LookBack", KeyState_Up, this, &CRaceScene::LookForward);
				CInput::GetInst()->SetCallback<CRaceScene>("ToggleAuto", KeyState_Down, this, &CRaceScene::ToggleAutoDriver);
				// ���� �Ҹ� ����
				GetSceneResoruce()->PlayEngine("Engine", true);
				GetSceneResoruce()->UpdateEngineSound("Engine", m_Player, m_Player->GetPos(),
					m_Player->GetHeight(), m_Player->m_SpeedForward, m_Player->m_SpeedTurn);
				GetSceneResoruce()->UpdateListener(m_Player->GetPos(), m_Player->GetAngle(), m_Player->GetHeight());
				// ����ѹ� ī��Ʈ ����
				m_Lakitu->ShowRaceStart();
			}
		}
	}


	// ���̽��� ���۵� ��
	if (m_RaceStart && !m_RaceFinished)
	{
		// ���� ���� bgm�� ���۵��� �ʾҴٸ� ���
		if (!m_MainBgmStart)
		{
			m_MainBgmStart = true;
			GetSceneResoruce()->SoundPlay("Main");
		}

		// ���̽� �ð� ����
		m_CurrentRaceTime += deltaTime;

		// ���� �÷��̾ �ڸ� ���ƺ� �ÿ���
		if (m_LookBack)
		{
			// Ư����ġ ��ŭ ������ �����̰�
			m_InvisiblePlayer->SetPos(m_Player->GetPos() + Vector2(cosf(m_Player->GetAngle()), sinf(m_Player->GetAngle())) * 0.03f);
			// ������ 180�� Ʋ� �ݴ� ������ ���� �Ѵ�.
			m_InvisiblePlayer->SetAngle(m_Player->GetAngle() - (float)M_PI);
		}

		// ��� ĳ���͸� ��ȸ�ϸ鼭 ����
		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			m_DriverArray[i]->Update(deltaTime);
		}

		GetSceneResoruce()->UpdateEngineSound("Engine", m_Player, m_Player->GetPos(),
			m_Player->GetHeight(), m_Player->m_SpeedForward, m_Player->m_SpeedTurn);
		GetSceneResoruce()->UpdateListener(m_Player->GetPos(), m_Player->GetAngle(), m_Player->GetHeight());

		// CPU ������ ���
		if (m_CurrentRaceTime > m_ItemUseAt)
		{
			// ���� �������� ��ٸ� ���� �������� ����� �ð��� �帥 �Ŀ� ����.
			m_ItemUseAt = m_CurrentRaceTime + TIME_BETWEEN_ITEM_CHECKS;


			for (const auto& driver : m_DriverArray)
			{
				// �÷��̾ �ƴ� CPU�� ���
				if (driver != m_Player && m_Player->GetItem() != EItem::NONE)
				{
					// Ȯ���� ���� �������� ����Ѵ�.
					float r = rand() / (float)RAND_MAX;

					// ������ ���Ȯ���� ��ȯ�ް�
					std::pair<float, bool> prob = CItem::GetItemUseProb(driver, m_Positions);

					// ������ �����Ѵٸ� �������� ����Ѵ�.
					if (r < prob.first / driver->m_ItemProbModifier)
					{
						CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");
						CItem::UseItem(driver, m_Positions, prob.second, this, gui);
					}
				}
			}
		}

		// ������ �����Ѵ�.
		auto iter = m_Positions.begin();

		// ���̽��� ���� ������ ������ �ʿ䰡 ������ �Ѿ��.
		while (iter != m_Positions.end() && (*iter)->GetLaps() > MAX_LAPS)
		{
			iter++;
		}

		// ���� �ݺ��� ��ġ�������� �����Ѵ�.
		std::sort(iter, m_Positions.end(), []
		(const CDriver* lhs, const CDriver* rhs) {
			// ���� ���� ���̶��
			if (lhs->GetLaps() == rhs->GetLaps())
			{
				// ��ȭ���� �� ū ���� ������ �� ���ƾ��Ѵ�.
				return lhs->GetLastGradient() < rhs->GetLastGradient();
			}
			// �ƴ϶�� �� ���� ����
			else
			{
				return lhs->GetLaps() > rhs->GetLaps();
			}
		});

		// ������ ��ȸ�ϸ鼭 ������ �����Ѵ�.
		for (size_t i = 0; i < m_Positions.size(); i++)
		{
			m_Positions[i]->m_Rank = i;

			if (m_Positions[i] == m_Player)
			{
				CRaceGUI* gui = FindUIWindow<CRaceGUI>("Race GUI");
				gui->SetRank(i + 1);
			}
		}

		// �� ������Ʈ ����
		for (const auto& obj : m_WallObjList)
			obj->Update(deltaTime);

		// ������ ����
		for (const auto& item : m_ItemList)
			item->Update(deltaTime);

		// �ٴ� ������Ʈ ����
		for (const auto& floor : m_SpecialFloor)
			floor->Update(deltaTime);

		// ���� ������ ����߿� �� ��ų� �������� �ʴ� �������� �����Ѵٸ� �����Ѵ�.
		m_ItemList.erase(std::remove_if(m_ItemList.begin(), m_ItemList.end(), [](const CSharedPtr<CItem>& item) {return !item ||
			item->GetUsed(); }), m_ItemList.end());

		// �浹�׽�Ʈ
		CCollisionHash::GetInst()->ResetDynamic();

		// �������� ���� �浹ü ��Ͽ� ����Ѵ�.
		for (const auto& item : m_ItemList)
		{
			CCollisionHash::GetInst()->RegisterDynamic((CWallObject*)item);
		}

		// ĳ���͵� ��������
		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			CCollisionHash::GetInst()->RegisterDriver(m_DriverArray[i]);
		}

		CollisionData data;

		// ��� ĳ���͸� ��ȸ�ϸ鼭 ���� �浹�� �Ͼ�ٸ�
		for (int i = 0; i < (int)EMenuPlayer::END; i++)
		{
			if (CCollisionHash::GetInst()->Collide(m_DriverArray[i], data))
			{
				// ����Ұ� �ӵ� ȸ������ ���Ѵ�.
				m_DriverArray[i]->m_CollisionMomentum = data.m_Momentum;
				m_DriverArray[i]->m_SpeedForward *= data.m_SpeedFactor;
				m_DriverArray[i]->m_SpeedTurn *= data.m_SpeedFactor;

				// �浹�� Ÿ�Կ� ���� �����Ѵ�.
				switch (data.type)
				{
				case ECollisionType::HIT:
					m_DriverArray[i]->ApplyHit();
					break;
				case ECollisionType::SMASH:
					m_DriverArray[i]->ApplySmash();
					break;
				default:
					// ���� ĳ���Ͱ��� �浹�Ͻ�
					if (!m_DriverArray[i]->IsImmune())
					{
						m_DriverArray[i]->AddCoin(-1);
					}

					if (m_DriverArray[i] == m_Player && m_DriverArray[i]->CanDrive() &&
						m_DriverArray[i]->m_SpeedForward != 0.0f)
					{
						// �浹 ���� ���.
						AddCollisionEffect(m_Player->GetPos());
						GetSceneResoruce()->SoundPlay("Crash");
					}

					// �浹 ����Ʈ ����
					AddCollisionEffect(m_DriverArray[i]->GetPos());
					break;
				}
			}
		}


		// ���� �ٴ� ������Ʈ�� ���Ӱ� ���ŉ��� ��
		bool hasChanged = CFloorObject::ApplyAllChanges();

		if (hasChanged)
		{

			if (m_FloorObjectUpdated)
			{
				{
					// ������ �ؽ��ĸ� �����ؿ´�. (�ٴ� ������Ʈ�� �ƹ��͵� �� �׷���)
					m_CircuitTexture = m_OriginalCircuit->Clone();
					// �ٴڿ�����Ʈ���� ��ȸ�Ѵ�.
					auto iter = m_vecFloor.begin();
					auto iterEnd = m_vecFloor.end();

					// ��� �ٴ� ������Ʈ���� ��ȸ�ϸ鼭 
					for (; iter != iterEnd; iter++)
					{
						// ��ġ�� ũ�⸦ ���ϰ�
						Vector2 pos = (*iter)->GetPos();
						Vector2 size = (*iter)->GetSize();
						CTexture* texture = (*iter)->GetCurrentTexture();

						// �ؽ��ĸ� �׷�����.
						if (texture != nullptr)
						{
							// ȭ�鿡 �׸��� ���̾ƴ϶� ����� ��Ʈ�� ������ �׷�����.
							TransparentBlt(m_CircuitTexture->GetTextureDC(), (int)pos.x, (int)pos.y, (int)size.x, (int)size.y,
								texture->GetTextureDC(), 0, 0, texture->GetWidth(), texture->GetHeight(), texture->GetColorKey());

							// �׸��� �����Ѵ�.
							SAFE_RELEASE(texture);
						}
					}

					m_FloorObjectUpdated = false;
				}

				// �ٽ� �� �迭�� ����
				m_CircuitTexture->SetPixelArray();
			}
		}


		// UI�� �׷�����.
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
	// ���̽��� ������ ��
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


		// ������ ����.
		float turnPct = std::fminf(m_TimeAfterRaceFinish / ANIMATION_TURN_TIME, 1.0f);
		// turnPct�� �ð��� ������ ���� 1.0�� �� ���̴�.
		// ���� �Ʒ��� ���� ���� ���������� 180�� �� ���̴�.
		float animTurnAngle = turnPct * (float)M_PI;
		// �Ʒ��� ���� ���������� 1.0�� �� ���̴�.
		float zoomOutPct = std::fminf(m_TimeAfterRaceFinish / ANIMATION_ZOOM_OUT_TIME, 1.0f);

		// ������ �ʴ� �÷��̾��� ��ġ�� ���� �÷��̾��� ��ġ���� zoomOutPct�� ���� ����ؼ� ���濡 ��ġ�ϰ� �ȴ�.
		m_InvisiblePlayer->SetPos(m_Player->GetPos() + Vector2(cosf(m_Player->GetAngle()), sinf(m_Player->GetAngle())) * 0.04f * zoomOutPct);

		// ������ �÷��̾ �ٶ󺸰��ִ� �������� +180��
		m_InvisiblePlayer->SetAngle(m_Player->GetAngle() + animTurnAngle);


		// ������ �����Ѵ�.
		auto iter = m_Positions.begin();

		// ���̽��� ���� ������ ������ �ʿ䰡 ������ �Ѿ��.
		while (iter != m_Positions.end() && (*iter)->GetLaps() > MAX_LAPS)
		{
			iter++;
		}

		// ���� �ݺ��� ��ġ�������� �����Ѵ�.
		std::sort(iter, m_Positions.end(), []
		(const CDriver* lhs, const CDriver* rhs) {
			// ���� ���� ���̶��
			if (lhs->GetLaps() == rhs->GetLaps())
			{
				// ��ȭ���� �� ū ���� ������ �� ���ƾ��Ѵ�.
				return lhs->GetLastGradient() < rhs->GetLastGradient();
			}
			// �ƴ϶�� �� ���� ����
			else
			{
				return lhs->GetLaps() > rhs->GetLaps();
			}
		});

		m_Lakitu->Update(deltaTime);

	}
	else
	{
		// ���� ���̽��� ���۾��߰� ī��Ʈ�� �������� ��쿡��
		if (!m_RaceStart && m_Lakitu->HasRaceStarted())
		{
			m_RaceStart = true;
			CInput::GetInst()->SetCallback<CRaceScene>("Drift", KeyState_Down, this, &CRaceScene::DriftStart);
			for (int i = 0; i < (int)EMenuPlayer::END; i++)
			{
				// CPU���߿� ����� ���������� ����ϰ�
				// �׷��� ���ϴ� CPU�� �����Ѵ�.
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
					// ���� �÷��̾ ������ �ð���ŭ �Ǽ��� �ߴٸ� ���� �ν���
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


		// �Ǽ� �ð��� �����Ѵ�.
		if (CInput::GetInst()->GetKeyState('X', EKeyState::KeyState_Hold))
			m_PlayerAccelTime += deltaTime;
		else
		{
			m_PlayerAccelTime -= deltaTime;
			if (m_PlayerAccelTime < 0.0f)
				m_PlayerAccelTime = 0.0f;
		}
	}

	// ����ѹ� ����
	m_Lakitu->Update(deltaTime);


	// ���� �Ѹ��� ����̹��� �����ϰ� ��� ����̹��� �� ���Դٸ�
	// ��ٸ��� �ð��� ���.
	if (m_WaitTime == 0.0f && m_Positions[m_Positions.size() - 2]->GetLaps() > MAX_LAPS)
	{
		m_WaitTime = m_CurrentRaceTime + 10.0f;
	}

	// ���� �÷��̾ �� ���� ���E�ų�, ���� ���̽� �ð��� ������ ��ٸ��� �ð����� ��ų�
	// �÷��̾ ���� ������ ������ ���¶�� ���̽��� ������ �ʾҴٸ�
	if ((m_Player->GetLaps() > MAX_LAPS ||
		(m_WaitTime != 0.0f && m_CurrentRaceTime > m_WaitTime &&
			m_Player->CanDrive() && m_Player->GetHeight() <= 0.0f)) && !m_RaceFinished)
	{
		// ���̽� ����
		m_RaceFinished = true;

		// �浹ü�� ����Ʈ�� ���� ����.
		CCollisionHash::GetInst()->ResetStatic();
		CCollisionHash::GetInst()->ResetDynamic();

		// �Ҹ� ����
		// �Ҹ� ����
		GetSceneResoruce()->StopAllMusic();
		GetSceneResoruce()->SoundStop("Engine");
		GetSceneResoruce()->SoundPlay("Goal");
		// ���� ���� �̳����
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

		// ����ѹ� ���� �ִϸ��̼�
		m_Lakitu->ShowRaceFinish();


		// �÷��̾ �ڵ� AI���� ����
		m_Player->m_ControlType = EDriverControlType::AI_GRADIENT;
	}

	return false;
}

// ���� ���� ���� ����Ʈ�� �����Ѵ�.
void CRaceScene::AddCoinEffect(const CDriver* driver, int amount, bool positive)
{
	float delay = 0.0f;

	for (int i = 0; i < amount; i++)
	{
		// ����Ʈ�� �����Ҷ� �ణ�� �����̸� �ش�.
		AddItem(new CCoinEffect(driver, this, "Coin Effect", delay, positive));

		// ������ �þ ��� 0.3��
		if (positive)
		{
			delay += 0.3f;
		}
		// �پ�� ���� 0.1��
		else
		{
			delay += 0.1f;
		}
	}
}

// �浹ȿ���� �������� �������� �߰��ȴ�.
void CRaceScene::AddCollisionEffect(const Vector2& pos)
{
	AddItem(new CCollisionEffect(pos, this, "Collision Effect"));
}

// ��Ŷ �ʱ�ȭ �ڵ� �ܺο��� ���� ��ȯ�ϰ� �� �ڵ带 ȣ���Ѵ�.
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

	// CPU�� ��� �پ��� �ʱⰪ�� �������� ���´�.
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



	// �ʱ⿡�� �÷��̾ ������ ��ġ�Ѵ�.
	std::swap(m_DriverArray[(int)m_PlayerSelect], m_DriverArray[(int)EMenuPlayer::END - 1]);
	std::random_shuffle(m_DriverArray, m_DriverArray + ((int)EMenuPlayer::END - 1));

	// ��ũ�� �����Ѵ�.
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

	// 4���� �������� ����Ʈ
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

		// ���ø� ����
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
			// ���� �������� �ִٸ�
			if (sample.x >= 0.0f && sample.x <= 1.0f &&
				sample.y >= 0.0f && sample.y <= 1.0f)
			{
				color = SampleMap(sample);
			}
			// ���� �ٱ����� �����ٸ�
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

// ��Ŷ�� ���ø��Ѵ�.
COLORREF CRaceScene::SampleMap(const Vector2& pos)
{
	// ��Ŷ�� ũ�⸦ ���ϰ�
	float xSize = (float)m_CircuitTexture->GetWidth();
	float ySize = (float)m_CircuitTexture->GetHeight();

	// ���޹��� ��ġ�� �� (0 ~ 1) ������ ���� ��Ŷ �ؽ����� ũ��� �ٲ����� �ּҰ��� �����Ѵ�.
	unsigned int px = min(m_CircuitTexture->GetWidth() - 1, (unsigned int)(pos.x * xSize));
	unsigned int py = min(m_CircuitTexture->GetHeight() - 1, (unsigned int)(pos.y * ySize));

	// ���� ��Ŷ�� ���� �迭�� ��ȸ�ؼ� ���� ��ȯ�Ѵ�.
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

// ��Ŷ�� �ؽ��ĸ� ���ø��ϴ� �Լ�
void CRaceScene::SetCircuitTexture(CSharedPtr<CDriver> player, CTexture& circuitTexture)
{
	// ���� ī�޶��� ��ǥ�� ���ؿ´�.
	float camX = GetCamera()->GetPos().x;
	float camY = GetCamera()->GetPos().y;

	// ���� �÷��̾ �ڸ� ���ƺ��� ���� �ÿ���
	if (!m_LookBack)
	{
		// �÷����� ��ġ���� ī�޶��� �Ÿ���ŭ �� �Ÿ��� ī�޶�� ��ġ�� ��´�.
		camX = player->GetPos().x - cosf(player->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_WIDTH);
		camY = player->GetPos().y - sinf(player->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_HEIGHT);
	}
	else
	{
		// ���� �ڸ� ���ƺ��� ���� ���� ������ �ʴ� �÷��̾ ��ġ�� �߽�
		camX = m_InvisiblePlayer->GetPos().x;
		camY = m_InvisiblePlayer->GetPos().y;
	}
	
	// ��ġ�� �����Ѵ�.
	GetCamera()->SetPos(camX, camY);

	// ���� ȭ���� �ػ󵵸� ������
	Resolution rs = CGameManager::GetInst()->GetResolution();

	// ��Ŷ�� ũ�⸦ ���Ѵ�.
	Vector2 circuitSize((float)rs.width, rs.height * CIRCUIT_HEIGHT_PCT);

	// ���7�� �����Ҷ� ���� ī�޶�� Ÿ���� �Ǵ� ��ü�� ���� �׸��� fov, clip near, clip far �̷��� ������ �����Ѵ�.
	Mode7(GetCamera()->GetPos(), m_LookBack ? m_InvisiblePlayer->GetAngle() : player->GetAngle(), MODE7_FOV_HALF, MODE7_CLIP_NEAR,
		MODE7_CLIP_FAR, circuitSize, true, circuitTexture);
}

// �ϴ��� �ؽ��ĸ� �����Ѵ�.
void CRaceScene::SetSkyTexture(CSharedPtr<CDriver> player, CTexture& back, CTexture& front)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	// ���� �ϴ��� �ؽ��Ŀ��� �Ϻκи� ���ø��Ѵ�.
	unsigned int textureWidth = (unsigned int)(rs.width / SKY_SCALE);
	unsigned int textureHeight = (unsigned int)(rs.height * SKY_HEIGHT_PCT / SKY_SCALE);

	// ���̿� �ʺ� ���Ѵ�.
	unsigned int skyBackWidth = m_SkybackTexture->GetWidth();
	unsigned int skyBackHeight = m_SkybackTexture->GetHeight();
	skyBackWidth /= 2;

	// ���̿� �ʺ� ���Ѵ�.
	unsigned int skyFrontWidth = m_SkyFrontTexture->GetWidth();
	unsigned int skyFrontHeight = m_SkyFrontTexture->GetHeight();
	skyFrontWidth /= 2;

	// �ؽ����� �������� ���Ѵ�.
	float textureScale = textureHeight / (SKY_CUT_PCT * skyBackHeight);
	textureHeight = (int)(textureHeight / textureScale);
	textureWidth = (int)(textureWidth / textureScale);

	// �߸��� �ȼ��� ��
	int backCutPixels = skyBackHeight - textureHeight;
	int frontCutPixels = skyFrontHeight - textureHeight;

	// ���� �÷��̾ �ٶ󺸰� �ִ� ������ 0 ~ 2pi ���� �ȿ��� ���Ѵ�.
	float modAngle = fmodf(m_LookBack ? m_InvisiblePlayer->GetAngle() : player->GetAngle(), 2.0f * (float)M_PI);

	// ���� ����� ���
	if (modAngle < 0.0f)
		modAngle += 2.0f * (float)M_PI;

	// ������ ������ �ٲ۴�. skybackWidth�� �ѹ����� 2pi�� �����ؼ� skybackWidth / 2pi�� ���Ѵ�.
	unsigned int sampleBackX = (int)(modAngle * skyBackWidth / (2.0f * (float)M_PI));

	// ������ ������ ���Ѵ�. pi
	float angleFront = fmodf(modAngle, (float)M_PI);

	// skyFront�� ��� 0 ~ pi���� ������ ������ ���� skyBack�� skyFront�� �ٸ� ȸ������ ������.
	unsigned int sampleFrontX = (int)(angleFront * skyFrontWidth / (float)M_PI);

	COLORREF* skyBackColors = new COLORREF[(skyBackHeight - backCutPixels) * textureWidth];

	// ������ ��� �߸� �ȼ�����ŭ �� �ݺ��Ѵ�.
	for (unsigned int y = 0; y < skyBackHeight - backCutPixels; y++)
	{
		for (unsigned int x = 0; x < textureWidth; x++)
		{
			COLORREF color;
			// sampleBackX���� �����ؼ� ���ø��� �����Ѵ�.
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

	// �̴ϸ��� ��� ī�޶��� ��ġ�� ���� �����ش�.
	float minDist = MINIMAP_POS_DISTANCE - 1.0f - 0.05f;
	float maxDist = MINIMAP_POS_DISTANCE + 0.15f;

	// 270�� ������ �̴ϸ� FOV������ 
	Mode7(Vector2(0.5f, MINIMAP_POS_DISTANCE), 3.0f * (float)M_PI_2, MINIMAP_FOV_HALF,
		minDist / cosf(MINIMAP_FOV_HALF), maxDist / cosf(MINIMAP_FOV_HALF),
		Vector2((float)rs.width, (float)rs.height * MINIMAP_HEIGHT_PCT), false, miniMap);
}

// �̴ϸʿ��� ��ũ�� ��ǥ��
Vector2 CRaceScene::MinimapToScreen(const Vector2& position)
{
	Vector2 bottom = Vector2(MINIMAP_BOTTOM_X, MINIMAP_BOTTOM_Y);
	Vector2 top = Vector2(MINIMAP_TOP_X, MINIMAP_TOP_Y);

	// ���� ������
	Vector2 middleLeft = top + (bottom - top) * position.y;

	// �������� ��� 1.0f ���� ������ ���� �����Ѵ�.
	Vector2 middleRight = Vector2(1.0f - middleLeft.x, middleLeft.y);

	// �ٽ��ѹ� ���� ������
	Vector2 ret = middleLeft + (middleRight - middleLeft) * position.x;

	return ret;
}

// ���� ��ǥ���� ��� ��ũ���� ��ǥ�� ��ȯ
bool CRaceScene::MapToScreen(CSharedPtr<CDriver> target, const Vector2& mapCoords, Vector2& screenCoords, float& z)
{
	Vector2 cameraPosition;

	cameraPosition.x = target->GetPos().x - cosf(target->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_WIDTH);
	cameraPosition.y = target->GetPos().y - sinf(target->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_HEIGHT);

	// ���� ������� ��ǥ���� ī�޶��� ��ġ�� �ٽ��ѹ� �����Ѵ�.
	Vector2 relativePoint = mapCoords - cameraPosition;


	// ���̸� ���Ѵ�.
	float relativeLength = sqrtf(relativePoint.x * relativePoint.x + relativePoint.y * relativePoint.y);

	// �÷��̾��� �չ��� ���͸� ���Ѵ�.
	Vector2 forward = Vector2(cosf(target->GetAngle()), sinf(target->GetAngle()));

	// cos^2 + sin^2 = 1
	// sin = sqrt(1 - cos^2)
	float cosFov = cosf(MODE7_FOV_HALF);
	float sinFov = sqrtf(1.0f - cosFov * cosFov);

	// �������� �ڻ���
	// z�� �� �÷��̾���� ���̰��� ���� ���� ���Ϳ� ī�޶��� ��ġ�� ������ش�.
	z = (forward.x * relativePoint.x + forward.y * relativePoint.y);
	float cos = z / relativeLength;
	// �������� ����
	float sin = (forward.x * relativePoint.y - forward.y * relativePoint.x) / relativeLength;

	float y = MODE7_CLIP_FAR * cosf(MODE7_FOV_HALF) / (relativeLength * cos);

	float x = (sin + sinFov) / (2.0f * sinFov);

	screenCoords = Vector2(x, y);

	return x >= -0.1f && x < 1.1f && y >= 0.0f && y <= 2.0f;
}

// �׷��� �� �ִ� ����̹����� ���� �����´�.
void CRaceScene::RenderableDrivers(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable)
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	for (int i = 0; i < (int)EMenuPlayer::END; i++)
	{
		if (m_DriverArray[i] == target || !m_DriverArray[i]->IsVisible())
			continue;

		// ������ ��ǥ�� ���Ѵ�.
		Vector2 radius;
		if (m_LookBack)
			radius = Vector2(cosf(m_InvisiblePlayer->GetAngle()), sinf(m_InvisiblePlayer->GetAngle())) * m_DriverArray[i]->GetVisualRadius();
		else
			radius = Vector2(cosf(target->GetAngle()), sinf(target->GetAngle())) * m_DriverArray[i]->GetVisualRadius();

		Vector2 screen;

		float z;

		// ������� ��ġ�� �����Ѵ�. ���� ��ġ���� �÷��̾ �ٶ󺸰��ִ� ������ ��ǥ�� ���� 
		// ������� ��ġ�� ���� �� �ִ�. ī�޶�� ����� ����.
		Vector2 driverPos = m_DriverArray[i]->GetPos();

		if (MapToScreen(m_LookBack ? m_InvisiblePlayer : target, driverPos - radius, screen, z))
		{
			float driverAngle = 0.0f;
			// �������̸� ���Ѵ�.
			if (m_LookBack)
				driverAngle = m_InvisiblePlayer->GetAngle() - m_DriverArray[i]->GetAngle();
			else
				driverAngle = target->GetAngle() - m_DriverArray[i]->GetAngle();

			// ���� �������̷� � ����� �׷������� ���Ѵ�.
			m_DriverArray[i]->m_Animator.SetViewTexture(driverAngle);
			CTexture* tex = m_DriverArray[i]->GetTexture();

			// ���� ��ũ�� ��ǥ�� ���� �ػ󵵿� ��Ŷ�� ����, �ϴ��� ������ �˸°� �����Ѵ�.
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

			// ���� ���� z�� ����ؼ� �������� �����Ѵ�.
			float scale = 1.0f / (3.6f * logf(1.02f + 0.8f * z));

			// ���� �����ϰ� �ִٸ�
			if (m_DriverArray[i]->GetHeight() > 0.0f)
			{
				// �׸��ڵ� �߰��Ѵ�.
				CTexture* shadow = m_DriverArray[i]->GetShadow();
				// �׸��ڴ� �������ٵ� ���� ���߿� �׷����ϹǷ� ū ���� �ִ´�.
				float zShadow = z + 10000.0f;
				// �׸����� �������� �Ѵ�.
				shadow->SetScale(m_DriverArray[i]->GetTexture()->GetScale() * CIRCUIT_HEIGHT_PCT * 2.0f);
				// y���� �������� ���̰� z�� ������ �޴´�.
				shadow->Scale(scale * screenScale, scale * screenScale / fmaxf(1.0f, z * 5.0f));
				shadow->SetPos(screen);

				vecRenderable.push_back(std::make_pair(std::make_pair(zShadow, shadow), (CWallObject*)m_DriverArray[i]));
			}

			// ���� ȭ���� ��ǥ���� ���� ��ŭ ���ش�.
			screen.y -= m_DriverArray[i]->GetHeight() * scale * screenScale;
			// ������ ����
			tex->Scale(scale * screenScale, scale * screenScale);
			tex->SetPos(screen);
			// �ؽ��ĸ� �������� ���Ϳ� �߰�
			vecRenderable.push_back(std::make_pair(std::make_pair(z, tex), (CWallObject*)m_DriverArray[i]));
		}
	}
}

// ����̹��� �Ȱ��� ������� �׷�����.
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

// �����۵� ����̹��� ���� ���� �Ȱ��� ������� �׷�����.
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
