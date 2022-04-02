#include "DriverAnimator.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"
#include "../Map/MapInfo.h"
#include "../GameManager.h"


CDriverAnimator::CDriverAnimator(const std::string& textureName, const EDriverControlType& control, CRaceScene* scene, bool IsInvisible)
	: m_ControlType(control)
{
	m_Scene = scene;

	if (IsInvisible)
	{
		m_Scene->GetSceneResoruce()->LoadTexture("Invisible", TEXT("Character/Invisible.bmp"));
		m_Scene->GetSceneResoruce()->FindTexture("Invisible")->SetColorKey(255, 0, 255);
		for (int i = 0; i < 12; i++)
		{
			m_LDriving[i] = m_Scene->GetSceneResoruce()->FindTexture("Invisible");
			m_RDriving[i] = m_Scene->GetSceneResoruce()->FindTexture("Invisible");
		}

		for (int i = 0; i < 3; i++)
		{
			m_LDriftTxtParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("Invisible");
			m_RDriftTxtParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("Invisible");
		}

		for (int i = 0; i < 4; i++)
		{
			m_GrassTxtParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("Invisible");
		}

		for (int i = 0; i < 5; i++)
		{
			m_DriftParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("Invisible");
		}

		m_CurrentTexture = m_RDriving[0];
		m_GrassParticle = m_GrassTxtParticles[0];
	}
	else
	{
		m_Scale = 2.0f;

		for (int i = 0; i < 12; i++)
		{
			TCHAR filePath[MAX_PATH] = {};
			std::wstring temp;
			temp.assign(textureName.begin(), textureName.end());
			wsprintf(filePath, TEXT("Character/%s/R%s%d.bmp"), temp.c_str(), temp.c_str(), i + 1);

			m_Scene->GetSceneResoruce()->LoadTexture("R" + textureName + std::to_string(i + 1), filePath);
			m_Scene->GetSceneResoruce()->SetTextureColorKey("R" + textureName + std::to_string(i + 1), 255, 0, 255);
			m_RDriving[i] = m_Scene->GetSceneResoruce()->FindTexture("R" + textureName + std::to_string(i + 1));

			m_RDriving[i]->SetPivot(m_RDriving[i]->GetWidth() / 2.0f, (float)m_RDriving[i]->GetHeight());
			m_RDriving[i]->SetScale(m_Scale, m_Scale);
		}

		for (int i = 0; i < 12; i++)
		{
			TCHAR filePath[MAX_PATH] = {};
			std::wstring temp;
			temp.assign(textureName.begin(), textureName.end());
			wsprintf(filePath, TEXT("Character/%s/L%s%d.bmp"), temp.c_str(), temp.c_str(), i + 1);

			m_Scene->GetSceneResoruce()->LoadTexture("L" + textureName + std::to_string(i + 1), filePath);
			m_Scene->GetSceneResoruce()->SetTextureColorKey("L" + textureName + std::to_string(i + 1), 255, 0, 255);
			m_LDriving[i] = m_Scene->GetSceneResoruce()->FindTexture("L" + textureName + std::to_string(i + 1));

			m_LDriving[i]->SetPivot(m_LDriving[i]->GetWidth() / 2.0f, (float)m_LDriving[i]->GetHeight());
			m_LDriving[i]->SetScale(m_Scale, m_Scale);
		}

		for (int i = 0; i < 3; i++)
		{
			TCHAR filePath[MAX_PATH] = {};

			wsprintf(filePath, TEXT("ETC/LDrift%d.bmp"), i + 1);

			m_Scene->GetSceneResoruce()->LoadTexture("LDrift" + std::to_string(i + 1), filePath);

			m_LDriftTxtParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("LDrift" + std::to_string(i + 1));
			m_LDriftTxtParticles[i]->SetColorKey(255, 0, 255);
			m_LDriftTxtParticles[i]->SetPivot(m_LDriftTxtParticles[i]->GetWidth() / 2.0f, m_LDriftTxtParticles[i]->GetHeight() / 2.0f);
		}

		for (int i = 0; i < 3; i++)
		{
			TCHAR filePath[MAX_PATH] = {};

			wsprintf(filePath, TEXT("ETC/RDrift%d.bmp"), i + 1);

			m_Scene->GetSceneResoruce()->LoadTexture("RDrift" + std::to_string(i + 1), filePath);

			m_RDriftTxtParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("RDrift" + std::to_string(i + 1));
			m_RDriftTxtParticles[i]->SetColorKey(255, 0, 255);
			m_RDriftTxtParticles[i]->SetPivot(m_RDriftTxtParticles[i]->GetWidth() / 2.0f, m_RDriftTxtParticles[i]->GetHeight() / 2.0f);
		}

		for (int i = 0; i < 2; i++)
		{
			TCHAR filePath[MAX_PATH] = {};

			wsprintf(filePath, TEXT("ETC/Grass%d"), i + 1);

			m_Scene->GetSceneResoruce()->LoadTexture("Grass" + std::to_string(i + 1), filePath);

			m_GrassTxtParticles[i] = m_Scene->GetSceneResoruce()->FindTexture("Grass" + std::to_string(i + 1));
			m_GrassTxtParticles[i]->SetColorKey(255, 0, 255);
			m_GrassTxtParticles[i]->SetPivot(m_GrassTxtParticles[i]->GetWidth() / 2.0f, m_GrassTxtParticles[i]->GetHeight() / 2.0f);
		}

		m_GrassParticle = m_GrassTxtParticles[0];
		m_GrassParticle->SetPivot(m_GrassParticle->GetWidth() / 2.0f, m_GrassParticle->GetHeight() / 2.0f);
		m_GrassParticle->SetScale(1.5f, 1.5f);


		{
			m_Scene->GetSceneResoruce()->LoadTexture("RSwamp", TEXT("ETC/RDirt.bmp"));
			m_SwampTxtParticles[0] = m_Scene->GetSceneResoruce()->FindTexture("RSwamp");
			m_SwampTxtParticles[0]->SetPivot(m_SwampTxtParticles[0]->GetWidth() / 2.0f, m_SwampTxtParticles[0]->GetHeight() / 2.0f);
			m_SwampTxtParticles[0]->SetColorKey(255, 0, 255);

			m_Scene->GetSceneResoruce()->LoadTexture("LSwamp", TEXT("ETC/LDirt.bmp"));
			m_SwampTxtParticles[1] = m_Scene->GetSceneResoruce()->FindTexture("LSwamp");
			m_SwampTxtParticles[1]->SetPivot(m_SwampTxtParticles[1]->GetWidth() / 2.0f, m_SwampTxtParticles[1]->GetHeight() / 2.0f);
			m_SwampTxtParticles[1]->SetColorKey(255, 0, 255);
		}


		int groundType = 0;

		for (int i = 0; i < 5; i++)
		{
			m_DriftParticles[i] = m_LDriftTxtParticles[groundType];
		}

		m_CurrentTexture = m_RDriving[0];

		m_SmashTime = 0.0f;
		m_StarTime = 0.0f;


		m_State = EPlayerState::GO_FORWARD;
	}
}

CDriverAnimator::~CDriverAnimator()
{
}

void CDriverAnimator::Init(const std::string& name, CRaceScene* scene)
{
	m_Scene = scene;

	m_Scale = 2.0f;

	for (int i = 0; i < 12; i++)
	{
		TCHAR filePath[MAX_PATH] = {};
		std::wstring temp;
		temp.assign(name.begin(), name.end());
		wsprintf(filePath, TEXT("Character/%s/R%s%d.bmp"), temp.c_str(), temp.c_str(), i + 1);

		m_Scene->GetSceneResoruce()->LoadTexture("R" + name + std::to_string(i + 1), filePath);
		m_Scene->GetSceneResoruce()->SetTextureColorKey("R" + name + std::to_string(i + 1), 255, 0, 255);
		m_RDriving[i] = m_Scene->GetSceneResoruce()->FindTexture("R" + name + std::to_string(i + 1));

		m_RDriving[i]->SetPivot(m_RDriving[i]->GetWidth() / 2.0f, (float)m_RDriving[i]->GetHeight());
		m_RDriving[i]->SetScale(m_Scale, m_Scale);
	}

	for (int i = 0; i < 12; i++)
	{
		TCHAR filePath[MAX_PATH] = {};
		std::wstring temp;
		temp.assign(name.begin(), name.end());
		wsprintf(filePath, TEXT("Character/%s/L%s%d.bmp"), temp.c_str(), temp.c_str(), i + 1);

		m_Scene->GetSceneResoruce()->LoadTexture("L" + name + std::to_string(i + 1), filePath);
		m_Scene->GetSceneResoruce()->SetTextureColorKey("L" + name + std::to_string(i + 1), 255, 0, 255);
		m_LDriving[i] = m_Scene->GetSceneResoruce()->FindTexture("L" + name + std::to_string(i + 1));

		m_LDriving[i]->SetPivot(m_LDriving[i]->GetWidth() / 2.0f, (float)m_LDriving[i]->GetHeight());
		m_LDriving[i]->SetScale(m_Scale, m_Scale);
	}
}

// 전진
void CDriverAnimator::GoForward()
{
	m_Drifting = false;
	m_State = EPlayerState::GO_FORWARD;
}

// 오른쪽
void CDriverAnimator::GoRight(bool drift)
{
	m_Drifting = drift;
	m_State = EPlayerState::GO_RIGHT;
}

// 왼쪽
void CDriverAnimator::GoLeft(bool drift)
{
	m_Drifting = drift;
	m_State = EPlayerState::GO_LEFT;
}

// 낙하
void CDriverAnimator::Fall()
{
	m_Drifting = false;
	m_State = EPlayerState::FALLING;
}

// 깃털
void CDriverAnimator::Feather()
{
	m_Drifting = false;
	m_State = EPlayerState::JUMP;
}

// 피격
void CDriverAnimator::Hit()
{
	m_Drifting = false;
	m_State = EPlayerState::HIT;
}

// 작아짐
void CDriverAnimator::Small(float duration)
{
	m_SmallTime = duration;
}

// 짓눌림
void CDriverAnimator::Smash(float duration)
{
	m_SmashTime = duration;
}

// 별
void CDriverAnimator::Star(float duration)
{
	m_StarTime = duration;
}


// 파티클을 그린다.
void CDriverAnimator::DrawParticles(HDC hDC, CTexture* driverTexture, bool IsSmall, Vector2 position)
{

	Vector2 driverPos = driverTexture->GetPos();
	Vector2 middlePos = Vector2(driverPos.x, driverPos.y);
	Vector2 driverSize = Vector2((float)driverTexture->GetWidth(), (float)driverTexture->GetHeight());

	Vector2 tilePos = Vector2(int(position.x * MAP_TILES_WIDTH), int(position.y * MAP_TILES_HEIGHT));


	ETileMaterial groundType = ETileMaterial::DIRT;

	ETileProperty tile = m_Scene->GetTileMap()->GetTile((int)tilePos.x, (int)tilePos.y)->GetTileProperty();

	// 현재 바닥 타입의 마테리얼을 가져온다.
	switch (tile)
	{
	case ETileProperty::SLOW:
	case ETileProperty::BLOCK:
		groundType = m_Scene->GetTileMap()->GetSlowMat();
		break;
	case ETileProperty::SLOW_2:
		groundType = m_Scene->GetTileMap()->GetSlowMat2();
		break;
	case ETileProperty::HAZARD:
		groundType = m_Scene->GetTileMap()->GetHazardMat();
		break;
	default:
		groundType = m_Scene->GetTileMap()->GetTrackMat();
		break;
	}


	float factor = CGameManager::GetInst()->GetResolution().width / BASIC_WIDTH;

	// 전진상태이면
	if (EPlayerState::GO_FORWARD == m_State)
	{
		// 바닥 마테리얼의 상태에 따라서 다른 파티클을 그린다.
		switch (groundType)
		{
		case ETileMaterial::GRASS:
		{
			if (m_GrassIndex > 5)
				m_GrassParticle = m_GrassTxtParticles[1];
			else
				m_GrassParticle = m_GrassTxtParticles[0];

			m_GrassIndex = (m_GrassIndex + 1) % 10;


			if (IsSmall)
				m_GrassParticle->SetScale(0.5f, 0.5f);
			else
				m_GrassParticle->SetScale(1.0f, 1.0f);

			m_GrassParticle->Scale(factor, factor);

			float posOffset = 0.0f;

			m_GrassParticle->SetPos(middlePos.x + driverSize.x * 0.3f + posOffset,
				middlePos.y - driverSize.y * 0.1f);

			m_GrassParticle->Render(hDC, Vector2(0.0f, 0.0f), Vector2((float)m_GrassParticle->GetWidth(), (float)m_GrassParticle->GetHeight()));

			m_GrassParticle->SetPos(middlePos.x - driverSize.x * 0.3f + posOffset, middlePos.y - driverSize.y * 0.1f);

			m_GrassParticle->Render(hDC, Vector2(0.0f, 0.0f), Vector2((float)m_GrassParticle->GetWidth(), (float)m_GrassParticle->GetHeight()));

		}
			break;
		case ETileMaterial::SWAMP:
		{
			if (IsSmall)
			{
				m_SwampTxtParticles[0]->SetScale(0.5f, 0.5f);
				m_SwampTxtParticles[1]->SetScale(0.5f, 0.5f);
			}
			else
			{
				m_SwampTxtParticles[0]->SetScale(1.0f, 1.0f);
				m_SwampTxtParticles[1]->SetScale(1.0f, 1.0f);
			}

			m_SwampTxtParticles[0]->Scale(factor, factor);
			m_SwampTxtParticles[1]->Scale(factor, factor);

			float posOffset = 0.0f;

			m_SwampTxtParticles[0]->SetPos(middlePos.x + driverSize.x + posOffset,
				middlePos.y + driverSize.y * 0.5f);

			m_SwampTxtParticles[0]->Render(hDC, Vector2(0.0f, 0.0f), Vector2((float)m_SwampTxtParticles[0]->GetWidth(), (float)m_SwampTxtParticles[0]->GetHeight()));

			m_SwampTxtParticles[1]->SetPos(middlePos.x - driverSize.x * 0.3f + posOffset, 
				middlePos.y + driverSize.y * 0.5f);

			m_SwampTxtParticles[1]->Render(hDC, Vector2(0.0f, 0.0f), Vector2((float)m_SwampTxtParticles[1]->GetWidth(), (float)m_SwampTxtParticles[1]->GetHeight()));

		}
			break;

		}
	}

	// 드리프트중
	if (m_Drifting)
	{

		// 바닥의 타입에 따라
		// 다른 색깔의 드리프트 파티클을 보여주려 했으나 winapi한계로 스킵
		int type = 0;
	
		float scale = 1.0f;
		switch (groundType)
		{
		case ETileMaterial::DIRT:
			type = 0;
			break;
		case ETileMaterial::GRASS:
			type = 2;
			scale = 0.7f;
			break;
		case ETileMaterial::SPOOKY_WOOD:
			type = 2;
			scale = 0.7f;
			break;
		case ETileMaterial::STONE:
			type = 1;
			break;
		case ETileMaterial::RAINBOW:
			type = 2;
			break;
		default:
			type = 0;
			break;
		}


		// 드리프트 파티클을 총 4개 그린다.
		for (int i = 0; i < 5; i++)
		{

			float posOffset = 0;

			// 오른쪽으로 갈시에는 오른쪽 드리프트
			if (m_State == EPlayerState::GO_RIGHT)
			{
				m_DriftParticles[i] = m_RDriftTxtParticles[type];


				m_DriftParticles[i]->SetScale(scale, scale);

				if (IsSmall)
					m_DriftParticles[i]->SetScale(0.5f, 0.5f);

				m_DriftParticles[i]->Scale(factor, factor);


				posOffset = -driverSize.x * 0.25f;
			}
			// 왼쪽으로 갈시에는 왼쪽으로 드리프트
			else if (m_State == EPlayerState::GO_LEFT)
			{
				m_DriftParticles[i] = m_LDriftTxtParticles[type];


				m_DriftParticles[i]->SetScale(scale, scale);

				if (IsSmall)
					m_DriftParticles[i]->SetScale(0.5f, 0.5f);

				m_DriftParticles[i]->Scale(factor, factor);

				posOffset = driverSize.x * 0.25f;
			}

			
			if (m_State == EPlayerState::GO_RIGHT)
			{
				m_DriftParticles[i]->SetPos(middlePos.x - driverSize.x * 0.25f + posOffset, middlePos.y + (17.0f * (i + 1)));
				m_DriftParticles[i]->Scale(2.0f + cosf(m_SpriteMovementDriftTime), 2.0f + cosf(m_SpriteMovementDriftTime));
			}
			else
			{
				m_DriftParticles[i]->SetPos(middlePos.x + driverSize.x * 1.2f + posOffset, middlePos.y + (17.0f * (i + 1)));
				m_DriftParticles[i]->Scale(2.0f + cosf(m_SpriteMovementDriftTime), 2.0f + cosf(m_SpriteMovementDriftTime));
			}


			m_DriftParticles[i]->Render(hDC,
				Vector2(0.0f, 0.0f), Vector2((float)m_DriftParticles[i]->GetWidth(), (float)m_DriftParticles[i]->GetHeight()));


			if (m_State == EPlayerState::GO_RIGHT)
			{
				m_DriftParticles[i]->SetPos(middlePos.x + driverSize.x * 0.25f + posOffset, middlePos.y + (17.0f * (i + 1)));
				m_DriftParticles[i]->Scale(2.0f + cosf(m_SpriteMovementDriftTime), 2.0f + cosf(m_SpriteMovementDriftTime));
			}
			else
			{
				m_DriftParticles[i]->SetPos(middlePos.x - driverSize.x * -0.1f + posOffset, middlePos.y + (17.0f * (i + 1)));
				m_DriftParticles[i]->Scale(2.0f + cosf(m_SpriteMovementDriftTime), 2.0f + cosf(m_SpriteMovementDriftTime));
			}


			m_DriftParticles[i]->Render(hDC, 
				Vector2(0.0f, 0.0f), Vector2((float)m_DriftParticles[i]->GetWidth(), (float)m_DriftParticles[i]->GetHeight()));


		}
	}
}

void CDriverAnimator::Update(const float speedForward, const float speedTurn, const float height, const float deltaTime, const Vector2& pos)
{
	// 현재 상황에 맞춰서 애니메이션을 바꾼다.
	switch (m_State)
	{
	// 우회전일 시에는
	case CDriverAnimator::EPlayerState::GO_RIGHT:
	{
		// 회전 속도에 따라 알맞은 애니메이션을 보여준다.
		if (speedTurn < 1.0f * (1.0f / 4))
			m_CurrentTexture = m_RDriving[1];
		else if (speedTurn < 1.0f * (1.0f / 2))
			m_CurrentTexture = m_RDriving[2];
		else if (m_Drifting)
			m_CurrentTexture = m_RDriving[4];
		else
			m_CurrentTexture = m_RDriving[3];
		m_CurrentTexture->SetScale(m_Scale, m_Scale);
	}
		break;
	// 좌회전시에도 마찬가지
	case CDriverAnimator::EPlayerState::GO_LEFT:
	{
		if (speedTurn > -1.0f * (1.0f / 4))
			m_CurrentTexture = m_LDriving[1];
		else if (speedTurn > -1.0f * (1.0f / 2))
			m_CurrentTexture = m_LDriving[2];
		else if (m_Drifting)
			m_CurrentTexture = m_LDriving[4];
		else
			m_CurrentTexture = m_LDriving[3];
		m_CurrentTexture->SetScale(m_Scale, m_Scale);
	}
		break;
	// 직진시에는 좌우 상관없이 0번째 애니메이션을 쓴다.
	case CDriverAnimator::EPlayerState::GO_FORWARD:
	{
		m_CurrentTexture = m_RDriving[0];
		m_CurrentTexture->SetScale(m_Scale, m_Scale);
	}
		break;
	case CDriverAnimator::EPlayerState::GO_BACK:
	{

	}
		break;
	// 추락시에는 점점 스케일이 작아진다.
	case CDriverAnimator::EPlayerState::FALLING:
	{
		m_FallScale = m_FallScale * 0.9f;
		// 그 작아지는 크기만큼 현재 크기에 곱해준다.
		m_CurrentTexture->SetScale(m_Scale * m_FallScale, m_Scale * m_FallScale);
	}
		break;
	// 피격시
	case CDriverAnimator::EPlayerState::HIT:
	{
		// 360도 회전한다.
		m_TexIdx = (m_TexIdx + 2) % NUM_360_TEXTURES;
		m_CurrentTexture = m_RDriving[m_360TexIdx[m_TexIdx]];

		// 만약 절반을 넘어갔다면 180도 돌았다는 뜻 따라서 우에서 좌로 바뀐다.
		if (m_TexIdx > NUM_360_TEXTURES / 2)
		{
			m_CurrentTexture = m_LDriving[m_360TexIdx[m_TexIdx]];
			m_CurrentTexture->SetScale(m_Scale, m_Scale);
		}
		else
			m_CurrentTexture->SetScale(m_Scale, m_Scale);
	}
		break;
	// 깃털로인한 점프 피격과 동일한 애니메이션을 보여주지만 유저 컨트롤이 가능한 상황
	case CDriverAnimator::EPlayerState::JUMP:
	{
		m_TexIdx = (m_TexIdx + 2) % NUM_360_TEXTURES;
		m_CurrentTexture = m_RDriving[m_360TexIdx[m_TexIdx]];

		if (m_TexIdx > NUM_360_TEXTURES / 2)
		{
			m_CurrentTexture = m_LDriving[m_360TexIdx[m_TexIdx]];
			m_CurrentTexture->SetScale(m_Scale, m_Scale);
		}
		else
			m_CurrentTexture->SetScale(m_Scale, m_Scale);
	}
		break;
	default:
		m_CurrentTexture = m_RDriving[0];
		m_CurrentTexture->SetScale(m_Scale, m_Scale);
		break;
	}

	// 만약 드리프트중이라면
	if (m_Drifting)
	{
		// 파티클을 계속해서 바꾼다.
		m_DriftIndex = (m_DriftIndex + 1) % 5;

		if (m_DriftParticles[m_DriftIndex]->GetScale().x > 2.0f)
			m_DriftParticles[m_DriftIndex]->SetScale(0.1f, 0.1f);
		else
			m_DriftParticles[m_DriftIndex]->SetScale(1.3f, 1.3f);
	}
	else
		m_DriftIndex = 0;

	// 만약 현재 번개를 맞아 작아진 상황이라면
	if (m_SmallTime > 0.0f)
	{
		m_SmallTime -= deltaTime;

		m_CurrentTexture->SetScale(1 / 2.0f, 1 / 2.0f);

		// 플레이어면서 번개 시간이 끝이나면은
		if (m_ControlType == EDriverControlType::PLAYER && m_SmallTime <= 0.0f)
		{
			// 다시 자라난다.
			m_Scene->GetSceneResoruce()->SoundPlay("Grow");
		}
	}

	// 짓눌림 상태
	if (m_SmashTime > 0.0f)
	{
		m_SmashTime -= deltaTime;
		m_CurrentTexture->SetScale(1, 1 / 2.0f);

		if (m_ControlType == EDriverControlType::PLAYER && m_SmashTime <= 0.0f)
		{
			m_Scene->GetSceneResoruce()->SoundPlay("Grow");
		}
	}

	// 무적상태
	if (m_StarTime > 0.0f)
	{
		m_StarTime -= deltaTime;

		if (m_ControlType == EDriverControlType::PLAYER && m_StarTime <= 0.0f)
		{
			m_Scene->GetSceneResoruce()->SoundStop("Star");
		}
	}


	// 플레이어가 드리프트 중일때 흔들림 효과를 준다.
	if (height <= 0.0f && m_Drifting)
	{
		// 20.0f 단위로 계속해서 반복해서 시간을 바꾼다.
		m_SpriteMovementDriftTime = fmodf(m_SpriteMovementDriftTime + 1.0f, MOVEMENT_DRIFT_PERIOD);
	}
	// 천천히 감소시킨다.
	else
	{
		if (m_SpriteMovementDriftTime > 0.001f)
			m_SpriteMovementDriftTime /= 1.5f;
		else
			m_SpriteMovementDriftTime = 0.0f;
	}

	// 플레이어가 직진으로 움직일시에는 위 아래로 조금씩 움직이게한다.
	if (height <= 0.0f && speedForward > 0.0f)
	{
		m_SpriteMovementSpeedTime = fmodf(m_SpriteMovementSpeedTime + speedForward * deltaTime, MOVEMENT_SPEED_PERIOD);
	}
	// 천천히 감소시킨다.
	else
	{
		if (m_SpriteMovementSpeedTime > 0.001f)
			m_SpriteMovementSpeedTime /= 1.5f;
		else
			m_SpriteMovementSpeedTime = 0.0f;
	}

	Vector2 tilePos = Vector2(int(pos.x * MAP_TILES_WIDTH), int(pos.y * MAP_TILES_HEIGHT));


	ETileProperty tile = m_Scene->GetTileMap()->GetTile((int)tilePos.x, (int)tilePos.y)->GetTileProperty();

	// 현재 타일의 성질에 따라
	switch (tile)
	{
	case ETileProperty::SLOW:
	case ETileProperty::SLOW_2:
	{
		m_OnSwamp |= m_Scene->GetTileMap()->GetSlowMat() == ETileMaterial::SWAMP;
		m_OnSwamp |= m_Scene->GetTileMap()->GetSlowMat2() == ETileMaterial::SWAMP;
	}
	break;
	default:
		m_OnSwamp = false;
		break;
	}


	
	m_SpriteMovementDrift = sinf(m_SpriteMovementDriftTime * 2.0f * (float)M_PI / MOVEMENT_DRIFT_PERIOD) * MOVEMENT_DRIFT_AMPLITUDE;

	// 늪지에 있을떄는 더 위아래로 흔들리게한다.
	if(!m_OnSwamp)
		m_SpriteMovementSpeed = sinf(m_SpriteMovementSpeedTime * 2.0f * (float)M_PI / MOVEMENT_SPEED_PERIOD) * MOVEMENT_SPEED_AMPLITUDE;
	else
		m_SpriteMovementSpeed = sinf(m_SpriteMovementSpeedTime * 2.0f * (float)M_PI / MOVEMENT_SPEED_PERIOD) * MOVEMENT_SPEED_AMPLITUDE * 3.0f;
}

// 현재 피격상태가 아니라면 조종이가능
bool CDriverAnimator::CanDrive() const
{
	return m_State != EPlayerState::HIT;
}

// 각도에 맞는 텍스쳐를 가져온다.
void CDriverAnimator::SetViewTexture(float angle)
{
	// 각도는 항상 0 ~ 2pi 범위로 한정
	angle = fmodf(angle, 2.0f * (float)M_PI);

	// 만약 0보다 작을시에는 더해준다. 
	if (angle < 0)
		angle += 2.0f * (float)M_PI;

	// 피격 상태가 아니라면
	if (m_State != EPlayerState::HIT)
	{
		// 0번 부터 시작해서 각도를 찾아낸다.
		for (int i = 1; i <= NUM_360_TEXTURES; i++)
		{
			// 각도를 라디안으로 바꾸는 식을 이용한다.
			// 만약 현재 각도가 0.5f 단위로 라디안으로 바꿨을때와 같거나 작다면
			if (angle <= ((i - 0.5f) * 2.0f * (float)M_PI) / (float)NUM_360_TEXTURES)
			{
				// 만약 180도 돌았다면
				if (angle > (float)M_PI)
					m_CurrentTexture = m_RDriving[m_360TexIdx[i - 1]];
				// 아니라면
				else
					m_CurrentTexture = m_LDriving[m_360TexIdx[i - 1]];
				break;
			}
		}
	}
	else
	{
		// 피격시에는 360도 회전한다.
		if (m_TexIdx > NUM_360_TEXTURES / 2)
			m_CurrentTexture = m_LDriving[m_360TexIdx[m_TexIdx]];
		else
			m_CurrentTexture = m_RDriving[m_360TexIdx[m_TexIdx]];
	}

	// 현재 서킷의 높이비율만큼 곱해준다.
	m_CurrentTexture->SetScale(m_Scene->GetCircuitHeightPCT(), m_Scene->GetCircuitHeightPCT());

	if (m_SmallTime > 0.0f)
		m_CurrentTexture->Scale(0.5f, 0.5f);
	
	if (m_SmashTime > 0.0f)
		m_CurrentTexture->Scale(1.0f, 0.5f);
}

// 미니맵상의 텍스쳐를 가져온다.
CTexture CDriverAnimator::GetMinimapTexture(float angle, const float screenScale) const
{
	// 미니맵에서는 기본 라디안은 90도 즉 우측을 바라보고 있는 상태로 정한다.
	angle += (float)M_PI / 2;

	// 다시한번 0 ~ 2pi 범위로 전환
	angle = fmodf(angle, 2.0f * (float)M_PI);

	if (angle < 0)
		angle += 2.0f * (float)M_PI;

	CTexture minimapTexture;
	int i;

	// 동일한 방법으로 텍스쳐를 가져온다.
	if (m_State != EPlayerState::HIT)
	{
		for (i = 1; i <= NUM_360_TEXTURES + 1; i++)
		{
			if (angle <= ((i - 0.5f) * 2.0f * (float)M_PI) / (float)NUM_360_TEXTURES)
			{
				if (angle < (float)M_PI)
					minimapTexture = *m_RDriving[m_360TexIdx[i - 1]];
				else				
					minimapTexture = *m_LDriving[m_360TexIdx[i - 1]];
				break;
			}
		}
	}
	else
	{
		if (m_TexIdx > NUM_360_TEXTURES / 2)
		{
			minimapTexture = *m_LDriving[m_360TexIdx[m_TexIdx]];
		}
		else
		{
			minimapTexture = *m_RDriving[m_360TexIdx[m_TexIdx]];
		}
	}

	if (m_SmallTime > 0.0f)
		minimapTexture.Scale(0.5f, 0.5f);

	if (m_SmashTime > 0.0f)
		minimapTexture.Scale(1.0f, 0.5f);

	return minimapTexture;
}

void CDriverAnimator::Reset()
{
	m_CurrentTexture = m_RDriving[0];

	m_SmashTime = 0.0f;
	m_StarTime = 0.0f;
	m_SmallTime = 0.0f;
	m_Scale = 2.0f;
	m_FallScale = 1.0f;

	m_TexIdx = 0;
	m_DriftIndex = 0;
	m_Drifting = false;

	m_State = EPlayerState::GO_FORWARD;
	m_CurrentTexture->SetScale(m_Scale, m_Scale);
}
