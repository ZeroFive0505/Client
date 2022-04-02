#include "Driver.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Timer.h"
#include "../Scene/RaceScene.h"
#include "../Map/MapInfo.h"
#include "../Scene/SceneResource.h"
#include "../UI/RaceGUI.h"
#include "../AI/AIGradientDescent.h"
#include "../Scene/CollisionHash.h"
#include "../Object/Lakitu.h"

// 각도를 0 ~ Pi 범위로 반환한다.
float NormalizeAngle(float angle)
{
	float normlizedAngle = angle;

	// 만약 각도가 2pi이상이라면 2pi만큼 뺀다.
	while (normlizedAngle >= 2 * (float)M_PI)
		normlizedAngle -= 2 * (float)M_PI;

	// 반약 0보다 작다면 2pi만큼 더한다.
	while (normlizedAngle < 0)
		normlizedAngle += 2 * (float)M_PI;

	return normlizedAngle;
}

// 속도 그래프를 시뮬레이션해서 속도를 계산한다.
void SimulateSpeedGraph(CDriver* self, float& accelerationLinear)
{
	// 중량급 드라이버시
	if (self->m_Kart->m_Convex)
	{
		// 현재 속도가 최대속도에 몇 퍼센트인지를 구한다.
		float speedPercentage = self->m_SpeedForward / self->m_Kart->m_MaxNormalLinearSpeed;

		// 만약 25퍼센트 아래라면
		if (speedPercentage < 0.25f)
		{
			// 선형속도는 가속도의 2분의 1배 만큼 증가
			accelerationLinear += self->m_Kart->m_MotorAcceleration / 2.0f;
		}
		// 만약 25퍼센트보단 크고 45퍼센트보단 아래라면
		else if (speedPercentage < 0.45f)
		{
			// 선형속도는 가속도에서 스피드 퍼센트에 0.075f를 더한만큼 곱해서 늘려준다.
			accelerationLinear += self->m_Kart->m_MotorAcceleration * (speedPercentage + 0.075f);
		}
		// 만약 45퍼센트보단 크고 95퍼센트보단 작다면
		else if (speedPercentage < 0.95f)
		{
			accelerationLinear += self->m_Kart->m_MotorAcceleration / 2.0f;
		}
		// 그 외
		else
		{
			accelerationLinear += (0.05 * self->m_Kart->m_MaxNormalLinearSpeed) / 4.0f;
		}
	}
	// 일반 드라이버시
	else
	{
		float speedPercentage = self->m_SpeedForward / self->m_Kart->m_MaxNormalLinearSpeed;

		if (speedPercentage < 0.45f)
		{
			accelerationLinear += self->m_Kart->m_MotorAcceleration / 2.0f;
		}
		else if (speedPercentage < 0.95f)
		{
			accelerationLinear += self->m_Kart->m_MotorAcceleration * (1.0f - speedPercentage);
		}
		else
		{
			accelerationLinear += (0.05f * self->m_Kart->m_MaxNormalLinearSpeed) / 4.0f;
		}
	}
}

// 회전속도 변환
bool IncreasingAngularAceleration(CDriver* self, float& accelerationAngular)
{
	// 일단 드리프트는 거짓으로 설정
	bool drifting = false;

	// 만약 드리프트가 눌려졌으며
	if (self->m_DriftPressed)
	{
		// 현재 방향을 꺾고있다면(시계방향)
		if (CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold))
		{
			// 각속도는 양수로 늘어난다.
			self->m_SpeedTurn = self->m_Kart->m_MaxTurningAngularSpeed * 0.41f;
		}
		// 반시계 방향이라면
		else if (CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold))
		{
			// 각속도는 음수로 줄어든다.
			self->m_SpeedTurn = -1.0f * self->m_Kart->m_MaxTurningAngularSpeed * 0.41f;
		}

		// 드리프트 초기화
		self->m_DriftPressed = false;
	}

	// 만약 각속도와 속도가 일정 수준 이상이라면
	if (std::fabs(self->m_SpeedTurn) > (self->m_Kart->m_MaxTurningAngularSpeed * 0.4f) &&
		std::fabs(self->m_SpeedForward) > (self->m_Kart->m_MaxNormalLinearSpeed * 0.4f))
	{
		// 드리프트를 시작한다.
		accelerationAngular = self->m_Kart->m_TurningAcceleration * 1.0f;
		drifting = true;
	}
	else
		accelerationAngular = self->m_Kart->m_TurningAcceleration * 0.15f;
	return drifting;
}

// 코너링시에 속도가 감소한다.
void ReduceLinearSpeedWhileTurning(CDriver* self, float& accelerationLinear, float& speedTurn)
{
	// 회전속도와 최대 회전 속도를 나눈다.
	float speedTurnPercentage = std::fabs(speedTurn / self->m_Kart->m_MaxTurningAngularSpeed);

	// 만약 현재 속도가 최대 90퍼 보다 높다면
	if (self->m_SpeedForward > self->m_Kart->m_MaxNormalLinearSpeed * 0.9f)
	{
		// 서서히 감속시킨다.
		accelerationLinear = -1.0f * self->m_Kart->m_MotorAcceleration * speedTurnPercentage;
	}
}


// 타일의 벽과 충돌 했을시
void HandleHitBlock(CDriver* self, const Vector2& nextPos)
{
	Vector2 moveWidth = Vector2(1.0f / MAP_TILES_WIDTH, 0.0f);
	Vector2 moveHeight = Vector2(0.0f, 1.0f / MAP_TILES_HEIGHT);


	CTileMap* currentTileMap = self->GetScene()->GetTileMap();

	if (currentTileMap)
	{
		int widthSize = 0;
		// -1, 1로 순회한다.
		for (int j = -1; j <= 1; j += 2)
		{
			for (int i = 1; i <= 4; i++)
			{
				// temp에는 nextPos + moveWidth * (-4 ~ -1), (1, 4)까지의 값을 더한다.
				Vector2 temp = nextPos + moveWidth * float(i * j);

				// 만약 살짝 더하거나 뺀 값에 벽이 위치했다면 크기를 늘린다.
				if (currentTileMap->GetTile(int(temp.x * MAP_TILES_WIDTH), int(temp.y * MAP_TILES_HEIGHT))->GetTileProperty() == ETileProperty::BLOCK)
					widthSize++;
				else
					break;
			}
		}

		// 이번엔 세로로 확인한다.
		int heightSize = 0;
		for (int j = -1; j <= 1; j += 2)
		{
			for (int i = 1; i <= 4; i++)
			{
				Vector2 temp = nextPos + moveHeight * float(i * j);

				if (currentTileMap->GetTile(int(temp.x * MAP_TILES_WIDTH), int(temp.y * MAP_TILES_HEIGHT))->GetTileProperty() == ETileProperty::BLOCK)
					heightSize++;
				else
					break;
			}
		}

		// 충돌 모멘텀을 구한다.
		float momentumSpeed = sqrtf(powf(self->m_CollisionMomentum.x, 2.0f) + powf(self->m_CollisionMomentum.y, 2.0f));

		float factor;
		float angle;

		// 만약 모멘텀이 없다면 그대로 유지된다.
		if (momentumSpeed == 0.0f)
		{
			factor = self->m_SpeedForward;
			angle = self->GetAngle();
		}
		// 아니라면
		else
		{
			factor = momentumSpeed;
			// 각도는 새롭게 갱신이된다.
			angle = atan2f(self->m_CollisionMomentum.y, self->m_CollisionMomentum.x);
		}

		// 모멘텀 초기화
		self->m_CollisionMomentum = Vector2(0.0f, 0.0f);

		// 무적일시에는 거의 최대속도가 그대로 유지된다.
		if (self->IsImmune())
			factor = fmax(factor, self->m_Kart->m_MaxNormalLinearSpeed * 0.97f);
		// 아니라면 절반이 감속된다.
		else
			factor = fmax(factor, self->m_Kart->m_MaxNormalLinearSpeed * 0.5f);


		// 모멘텀을 구한다.
		Vector2 momentum = Vector2(cosf(angle), sinf(angle)) * fmax(0.01f, factor);

		// 양옆 막혀있고 위아래는 뚫려있을 경우는 -y
		if (widthSize > 4 && heightSize < 4)
			self->m_VectorialSpeed = Vector2(momentum.x, -momentum.y);
		// 위아래가 막혀있고 양옆이 뚫려있을 경우는 -x
		else if (widthSize < 4 && heightSize > 4)
			self->m_VectorialSpeed = Vector2(-momentum.x, momentum.y);
		// 그 외 상황에는 정 반대의 방향
		else
			self->m_VectorialSpeed = Vector2(-momentum.x, -momentum.y);
	}

}


// 타일 맵과 충돌 감지
void ImprovedCheckOfMap(CDriver* self, const Vector2& pos, Vector2& deltaPos)
{
	Vector2 nextPos = pos + deltaPos;

	// 아주 작은 오프셋 값을 설정한다. 맵 타일의 크기는 8이고 전체 맵의 크기는 1024, 거기서 3.5f로 나눠 아주 작은 값
	float halfTileWidthInMapCoord = float(MAP_TILE_SIZE) / MAP_ASSETS_WIDTH / 3.5f;
	float halfTileHeightInMapCoord = float(MAP_TILE_SIZE) / MAP_ASSETS_HEIGHT / 3.5f;

	// 현재 플레이어가 바라보고 있는 방향에서 각도의 오프셋 값들
	// 0(직선), M_PI_2(시계 방향으로 90도 틀었을때)....
	float deltaAngle[5] = { 0, (float)M_PI_2, (float)-M_PI_2, (float)M_PI_4, (float)-M_PI_4 };

	for (int i = 0; i < 5; i++)
	{
		// 현재 플레이어가 바라보고있는 방향에서 살짝 각도의 값에 변화를 준다.
		Vector2 shifting = Vector2(cosf(self->GetAngle() + deltaAngle[i]) * halfTileWidthInMapCoord,
			sinf(self->GetAngle() + deltaAngle[i]) * halfTileHeightInMapCoord);

		CTileMap* currentTileMap = self->GetScene()->GetTileMap();

		if (currentTileMap)
		{
			Vector2 testPos = nextPos + shifting;

			ETileProperty tile = currentTileMap->GetTile(int(testPos.x * MAP_TILES_WIDTH), int(testPos.y * MAP_TILES_HEIGHT))->GetTileProperty();

			// 만약 임시좌표가 충돌할 위치라면
			switch (tile)
			{
			case ETileProperty::BLOCK:
				HandleHitBlock(self, testPos);
				// 모든 속도 상승 상태를 제거
				self->PopStateEnd(EDriverState::SPEED_UP);
				self->PopStateEnd(EDriverState::MORE_SPEED_UP);

				if (self->m_ControlType == EDriverControlType::PLAYER)
				{
					// 충돌 소리 재생
					self->GetScene()->GetSceneResoruce()->SoundPlay("Crash");
				}

				// 충돌 이펙트 생성
				self->GetScene()->AddCollisionEffect(pos);

				self->m_SpeedForward = 0.0f;
				self->m_CollisionMomentum = Vector2(0.0f, 0.0f);
				deltaPos = Vector2(0.0f, 0.0f);
				return;
			case ETileProperty::HAZARD:
				self->GetScene()->GetSceneResoruce()->SoundStop("Star");
				self->GetScene()->GetSceneResoruce()->SoundStop("Skid");
				self->m_Falling = true;
				return;
			default:
				break;
			}

		}
	}
}

void CDriver::Start()
{
}

void CDriver::Init()
{
}

CDriver::CDriver(const std::string& textureName, CRaceScene* scene, const Vector2& position,
	const float angle, const int mapWidth, const int mapHeight,
	const EDriverControlType controlType,
	const CKartProperties& kart, const EMenuPlayer player,
	const std::array<CDriver*, (int)EMenuPlayer::END>& positions, bool IsRealPlayer,
	int farVisionMod, float itemProbMod,
	unsigned int impedimentMod, bool IsInvisible) :
	CWallObject(position, 1.0f, DRIVER_HITBOX_RADIUS, 0.0f, mapWidth, mapHeight, scene, textureName),
	m_Angle(angle),
	m_CurrentPlayer(player),
	m_SpeedForward(0.0f),
	m_SpeedTurn(0.0f),
	m_SpeedUpwards(0.0f),
	m_CollisionMomentum(0.0f, 0.0f),
	m_ControlType(controlType),
	m_Kart(&kart),
	m_Positions(positions),
	m_IsRealPlayer(IsRealPlayer),
	m_FarVisionModifier(farVisionMod),
	m_ItemProbModifier(itemProbMod),
	m_ImpedimentModifier(impedimentMod),
	m_DeletePosition(false),
	m_Animator(textureName, controlType, m_Scene)
{
}

CDriver::CDriver(const std::string& textureName, CRaceScene* scene, const Vector2& position, const float angle,
	const int mapWidth, const int mapHeight, const EDriverControlType controlType, const CKartProperties& kart,
	const EMenuPlayer player, bool IsInvisible) :
	CWallObject(position, 1.0f, DRIVER_HITBOX_RADIUS, 0.0f, mapWidth, mapHeight, scene, textureName),
	m_Angle(angle),
	m_CurrentPlayer(player),
	m_SpeedForward(0.0f),
	m_SpeedTurn(0.0f),
	m_SpeedUpwards(0.0f),
	m_CollisionMomentum(0.0f, 0.0f),
	m_ControlType(controlType),
	m_Kart(&kart),
	m_Positions(*(new std::array<CDriver*, (int)EMenuPlayer::END>())),
	m_IsRealPlayer(false),
	m_DeletePosition(true),
	m_Animator(textureName, controlType, m_Scene)
{
}

CDriver::~CDriver()
{
	if (m_DeletePosition)
		delete& m_Positions;
}


// 상태 플래그를 세운다.
void CDriver::PushStateEnd(EDriverState state, const float endTime)
{
	// OR로 비트를 설정하고
	this->m_CurrentState |= (int)state;
	// 상태를 log2를 통해서 바로 구해낸다. (state 0, 1, 2, 4, 8...) 
	m_StateEnd[(int)log2((int)state)] = endTime;
}

// 상태 플래그를 제거한다.
void CDriver::PopStateEnd(EDriverState state)
{
	// AND NOT으로 해당 상태만 제거하고 
	this->m_CurrentState &= ~(int)state;
	// 시간도 초기화
	m_StateEnd[(int)log2((int)state)] = 0.0f;
}

// 모든 상태를 순회하면서 적당한 시간이 흘렀다면 제거한다.
int CDriver::PopStateEnd(const float currentTime)
{
	// 끝난 상태들
	int finishedStates = 0;

	if (this->m_CurrentState != 0)
	{
		// 1, 2, 4, 8, 16 ....
		int state = 1;

		for (int i = 0; i < (int)EDriverState::MAX; i++)
		{
			if (this->m_CurrentState & state)
			{
				// 만약 충분한 시간이 흘렀다면
				if (m_StateEnd[i] < currentTime)
				{
					// 끝난 상태에는 추가하고
					finishedStates |= state;
					// 현재 상태에서는 제거한다.
					this->m_CurrentState &= ~state;
				}
			}

			state *= 2;
		}
	}

	return finishedStates;
}

// 유저 컨트롤
void CDriver::PlayerControl(float& accelerationLinear)
{
	// 만약 공중에 떠 있지 않고
	if (m_Height == 0.0f)
	{
		// 악셀을 누르고있다면 속도 그래프 공식을 이용하여 속도를 갱신한다.
		if (CInput::GetInst()->GetKeyState('X', EKeyState::KeyState_Hold))
			SimulateSpeedGraph(this, accelerationLinear);


		// 브레이크는 거짓으로
		bool IsBrake = false;

		// 만약 현재 플레이어가 브레이크를 누르고있으며
		if (CInput::GetInst()->GetKeyState('Z', EKeyState::KeyState_Hold))
		{
			// 속도가 일정 속도 이상이라면
			if (m_SpeedForward > 0.2f * m_Kart->m_MaxNormalLinearSpeed && !IsBrake)
			{
				// 브레이크를 참으로
				IsBrake = true;
				// 브레이크 소리 재생
				if(!m_Scene->GetSceneResoruce()->IsPlaying("Brake"))
					m_Scene->GetSceneResoruce()->SoundPlay("Brake");
			}
			// 속도는 감속
			accelerationLinear += BREAK_ACELERATION;
		}
		else
		{
			// 아닐경우 브레이크는 정지
			IsBrake = false;
			m_Scene->GetSceneResoruce()->SoundStop("Brake");
		}
	}

	bool drift = false;

	// 만약 플레이어가 현재 왼쪽으로 방향을 틀고있다면
	if (CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) &&
		!CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold))
	{
		float accelerationAngular = 0.0f;
		// 회전속도를 갱신하고 만약 회전속도가 일정 수치 이상이라면 드리프트 실행
		drift = IncreasingAngularAceleration(this, accelerationAngular);

		// 속도는 감속시킨다.
		m_SpeedTurn = std::fmaxf(m_SpeedTurn - accelerationAngular,
			m_Kart->m_MaxTurningAngularSpeed * -1.0f);

		// 코너링중 회전속도의 감속
		ReduceLinearSpeedWhileTurning(this, accelerationLinear, m_SpeedTurn);

		// 만약 깃털아이템을 쓰고있지 않다면 애니메이션을 바꾼다.
		if(!m_JumpByFeather)
			m_Animator.GoLeft(drift);
	}
	else if (CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold) &&
		!CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold))
	{
		float accelrationAngular = 0.0f;
		drift = IncreasingAngularAceleration(this, accelrationAngular);
		m_SpeedTurn = std::fminf(m_SpeedTurn + accelrationAngular, m_Kart->m_MaxTurningAngularSpeed);
		ReduceLinearSpeedWhileTurning(this, accelerationLinear, m_SpeedTurn);

		if(!m_JumpByFeather)
			m_Animator.GoRight(drift);
	}

	// 플레이어의 경우
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		if (drift && m_Height == 0.0f && !m_Falling)
		{
			// 드리프트 사운드 재생
			if (!m_Scene->GetSceneResoruce()->IsPlaying("Skid"))
				m_Scene->GetSceneResoruce()->SoundPlay("Skid");
		}
		else
		{
			// 드리프트 사운드 중지
			if(m_Scene->GetSceneResoruce()->IsPlaying("Skid"))
				m_Scene->GetSceneResoruce()->SoundStop("Skid");
		}
	}
}

void CDriver::UseGradientControl(float& accelerationLinear)
{
	Vector2 dirSum(0.0f, 0.0f);
	
	// 앞으로 몇 타일을 내다볼지 정한다.
	int tilesForward = m_SpeedForward < m_Kart->m_MaxNormalLinearSpeed / 4.0f ? 
		1 : m_Scene->GetTileMap()->GetAIFarVision() + m_FarVisionModifier;

	for (int i = 0; i < tilesForward; i++)
	{
		dirSum += CAIGradientDescent::GetNextDirection(m_Position + dirSum);
	}

	// 벽을 회피하는 벡터
	Vector2 evadeVector;

	bool evadeFound = false;

	// 현재 방향과 속도로 벡터를 구한다.
	Vector2 scaledForward(cosf(m_Angle + m_SpeedTurn * 0.15f) * m_SpeedForward * 0.06f,
		sinf(m_Angle + m_SpeedTurn * 0.15f) * m_SpeedForward * 0.06f);

	// 만약 속도가 일정 수치 이상이라면
	if (m_SpeedForward / m_Kart->m_MaxNormalLinearSpeed > 0.3f)
	{
		// 회피를 할 상황인지 구한다.
		for (int i = 1; i < 12; i++)
		{
			if (CCollisionHash::GetInst()->Evade(this, m_Position + scaledForward * (float)i, m_HitboxRadius * 2.0f, evadeVector))
			{
				evadeFound = true;
				break;
			}
		}
	}

	// 회피 연산
	float evadeAngle = 0.0f;

	if (evadeFound)
	{
		// 수직인 벡터를 구한다.
		Vector2 perpendicularDirection(scaledForward.y, scaledForward.x * -1.0f);

		// 두 벡터간의 유사도를 구한다.
		float dotProduct = perpendicularDirection.x * evadeVector.x + perpendicularDirection.y * evadeVector.y;

		// 장애물과의 거리를 구한다.
		float evadeModule = sqrtf(fabsf(evadeVector.x * evadeVector.x + evadeVector.y * evadeVector.y));

		// 회피거리 이내라면
		if (evadeModule < MAX_EVADE_DISTANCE)
		{
			// 회피 비율을 구한다.
			float evadePct = 1.0f - (evadeModule / MAX_EVADE_DISTANCE);

			// 만약 유사도가 음수라면 시계방향으로 더 꺾는다.
			// 만약 유사도가 양수라면 반시계 방향으로 더 꺾는다.
			evadeAngle = evadePct * MAX_EVADE_ANGLE * (dotProduct > 0.0f ? -1.0f : 1.0f);
		}
	}

	CTileMap* tileMap = m_Scene->GetTileMap();

	if (tileMap)
	{
		// 떨어질 가능성도 있는지 확인한다.
		bool IsGoingToFall = false;

		CTile* tile = tileMap->GetTile(m_Position + scaledForward * 16.0f);
		
		if (tile == nullptr)
			IsGoingToFall = true;

		if (tile && tile->GetTileProperty() == ETileProperty::HAZARD)
			IsGoingToFall = true;

		tile = tileMap->GetTile(m_Position + scaledForward * 8.0f);
		
		if (tile == nullptr)
			IsGoingToFall = true;

		if (tile && tile->GetTileProperty() == ETileProperty::HAZARD)
			IsGoingToFall = true;

		// 가고자 하는 위치의 방향을 구한다.
		float targetAngle = std::atan2f(dirSum.y, dirSum.x);

		// 플레이어간의 각도
		float angleP2P = 0.0f;
		// 방해 계수
		float goHitBackMultiplier = 1.0f;

		// 만약 뒤에 드라이버가 있을경우
		if (m_Rank >= 0 && m_Rank < (int)EMenuPlayer::END - 1)
		{
			// 바로 뒤 드라이버를 구한다.
			const CDriver* backPlayer = m_Positions[m_Rank + 1];

			// 플레이어일 경우와 아닐 경우 다른 값으로 초기화한다.
			goHitBackMultiplier = backPlayer->m_ControlType == EDriverControlType::PLAYER ? 1.0f : 8.0f;

			// 자기의 위치에서 바로 뒤 드라이버의 위치까지의 벡터를 구한다.
			Vector2 vecP2P = m_Position - backPlayer->GetPos();

			// 거리를 구한다.
			float dP2P_2 = vecP2P.x * vecP2P.x + vecP2P.y * vecP2P.y;

			const int NUM_TILES_FOR_OCCLUSION = 6;
			const float DIST_FOR_OCCLUSION = (NUM_TILES_FOR_OCCLUSION / (float)MAP_TILES_WIDTH) * (NUM_TILES_FOR_OCCLUSION / (float)MAP_TILES_WIDTH);

			// 만약 거리가 일정 거리 이하라면 충돌할 거리
			if (dP2P_2 < DIST_FOR_OCCLUSION)
			{
				// 각도를 구한다.
				angleP2P = atan2f(vecP2P.y, vecP2P.x);
				angleP2P = angleP2P - m_Angle;
				angleP2P = NormalizeAngle(angleP2P);

				// 만약 90도 보다 크거나 270보다 작다면
				if (angleP2P > (float)M_PI_2 && angleP2P < 3.0f * M_PI_2)
				{
					angleP2P = 0.0f;
				}
				
				// 만약 뒤의 차량이 무적이면
				if (backPlayer->IsImmune())
				{
					// 달아난다.
					angleP2P = NormalizeAngle(-angleP2P);
				}
			}
		}

		float goHitBackPlayer = (angleP2P < (float)M_PI ? -1.0f : 1.0f) * angleP2P / (float)m_ImpedimentModifier;

		float diff = targetAngle + evadeAngle - m_Angle - m_SpeedTurn * 0.15f + goHitBackPlayer / goHitBackMultiplier;

		diff = fmodf(diff, 2.0f * (float)M_PI);

		if (diff < 0.0f)
			diff += 2.0f * (float)M_PI;

		// 직선코스에서는 가속한다.
		if (m_Height <= 0.0f && fabsf((float)M_PI - diff) > 0.85f * (float)M_PI)
		{
			if (m_IsRealPlayer && m_SpeedForward >= 0.4f * m_Kart->m_MaxNormalLinearSpeed)
			{
				// 러버 밴딩
				const CDriver* player = m_Scene->GetPlayer();
				long long playerPosValue = CAIGradientDescent::GetPositionValue(player->GetPos()) +
					CAIGradientDescent::MAX_POSITION_MATRIX * player->m_Laps;

				long long currentPosValue = CAIGradientDescent::GetPositionValue(m_Position) + CAIGradientDescent::MAX_POSITION_MATRIX * m_Laps;

				// 나중에는 메뉴에서 정한 CC 옵션을 적용하기
				int maxBehind = CAIGradientDescent::MAX_DISTANCE_BEHIND[(int)m_CCoption];

				int minProbBehind = CAIGradientDescent::MIN_PROB_BEHIND[(int)m_CCoption];

				int maxAhead = CAIGradientDescent::MAX_DISTANCE_AHEAD[(int)m_CCoption];

				float minProbAhead = CAIGradientDescent::MIN_PROB_AHEAD[(int)m_CCoption];

				long long distance = std::abs(playerPosValue - currentPosValue);

				// 현재 AI가 더 순위가 낮을 경우
				if (m_Rank > player->m_Rank)
				{
					float variance = 1.0f - minProbBehind;

					float prob = (((float)distance / maxBehind) * variance) + minProbBehind;

					prob = fmaxf(0.5f, fminf(prob, 1.0f));

					if (rand() / (float)RAND_MAX <= prob)
					{
						SimulateSpeedGraph(this, accelerationLinear);
						m_Animator.GoForward();
					}
				}
				else
				{
					// 앞서나가고 있을 경우
					float variance = minProbBehind - minProbAhead;

					float prob = ((1.0f - (float)distance / maxAhead) * variance) + minProbAhead;

					prob = fmaxf(0.4f, fminf(prob, 0.5f));

					if (rand() / (float)RAND_MAX <= prob)
					{
						SimulateSpeedGraph(this, accelerationLinear);
						m_Animator.GoForward();
					}
				}
			}
			else
			{
				SimulateSpeedGraph(this, accelerationLinear);
				m_Animator.GoForward();
			}
		}

		if (diff >= 0.05f * (float)M_PI && diff <= 1.95f * (float)M_PI)
		{
			float accelerationAngular = m_Kart->m_TurningAcceleration;
			float turnMultiplier = IsGoingToFall ? 5.0f : 2.0f;
			float totalMultiplier = IsGoingToFall ? 1.5f : 1.0f;

			// 왼쪽으로 꺾는다.(반 시계)
			if (diff > (float)M_PI)
			{
				m_SpeedTurn = std::fmaxf(m_SpeedTurn - accelerationAngular * turnMultiplier,
					m_Kart->m_MaxTurningAngularSpeed * -totalMultiplier);

				ReduceLinearSpeedWhileTurning(this, accelerationLinear, m_SpeedTurn);

				m_Animator.GoLeft(m_SpeedUpwards < m_Kart->m_MaxTurningAngularSpeed * -0.4f);
			}
			// 오른쪽으로 꺾는다.(시계)
			else
			{
				m_SpeedTurn = std::fminf(m_SpeedTurn + accelerationAngular * turnMultiplier,
					m_Kart->m_MaxTurningAngularSpeed * totalMultiplier);

				ReduceLinearSpeedWhileTurning(this, accelerationLinear, m_SpeedTurn);

				m_Animator.GoRight(m_SpeedTurn > m_Kart->m_MaxTurningAngularSpeed * 0.4f);
			}
		}
	}

}

void CDriver::UpdateGradientPosition()
{
	// 변화량 업데이트
	const int CONSECUTIVE_INCREMENTS_FOR_BACKWARDS = 5;

	// 경계선 검사
	if (m_Position.x < 1e-4f || m_Position.x > 1.0f - 1e-4f || m_Position.y < 1e-4f || m_Position.y > 1.0f - 1e-4f)
		return;

	int gradient = CAIGradientDescent::GetPositionValue(int(m_Position.x * MAP_TILES_WIDTH), int(m_Position.y * MAP_TILES_HEIGHT));

	// 벽이나 느려지는 타일 위에 있거나 변화량이 없다면
	if (gradient == -1 || gradient == m_LastGradient)
		return;

	// 마지막 변화량이 초기화 되지 않았을 경우
	if (m_LastGradient == -1)
	{
		m_LastGradient = gradient;
		return;
	}

	if (gradient > m_LastGradient)
	{
		m_ConsecutiveGradientIncrement = min(m_ConsecutiveGradientIncrement + 1, CONSECUTIVE_INCREMENTS_FOR_BACKWARDS);

		// 일정 값만큼 뒤로 갔다면
		if (m_ConsecutiveGradientIncrement == CONSECUTIVE_INCREMENTS_FOR_BACKWARDS)
			m_GoingForward = false;
	}
	else
	{
		m_ConsecutiveGradientIncrement = max(m_ConsecutiveGradientIncrement - 1, CONSECUTIVE_INCREMENTS_FOR_BACKWARDS * -1);

		// 만약 위치의 값이 감소했다면 제대로된 길을 가고 있다는 뜻
		if (m_ConsecutiveGradientIncrement == CONSECUTIVE_INCREMENTS_FOR_BACKWARDS * -1)
			m_GoingForward = true;
	}

	int diff = gradient - m_LastGradient;

	// 현재 위지값이 랩 체크값보다 크다면 
	// 한 바퀴 돌았다는 뜻
	if (diff > CAIGradientDescent::GRADIENT_LAP_CHECK)
	{
		m_Laps = m_Laps + 1;

		if (m_Laps == 6)
		{
			// 레이스 완주시간 기록
			float finishTime = m_Scene->GetCurrentRaceTime();
			CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");
			gui->AddFinishTime(m_CurrentPlayer, finishTime);
		}

		if (m_Laps == 5 && m_MaxLapSoFar == 4 && m_ControlType == EDriverControlType::PLAYER)
		{
			// 현재 재생중인 모든 음악을 정지한다.
			// 그리고 마지막 랩 알림을과 마지막 랩 BGM을 재생한다.
			m_Scene->GetSceneResoruce()->StopAllMusic();
			m_Scene->GetSceneResoruce()->SoundPlay("LakituFinalLap");
			m_Scene->GetSceneResoruce()->SoundPlay("FinalLap");
		}

		if (m_Laps > m_MaxLapSoFar)
		{
			if (m_ControlType == EDriverControlType::PLAYER && m_Laps < 6)
			{
				// 김수한무을 소환시켜 현재 랩 표시
				m_Scene->GetLakitu()->ShowCurrentLap(m_Laps);
			}
			
			if (m_Laps > 1)
			{
				m_IndexOfLap.push_back(m_FollwedPath.size() - 1);
			}

			m_MaxLapSoFar = m_Laps;
		}
	}
	else if (diff < CAIGradientDescent::GRADIENT_LAP_CHECK * -1 && m_Laps > 0)
	{
		m_Laps--;
	}

	m_LastGradient = gradient;

	// 변화량 업데이트후에 방향을 체크한다.
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		// 거꾸로가고있다는 김수한무를 소환.
		m_Scene->GetLakitu()->WarningWrongDir(!m_GoingForward);
	}
}

void CDriver::ApplyMushroom()
{
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		m_Scene->GetSceneResoruce()->SoundPlay("Mushroom");
	}

	// 속도는 최대 속도로 올라간다.
	m_SpeedForward = m_Kart->m_MaxNormalLinearSpeed;

	// 속도 업 상태
	PushStateEnd(EDriverState::SPEED_UP, m_Scene->GetCurrentRaceTime() + SPEED_UP_DURATION);
}

void CDriver::ApplyStar()
{
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		m_Scene->GetSceneResoruce()->SoundPlay("Star");
	}

	// 스타 상태
	PushStateEnd(EDriverState::STAR, m_Scene->GetCurrentRaceTime() + STAR_DURATION);
	m_Animator.Star(STAR_DURATION);
}

void CDriver::ApplyThunder(float duration)
{
	// 회전속도는 0으로 만든다.
	m_SpeedTurn = 0.0f;

	// 전진 속도는 적절한 값으로 설정
	m_SpeedForward = std::fmin(m_SpeedForward, m_Kart->m_MaxNormalLinearSpeed * 0.3f);

	// 통제 불능 상태 추가
	PushStateEnd(EDriverState::UNCONTROLLED, m_Scene->GetCurrentRaceTime() + UNCONTROLLED_DURATION);

	if (m_ControlType == EDriverControlType::PLAYER)
	{
		// 플레이어 번개 피격 소리 재생
		m_Scene->GetSceneResoruce()->SoundPlay("Thunder");
		CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");

		if (m_Scene->GetSceneResoruce()->IsPlaying("ItemReel"))
			m_Scene->GetSceneResoruce()->SoundStop("ItemReel");

		gui->SetItem(EItem::NONE);
	}

	// 작아진 상태
	m_Animator.Small(duration);
	m_CurrentItem = EItem::NONE;

	// 속도 감속 상태
	PushStateEnd(EDriverState::SPEED_DOWN, m_Scene->GetCurrentRaceTime() + duration);
}

void CDriver::ApplyFeather()
{
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");

		// 점프 비율
		const float JUMP_INCLINATION = 80.0f / 90.0f;

		// 상승속도
		m_SpeedUpwards = JUMP_INCLINATION * m_SpeedForward * 400.0f * 8.0f;
		m_SpeedUpwards = std::fmax(m_SpeedUpwards, 60.0f);
		m_SpeedUpwards = std::fmin(m_SpeedUpwards, 80.0f);
		m_SpeedForward = (1.0f - JUMP_INCLINATION) * m_SpeedForward;

		m_SpeedForward = (float)std::fmax(m_SpeedForward, 0.1);

		// 각도 정규화
		float normlizedAngle = NormalizeAngle(m_Angle);

		m_FlightAngle = normlizedAngle;

		m_Animator.Feather();

		gui->SetItem(EItem::NONE);
	}

	m_JumpByFeather = true;

}

void CDriver::ShortJump()
{
	if (m_Height == 0.0f)
	{
		m_FlightAngle = m_Angle;
		m_Height = 3.0f;
		if (m_ControlType == EDriverControlType::PLAYER)
		{
			// 점프소리 재생
			m_Scene->GetSceneResoruce()->SoundPlay("Jump");
		}
	}
}

void CDriver::JumpByRamp(const ETileProperty& land)
{
	const float RAMP_INCLINATION = 22.5f / 90.0f;

	m_SpeedUpwards = RAMP_INCLINATION * m_SpeedForward * 384.0f * 8.0f;
	m_SpeedUpwards = std::fmax(m_SpeedUpwards, 20.0f);
	m_SpeedUpwards = std::fmin(m_SpeedUpwards, 40.0f);
	m_SpeedForward = (1.0f - RAMP_INCLINATION) * m_SpeedForward;

	m_SpeedForward = (float)std::fmax(m_SpeedForward, 0.1);

	m_JumpByRamp = true;

	float normlizedAngle = NormalizeAngle(m_Angle);

	// 만약 수평 언덕길이라면
	if (land == ETileProperty::RAMP_HORIZONTAL)
	{

		// 현재 각도가 0 ~ 180도 사이라면
		if (normlizedAngle >= 0 && normlizedAngle <= (float)M_PI)
			// 비행시 각도는 90도 고정
			m_FlightAngle = (float)M_PI_2;
		// 그 외라면
		else
			// 비행시 각도는 270도
			m_FlightAngle = 3 * (float)M_PI_2;
	}
	// 수직 언덕길일 경우
	else if (land == ETileProperty::RAMP_VERTICAL)
	{
		// 오른쪽에서 왼쪽으로 건널경우 즉 현재 각도가 90도와 270도 사이라면
		if (normlizedAngle > (float)M_PI_2 && normlizedAngle < 3 * (float)M_PI_2)
			m_FlightAngle = (float)M_PI;
		else
			// 아니라면 왼쪽에서 오른쪽의 경우는 각도는 0
			m_FlightAngle = 0;
	}

	if (m_ControlType == EDriverControlType::PLAYER)
	{
		// 점프 사운드 재생
		m_Scene->GetSceneResoruce()->SoundPlay("Jump");
	}
}

void CDriver::ApplyHit()
{
	// 무적이 아니라면 히트
	if (!IsImmune())
	{
		// 코인과 상태 제거
		AddCoin(-2);
		PopStateEnd(EDriverState::SPEED_UP);
		PopStateEnd(EDriverState::MORE_SPEED_UP);

		// 회전속도 제거
		m_SpeedTurn = 0.0f;

		m_SpeedForward = std::fmin(m_SpeedForward, m_Kart->m_MaxNormalLinearSpeed * 0.6f);

		if (m_ControlType == EDriverControlType::PLAYER && CanDrive())
		{
			// 피격 소리 재생
			m_Scene->GetSceneResoruce()->SoundPlay("Hit");
		}

		PushStateEnd(EDriverState::UNCONTROLLED, m_Scene->GetCurrentRaceTime() + UNCONTROLLED_DURATION);
	}
}

void CDriver::ApplySpinOver()
{
	if (!IsImmune())
	{
		PopStateEnd(EDriverState::SPEED_UP);
		PopStateEnd(EDriverState::MORE_SPEED_UP);

		m_SpeedTurn = 0.0f;

		m_SpeedForward = std::fmin(m_SpeedForward, m_Kart->m_MaxNormalLinearSpeed * 0.6f);

		if (m_ControlType == EDriverControlType::PLAYER)
		{
			m_Scene->GetSceneResoruce()->SoundPlay("Hit");
		}

		PushStateEnd(EDriverState::UNCONTROLLED, m_Scene->GetCurrentRaceTime() + UNCONTROLLED_DURATION);
	}
}

void CDriver::ApplySmash()
{
	if (!IsImmune())
	{
		AddCoin(-3);
		m_SpeedTurn = 0.0f;
		m_SpeedForward = 0.0f;

		m_Animator.Smash(SPEED_DOWN_DURATION + UNCONTROLLED_DURATION);

		if (m_ControlType == EDriverControlType::PLAYER && CanDrive())
		{
			m_Scene->GetSceneResoruce()->SoundPlay("Shrink");
		}

		PushStateEnd(EDriverState::UNCONTROLLED, m_Scene->GetCurrentRaceTime() + UNCONTROLLED_DURATION);
	}


}

// 코인추가
void CDriver::AddCoin(int amount)
{
	// 만약 코인이 10개를 넘을 경우
	if (m_Coins + amount > 10)
	{
		// 더해지는 코인수는 10에서 빼준다.
		// 예) m_Coins 9, amount 3일 경우 amount는 10 - 9  = 1
		// 따라서 하나 추가된다.
		amount = 10 - m_Coins;
	}
	// 만약 0보다 적을경우 (음수)
	else if (m_Coins + amount < 0)
	{
		// 더해지는 코인은 0에서 빼준다.
		// 예) m_Coins 2, amount 3일경우 0 - 2 amount는 = -2
		amount = 0 - m_Coins;
	}

	m_Coins += amount;

	// 코인 효과를 추가한다.
	if (amount != 0)
	{
		m_Scene->AddCoinEffect(this, std::abs(amount), amount > 0);
	}

	if (m_Coins < 11 && m_ControlType == EDriverControlType::PLAYER)
	{
		CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");
		gui->AddCoin(amount);
	}
}

bool CDriver::CanDrive() const
{
	// 만약 현재 통제불능도 아니고 정지 상황도 아니라면 운전 가능 상태
	return !(m_CurrentState & (int)EDriverState::UNCONTROLLED) &&
		!(m_CurrentState & (int)EDriverState::STOPPED);
}

bool CDriver::IsImmune() const
{
	// 스타라면 무적
	return m_CurrentState & (int)EDriverState::STAR;
}

bool CDriver::IsVisible() const
{
	// 현재 ~상태에서 투명상태를 AND 후에 참이라면 보이는 상태
	return ~m_CurrentState & (int)EDriverState::INVISIBLE;
}

bool CDriver::CanUseItem() const
{
	return m_CanUseAt < m_Scene->GetCurrentRaceTime();
}

void CDriver::PickUpItem(EItem item)
{
	m_CurrentItem = item;
	m_CanUseAt = m_Scene->GetCurrentRaceTime() + ITEM_USE_WAIT;

	if (m_ControlType == EDriverControlType::PLAYER)
	{
		if (item != EItem::NONE)
		{
			// 아이템 슬롯 음악 사운드 재생
			m_Scene->GetSceneResoruce()->SoundPlay("ItemReel");
		}

		// Gui 아이템 이미지 설정
		CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");
		gui->SetItem(item);
	}
}

void CDriver::Reset()
{
	m_OnLakitu = false;
	m_Falling = false;
	m_DriftPressed = false;
	m_JumpByRamp = false;
	m_JumpByFeather = false;
	m_CurrentState = (int)EDriverState::NORMAL;

	for (int i = 0; i < (int)EDriverState::MAX; i++)
		m_StateEnd[i] = 0.0f;

	m_Animator.Reset();
}

void CDriver::EndRaceAndReset()
{
	Reset();
}

void CDriver::SetPositionAndReset(const Vector2& position, const float angle)
{
	m_Position = position;
	m_Angle = angle;

	m_Height = 0.0f;
	m_FlightAngle = 0.0f;

	m_FollwedPath.clear();
	m_PrevLab.clear();
	m_IndexOfLap.clear();
	m_FollwedPath.push_back(position);
	m_PrevLab.push_back(0);
	m_IndexOfLap.push_back(0);

	m_PathLastUpdatedAt = m_Scene->GetCurrentRaceTime();

	m_Laps = 0;
	m_MaxLapSoFar = 0;
	m_CurrentItem = EItem::NONE;
	m_CanUseAt = m_Scene->GetCurrentRaceTime();
	m_Coins = 0;
	m_Rank = 0;
	m_GoingForward = true;
	m_LastGradient = -1;
	m_ConsecutiveGradientIncrement = 0;

	m_SpeedForward = 0.0f;
	m_SpeedTurn = 0.0f;
	m_SpeedUpwards = 0.0f;
	m_CollisionMomentum = Vector2(0.0f, 0.0f);
	m_VectorialSpeed = Vector2(0.0f, 0.0f);

	Reset();

	m_Animator.Reset();
}

void CDriver::Update(float deltaTime)
{

	if (!m_Start)
	{
		m_Start = true;
		Start();
	}

	// 선형속도 초기화.
	float accelerationLinear = 0.0f;

	// 만약 땅에 닿아있다면
	// 마찰계수만큼 속도는 감속한다.
	if (m_Height <= 0.0f)
		accelerationLinear += FRICTION_LINEAR_ACELERATION;


	// 만약 회전을 하고 있지 않다면
	if ((!CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) &&
		!CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold))
		||
		(CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) && m_SpeedTurn > 0.0f) ||
		(CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold) && m_SpeedTurn < 0.0f))
	{
		// 회전속도는 천천히 감속한다.
		if (m_SpeedTurn > 0.001f)
			m_SpeedTurn /= 1.2f;
		else
			m_SpeedTurn = 0.0f;
	}

	// 끝난 상태는 제거해준다.
	PopStateEnd(m_Scene->GetCurrentRaceTime());

	

	// 만약 통제불능 상태라면
	if (m_CurrentState & (int)EDriverState::UNCONTROLLED)
	{
		// 피격 애니메이션 재생
		m_Animator.Hit();
	}
	else if (m_CurrentState & (int)EDriverState::STOPPED)
	{

	}
	else
	{
		// 아니라면 직진 모션
		if (m_Height <= 0.0f && !m_JumpByFeather)
		{
			m_Animator.GoForward();
		}

		// 현재 컨트롤 상태에따라 AI, Player의 컨트롤 함수 호출
		switch (m_ControlType)
		{
		case EDriverControlType::DISABLED:
			break;
		case EDriverControlType::AI_GRADIENT:
			UseGradientControl(accelerationLinear);
			break;
		case EDriverControlType::PLAYER:
			PlayerControl(accelerationLinear);
			break;
		}
	}

	// 랭크에 따른 보너스 속도
	accelerationLinear *= 1.0f + POSITION_ACCELERATION_BONUS_PCT * m_Rank;

	// 만약 부적이라면 속도는 2배
	if (IsImmune())
		accelerationLinear *= 2.0f;

	CTileMap* currentTileMap = m_Scene->GetTileMap();

	if (currentTileMap)
	{
		ETileProperty tile = currentTileMap->GetTile(int(m_Position.x * MAP_TILES_WIDTH), int(m_Position.y * MAP_TILES_HEIGHT))->GetTileProperty();

		// 현재 타일이 느려지는 타일이고 현재 상태가 무적이 아니거나 속도 상승 상태가 아닐 경우는
		if ((tile == ETileProperty::SLOW || tile == ETileProperty::SLOW_2) && (~m_CurrentState & (int)EDriverState::STAR) &&
			(~m_CurrentState & (int)EDriverState::SPEED_UP))
		{
			// 속도는 줄어든다.
			if (m_SpeedForward > m_Kart->m_SlowTileMaxLinearSpeed)
				accelerationLinear += SLOW_LAND_LINEAR_ACELERATION;
			if (m_Animator.IsDrifting())
			{
				m_SpinOver += deltaTime;

				if (m_SpinOver >= SPINOVERTIME)
				{
					ApplySpinOver();
					m_SpinOver = 0.0f;
				}
			}
		}
		else
			m_SpinOver = 0.0f;


		// 땅에 닿아있고
		if (m_Height <= 0.0f)
		{

			// 만약 기름을 밟았다면
			if (tile == ETileProperty::OIL_SLICK)
			{
				// 피격
				ApplyHit();
				m_SpeedForward = std::fmaxf(m_SpeedForward, 0.01f);
			}
			// 언덕이라면 점프
			else if (tile == ETileProperty::RAMP_HORIZONTAL || tile == ETileProperty::RAMP_VERTICAL)
			{
				JumpByRamp(tile);
			}
			// 가속 패널이라면
			else if (tile == ETileProperty::ACCEL_PANEL)
			{
				if (~m_CurrentState & (int)EDriverState::SPEED_UP &&
					m_ControlType == EDriverControlType::PLAYER)
				{
					// 가속 소리 재생
					m_Scene->GetSceneResoruce()->SoundPlay("Mushroom");
				}

				// cpu의 경우 좀더 보너스
				if (m_CurrentState & (int)EDriverState::SPEED_UP && m_ControlType != EDriverControlType::PLAYER)
				{
					PushStateEnd(EDriverState::MORE_SPEED_UP, m_Scene->GetCurrentRaceTime() + MORE_SPEED_UP_DURATION);
					m_SpeedForward = m_Kart->m_MaxSpeedUpLinearSpeed * 1.2f;
				}
				else
					m_SpeedForward = m_Kart->m_MaxSpeedUpLinearSpeed;

				PushStateEnd(EDriverState::SPEED_UP, m_Scene->GetCurrentRaceTime() + SPEED_UP_DURATION);
			}
			else if (tile == ETileProperty::OTHER)
			{
				// 아이템 패널 코인등과 충돌했을때
				m_Scene->CollideWithFloor(this);
			}
		}
	}

	// 중력 작용
	if ((m_Height > 0.0f || m_SpeedUpwards > 0.0f) && !m_OnLakitu)
	{
		// 중력 배수
		const float gravityAcelration = -9.8f * 16.0f;

		// 자유낙하 공식 높이는 양수이다. 하지만 그려낼때는 높이의 수만큼 빼준다. 스크린은 y축이 반전
		m_Height = m_Height + m_SpeedUpwards * deltaTime + 0.5f * gravityAcelration * deltaTime * deltaTime;


		if (m_Height < 0.0f)
		{
			if (m_ControlType == EDriverControlType::PLAYER)
			{
				// 착륙사운드 재생
				m_Scene->GetSceneResoruce()->SoundPlay("Landing");

				if (m_Scene->GetSceneResoruce()->IsPlaying("Feather"))
					m_Scene->GetSceneResoruce()->SoundStop("Feather");
			}

			// 만약 점프가 드리프트 전에 하는 소 점프일 경우
			// 드리프트 키 온
			if (!m_JumpByRamp && (CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) ||
				CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold)))
			{
				m_DriftPressed = true;
			}

			if (m_JumpByFeather)
				m_Animator.GoForward();

			m_JumpByFeather = false;
			m_JumpByRamp = false;
		}

		m_Height = std::fmax(m_Height, 0.0f);
		
		// 상승 속도 업데이트
		m_SpeedUpwards = m_SpeedUpwards + gravityAcelration * deltaTime;

		if (m_Height <= 0.0f)
			m_SpeedUpwards = 0.0f;
	}


	// 최대 선형 속도
	float maxLinearSpeed;

	if (m_CurrentState & (int)EDriverState::MORE_SPEED_UP)
		maxLinearSpeed = m_Kart->m_MaxSpeedUpLinearSpeed * 1.2f;
	else if (m_CurrentState & (int)EDriverState::SPEED_UP ||
		m_CurrentState & (int)EDriverState::STAR)
	{
		maxLinearSpeed = m_Kart->m_MaxSpeedUpLinearSpeed;
		accelerationLinear *= 2.0f;
	}
	else if (m_CurrentState & (int)EDriverState::SPEED_DOWN)
		maxLinearSpeed = m_Kart->m_MaxSpeedDownLinearSpeed;
	else
		maxLinearSpeed = m_Kart->m_MaxNormalLinearSpeed;

	maxLinearSpeed = maxLinearSpeed + (COIN_SPEED * m_Coins);


	// 속도와 각도의 변화
	float deltaAngle = m_SpeedTurn * deltaTime;
	float deltaSpace = m_SpeedForward * deltaTime + accelerationLinear * (deltaTime * deltaTime) / 2.0f;

	deltaSpace = std::fminf(deltaSpace, maxLinearSpeed * deltaTime);
	deltaSpace = std::fmaxf(deltaSpace, 0.0f);

	// 속도 갱신
	m_SpeedForward += accelerationLinear * deltaTime;
	m_SpeedForward = std::fminf(m_SpeedForward, maxLinearSpeed);
	m_SpeedForward = std::fmaxf(m_SpeedForward, 0.0f);

	// 만약 현재 상태가 공중에 있는 상태라면 각도는 비행각도로 바뀐다.
	float movementAngle = m_Height == 0.0f ? m_Angle : m_FlightAngle;

	Vector2 deltaPosition = Vector2(cosf(movementAngle), sinf(movementAngle)) * deltaSpace;

	// 만약 충돌 모멘텀이 있다면 더해준다.
	deltaPosition += m_CollisionMomentum;
	// 충돌모멘텀 감소
	m_CollisionMomentum /= 1.2f;
	// 충돌 속도도 추가
	deltaPosition += m_VectorialSpeed * deltaTime;
	m_VectorialSpeed /= 1.3f;

	// 만약 맵을 벗어나게 된다면 추락
	if (((m_Position + deltaPosition).x < 0.0f ||
		(m_Position + deltaPosition).x > 1.0f) ||
		((m_Position + deltaPosition).y < 0.0f ||
			(m_Position + deltaPosition).y > 1.0f)) {
		m_Falling = true;
		deltaPosition = Vector2(0.0f, 0.0f);
	}

	if (currentTileMap)
	{
		ETileProperty tileProp = currentTileMap->GetTile(int(m_Position.x * MAP_TILES_WIDTH), int(m_Position.y * MAP_TILES_HEIGHT))->GetTileProperty();

		// 만약 현재 언덕으로 인한 점프도 아니고 깃털로인한 점프도 아니라면
		// 타일과 충돌 체크
		if (!m_JumpByRamp && !m_JumpByFeather && tileProp != ETileProperty::BLOCK)
			ImprovedCheckOfMap(this, m_Position, deltaPosition);
	}

	// 추락중이라면
	if (m_Falling)
	{
		// 회전속도는 초기화
		m_SpeedTurn = 0.0f;
		m_SpeedForward = m_SpeedForward / 1.1f;

		if (m_ControlType == EDriverControlType::PLAYER)
		{
			// 추락 애니메이션 
			m_Animator.Fall();
			CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");

			// 페이드 인 아웃
			if (!gui->IsBlackScreen() && !gui->HasEffectEnd())
			{
				gui->EffectStart();
				m_Scene->GetSceneResoruce()->SoundPlay("Fall");
				PushStateEnd(EDriverState::STOPPED, m_Scene->GetCurrentRaceTime() + 10.0f);
			}
			else if (gui->HasEffectEnd())
			{
				m_SpeedTurn = 0.0f;
				m_SpeedForward = 0.0f;
				RelocateToNearestGoodPosition();
				AddCoin(-2);
				Reset();
				m_Scene->GetLakitu()->PickUpDriver(this);
				m_Falling = false;
				gui->ResetEffectEnd();
			}
		}
		// AI가 추락했을 경우
		else
		{
			m_SpeedTurn = 0.0f;
			m_SpeedForward = 0.0f;
			Reset();
			AddCoin(-2);
			RelocateToNearestGoodPosition();
			PushStateEnd(EDriverState::STOPPED, m_Scene->GetCurrentRaceTime() + 2.5f);

			PushStateEnd(EDriverState::INVISIBLE, m_Scene->GetCurrentRaceTime() + 2.5f);

			m_Falling = false;
		}

	}

	m_Position += deltaPosition;

	if ((m_Position.x < 0.0f || m_Position.x > 1.0f) ||
		(m_Position.y < 0.0f || m_Position.y > 1.0f))
	{
		m_Falling = true;
	}

	if (m_Height <= 0.0f)
	{
		m_Angle += deltaAngle;
		m_Angle = fmodf(m_Angle, 2.0f * (float)M_PI);
	}

	// 변화량 갱신
	UpdateGradientPosition();
	// 애니메이터 업데이트
	m_Animator.Update(m_SpeedForward, m_SpeedTurn, m_Height, deltaTime, m_Position);

	// 새로운 위치 정보를 기록한다. (AI)
	if (!m_Falling && m_Scene->GetCurrentRaceTime() - m_PathLastUpdatedAt >
		FOLLWED_PATH_UPDATE_INTERVAL)
	{
		auto iter = m_FollwedPath.rbegin();
		int numOfUpdatesWithoutMoving = 0;

		// AI일 경우
		if (m_ControlType != EDriverControlType::PLAYER)
		{
			while (iter != m_FollwedPath.rend())
			{
				// 만약 타일 하나를 벗어난 정도로 움직였다면 바로 새로운 위치정보를 기록한다.
				if (fabs(m_Position.x - iter->x) > (1.0f / MAP_TILES_WIDTH))
					break;

				// 마찬가지
				if (fabs(m_Position.y - iter->y) > (1.0f / MAP_TILES_HEIGHT))
					break;

				// 만약 특정 횟수만큼 움직임이 없었다면
				if (++numOfUpdatesWithoutMoving >= STPES_STILL_FOR_RELOCATION)
				{
					// 그동안 따라온 경로로 새로운 경로를 만들어낸다.
					int size = (int)m_FollwedPath.size();

					// 그동안 따라온 경로 목록에서 제거한다.
					for (int i = max(size - STPES_STILL_FOR_RELOCATION - 5, 1); i < size; i++)
						m_FollwedPath.pop_back();

					// 새로운 위치로 재조정
					RelocateToNearestGoodPosition();
					break;
				}

				iter++;
			}
		}

		// 기록한다.
		m_FollwedPath.push_back(m_Position);
		m_PrevLab.push_back(m_Laps);
		m_PathLastUpdatedAt = m_Scene->GetCurrentRaceTime();
	}
}

CTexture* CDriver::GetTexture()
{
	return m_Animator.GetTexture();
}

void CDriver::Render(const float scale, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& drawables, const bool withShadow)
{
	float width = (float)CGameManager::GetInst()->GetResolution().width;
	float y = CGameManager::GetInst()->GetResolution().height * 45.0f / 100.0f - 10.0f;

	if (m_Animator.GetSmallTime() > 0.0f)
	{
		y += 32.0f;
		m_Animator.GetTexture()->SetPos(width / 2.0f + 16.0f, y);
	}
	else if (m_Animator.GetSmashTime() > 0.0f)
	{
		y += 32.0f;
		m_Animator.GetTexture()->SetPos(width / 2.0f, y);
	}
	else
	{
		m_Animator.GetTexture()->SetPos(width / 2.0f, y);
	}



	float moveX = m_Animator.GetSpriteDriftMovement();
	float moveY = m_Animator.GetSpriteMovementSpeed() * 1.75f - m_Height * 8.0f;
	m_Animator.GetTexture()->SetOffset(-6.5f, 0.0f);
	m_Animator.GetTexture()->Move((moveX * scale), (moveY * scale));
	m_Animator.GetTexture()->Scale(scale, scale);

	float z = m_Scene->GetCamDist() / MAP_ASSETS_WIDTH;

	drawables.push_back(std::make_pair(std::make_pair(z, m_Animator.GetTexture()), this));

	if (m_Height > 0.0f && withShadow)
	{
		// 현재 애니메이션 스케일을 구해서 그림자의 스케일을 조정한다.
		m_Shadow->SetScale(m_Animator.GetTexture()->GetScale() * m_Scene->GetCircuitHeightPCT() * 2.0f);
		// 그림자도 현재 속도에 알맞게 움직인다.
		m_Shadow->SetPos(width / 2.0f, y + 30.0f);
		m_Shadow->Move(moveX * scale, m_Animator.GetSpriteMovementSpeed() * scale);

		float zShaodw = z + 10000.0f;

		drawables.push_back(std::make_pair(std::make_pair(zShaodw, m_Shadow), this));
	}
}

void CDriver::RenderFront(const float scale, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& drawables)
{
	float width = (float)CGameManager::GetInst()->GetResolution().width;
	float y = CGameManager::GetInst()->GetResolution().height * 45.0f / 100.0f + 30.0f;
	CTexture* front = m_Animator.GetFrontFaceTexture();

	front->SetScale(m_Animator.GetTexture()->GetScale());

	if (m_Animator.GetSmallTime() > 0.0f)
	{
		y -= 32.0f;
		front->SetPos(width / 2.0f + 16.0f, y);
	}
	else if (m_Animator.GetSmashTime() > 0.0f)
	{
		front->SetPos(width / 2.0f, y + 32.0f);
	}
	else
	{
		front->SetPos(width / 2.0f, y);
	}

	float moveX = m_Animator.GetSpriteDriftMovement();
	float moveY = m_Animator.GetSpriteMovementSpeed() * 1.75f - m_Height * 8.0f;
	front->SetOffset(-6.5f, 0.0f);
	front->Move((moveX * scale), (moveY * scale));
	front->Scale(scale, scale);

	float z = m_Scene->GetCamDist() / MAP_ASSETS_WIDTH;

	drawables.push_back(std::make_pair(std::make_pair(z, front), this));
}

bool CDriver::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed,
	const Vector2& otherPos, const float otherWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
	// 만약 일정 수준의 높이차가 있을경우 충돌은 무시된다.
	if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
		return false;

	// 만약 둘중에 하나라도 무적이라면 충돌은 역시나 무시된다.
	if (IsImmune() && !otherIsImmune)
	{
		// 자기자신은 무적이고 상대방은 무적이 아니기에 충돌정보를 설정
		collisionData = CollisionData(Vector2(0.0f, 0.0f), 0.4f, ECollisionType::HIT);
		return true;
	}
	else if (!IsImmune() && otherIsImmune)
		return false;

	// 둘다 무적이 아니거나 둘다 무적일 경우
	float mySpeedMod = sqrtf(fmaxf(m_SpeedForward * m_SpeedForward, 1e-12f));
	float otherSpeedMod = sqrtf(fmaxf(otherSpeed.x * otherSpeed.x + otherSpeed.y * otherSpeed.y, 1e-12f));

	float speedFactor = mySpeedMod / (mySpeedMod + otherSpeedMod);

	float weightFactor = sqrtf(fmaxf(m_Kart->m_Weight / (m_Kart->m_Weight + otherWeight), 1e-12f));
	Vector2 dir = (otherPos - m_Position) / sqrtf(fmaxf(1e-12f, distance));

	collisionData = CollisionData(dir * mySpeedMod * speedFactor * weightFactor * 0.1f, weightFactor * 0.95f);

	return true;
}

void UpdatePosition(Vector2& pos, int& stepsFromGoal)
{
	Vector2 nextDir = CAIGradientDescent::GetNextDirection(pos);
	pos = pos + nextDir;

	// 만약 0일시에는 시작 또는 골 지점
	if (stepsFromGoal == 0)
	{
		// 따라서 한칸 앞으로 움직인다.
		if (CAIGradientDescent::GetPositionValue(pos) == 0)
			stepsFromGoal = 1;
	}
	else
		stepsFromGoal++;
}

void CDriver::RelocateToNearestGoodPosition()
{
	unsigned int index = 0;

	if (m_FollwedPath.size() > STEPS_BACK_FOR_RELOCATION)
		index = (int)m_FollwedPath.size() - 1 - STEPS_BACK_FOR_RELOCATION;

	m_Position = m_FollwedPath[index];
	m_Laps = m_PrevLab[index];

	// 현재 위치가 골인 지점으로 얼마나 차이가나는지 만약 0이라면 바로 골인지점
	// 아니라면 0을 대입
	int stepsFromGoal = CAIGradientDescent::GetPositionValue(m_Position) == 0 ? 1 : 0;

	CTileMap* tileMap = m_Scene->GetTileMap();

	if (tileMap)
	{
		// 위험지역에서 벗어날때까지 계속해서 이동한다.
		while (tileMap->GetTile(m_Position * MAP_ASSETS_HEIGHT)->GetTileProperty() == ETileProperty::HAZARD)
		{
			UpdatePosition(m_Position, stepsFromGoal);
		}

		// 약간의 랜덤값을 준다.
		int times = rand() % 6 + 2;

		// 랜덤한 양만큼 또 살짝 업데이트
		for (int i = 0; i < times; i++)
		{
			UpdatePosition(m_Position, stepsFromGoal);
		}

		// 다시 위험지역에서 벗아날때까지 반복
		while (tileMap->GetTile(m_Position * MAP_ASSETS_HEIGHT)->GetTileProperty() == ETileProperty::HAZARD)
		{
			UpdatePosition(m_Position, stepsFromGoal);
		}

		// 다음 방향을 정한다.
		Vector2 next = CAIGradientDescent::GetNextDirection(m_Position);

		// 만약 0보다 크다면
		if (stepsFromGoal > 0)
		{
			// 그 수치만큼 위치를 새로 갱신한다.
			m_Position -= next * float(stepsFromGoal + 2);
		}

		// 각도도 갱신
		m_Angle = std::atan2f(next.y, next.x);
	}

}