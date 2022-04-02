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

// ������ 0 ~ Pi ������ ��ȯ�Ѵ�.
float NormalizeAngle(float angle)
{
	float normlizedAngle = angle;

	// ���� ������ 2pi�̻��̶�� 2pi��ŭ ����.
	while (normlizedAngle >= 2 * (float)M_PI)
		normlizedAngle -= 2 * (float)M_PI;

	// �ݾ� 0���� �۴ٸ� 2pi��ŭ ���Ѵ�.
	while (normlizedAngle < 0)
		normlizedAngle += 2 * (float)M_PI;

	return normlizedAngle;
}

// �ӵ� �׷����� �ùķ��̼��ؼ� �ӵ��� ����Ѵ�.
void SimulateSpeedGraph(CDriver* self, float& accelerationLinear)
{
	// �߷��� ����̹���
	if (self->m_Kart->m_Convex)
	{
		// ���� �ӵ��� �ִ�ӵ��� �� �ۼ�Ʈ������ ���Ѵ�.
		float speedPercentage = self->m_SpeedForward / self->m_Kart->m_MaxNormalLinearSpeed;

		// ���� 25�ۼ�Ʈ �Ʒ����
		if (speedPercentage < 0.25f)
		{
			// �����ӵ��� ���ӵ��� 2���� 1�� ��ŭ ����
			accelerationLinear += self->m_Kart->m_MotorAcceleration / 2.0f;
		}
		// ���� 25�ۼ�Ʈ���� ũ�� 45�ۼ�Ʈ���� �Ʒ����
		else if (speedPercentage < 0.45f)
		{
			// �����ӵ��� ���ӵ����� ���ǵ� �ۼ�Ʈ�� 0.075f�� ���Ѹ�ŭ ���ؼ� �÷��ش�.
			accelerationLinear += self->m_Kart->m_MotorAcceleration * (speedPercentage + 0.075f);
		}
		// ���� 45�ۼ�Ʈ���� ũ�� 95�ۼ�Ʈ���� �۴ٸ�
		else if (speedPercentage < 0.95f)
		{
			accelerationLinear += self->m_Kart->m_MotorAcceleration / 2.0f;
		}
		// �� ��
		else
		{
			accelerationLinear += (0.05 * self->m_Kart->m_MaxNormalLinearSpeed) / 4.0f;
		}
	}
	// �Ϲ� ����̹���
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

// ȸ���ӵ� ��ȯ
bool IncreasingAngularAceleration(CDriver* self, float& accelerationAngular)
{
	// �ϴ� �帮��Ʈ�� �������� ����
	bool drifting = false;

	// ���� �帮��Ʈ�� ����������
	if (self->m_DriftPressed)
	{
		// ���� ������ �����ִٸ�(�ð����)
		if (CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold))
		{
			// ���ӵ��� ����� �þ��.
			self->m_SpeedTurn = self->m_Kart->m_MaxTurningAngularSpeed * 0.41f;
		}
		// �ݽð� �����̶��
		else if (CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold))
		{
			// ���ӵ��� ������ �پ���.
			self->m_SpeedTurn = -1.0f * self->m_Kart->m_MaxTurningAngularSpeed * 0.41f;
		}

		// �帮��Ʈ �ʱ�ȭ
		self->m_DriftPressed = false;
	}

	// ���� ���ӵ��� �ӵ��� ���� ���� �̻��̶��
	if (std::fabs(self->m_SpeedTurn) > (self->m_Kart->m_MaxTurningAngularSpeed * 0.4f) &&
		std::fabs(self->m_SpeedForward) > (self->m_Kart->m_MaxNormalLinearSpeed * 0.4f))
	{
		// �帮��Ʈ�� �����Ѵ�.
		accelerationAngular = self->m_Kart->m_TurningAcceleration * 1.0f;
		drifting = true;
	}
	else
		accelerationAngular = self->m_Kart->m_TurningAcceleration * 0.15f;
	return drifting;
}

// �ڳʸ��ÿ� �ӵ��� �����Ѵ�.
void ReduceLinearSpeedWhileTurning(CDriver* self, float& accelerationLinear, float& speedTurn)
{
	// ȸ���ӵ��� �ִ� ȸ�� �ӵ��� ������.
	float speedTurnPercentage = std::fabs(speedTurn / self->m_Kart->m_MaxTurningAngularSpeed);

	// ���� ���� �ӵ��� �ִ� 90�� ���� ���ٸ�
	if (self->m_SpeedForward > self->m_Kart->m_MaxNormalLinearSpeed * 0.9f)
	{
		// ������ ���ӽ�Ų��.
		accelerationLinear = -1.0f * self->m_Kart->m_MotorAcceleration * speedTurnPercentage;
	}
}


// Ÿ���� ���� �浹 ������
void HandleHitBlock(CDriver* self, const Vector2& nextPos)
{
	Vector2 moveWidth = Vector2(1.0f / MAP_TILES_WIDTH, 0.0f);
	Vector2 moveHeight = Vector2(0.0f, 1.0f / MAP_TILES_HEIGHT);


	CTileMap* currentTileMap = self->GetScene()->GetTileMap();

	if (currentTileMap)
	{
		int widthSize = 0;
		// -1, 1�� ��ȸ�Ѵ�.
		for (int j = -1; j <= 1; j += 2)
		{
			for (int i = 1; i <= 4; i++)
			{
				// temp���� nextPos + moveWidth * (-4 ~ -1), (1, 4)������ ���� ���Ѵ�.
				Vector2 temp = nextPos + moveWidth * float(i * j);

				// ���� ��¦ ���ϰų� �� ���� ���� ��ġ�ߴٸ� ũ�⸦ �ø���.
				if (currentTileMap->GetTile(int(temp.x * MAP_TILES_WIDTH), int(temp.y * MAP_TILES_HEIGHT))->GetTileProperty() == ETileProperty::BLOCK)
					widthSize++;
				else
					break;
			}
		}

		// �̹��� ���η� Ȯ���Ѵ�.
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

		// �浹 ������� ���Ѵ�.
		float momentumSpeed = sqrtf(powf(self->m_CollisionMomentum.x, 2.0f) + powf(self->m_CollisionMomentum.y, 2.0f));

		float factor;
		float angle;

		// ���� ������� ���ٸ� �״�� �����ȴ�.
		if (momentumSpeed == 0.0f)
		{
			factor = self->m_SpeedForward;
			angle = self->GetAngle();
		}
		// �ƴ϶��
		else
		{
			factor = momentumSpeed;
			// ������ ���Ӱ� �����̵ȴ�.
			angle = atan2f(self->m_CollisionMomentum.y, self->m_CollisionMomentum.x);
		}

		// ����� �ʱ�ȭ
		self->m_CollisionMomentum = Vector2(0.0f, 0.0f);

		// �����Ͻÿ��� ���� �ִ�ӵ��� �״�� �����ȴ�.
		if (self->IsImmune())
			factor = fmax(factor, self->m_Kart->m_MaxNormalLinearSpeed * 0.97f);
		// �ƴ϶�� ������ ���ӵȴ�.
		else
			factor = fmax(factor, self->m_Kart->m_MaxNormalLinearSpeed * 0.5f);


		// ������� ���Ѵ�.
		Vector2 momentum = Vector2(cosf(angle), sinf(angle)) * fmax(0.01f, factor);

		// �翷 �����ְ� ���Ʒ��� �շ����� ���� -y
		if (widthSize > 4 && heightSize < 4)
			self->m_VectorialSpeed = Vector2(momentum.x, -momentum.y);
		// ���Ʒ��� �����ְ� �翷�� �շ����� ���� -x
		else if (widthSize < 4 && heightSize > 4)
			self->m_VectorialSpeed = Vector2(-momentum.x, momentum.y);
		// �� �� ��Ȳ���� �� �ݴ��� ����
		else
			self->m_VectorialSpeed = Vector2(-momentum.x, -momentum.y);
	}

}


// Ÿ�� �ʰ� �浹 ����
void ImprovedCheckOfMap(CDriver* self, const Vector2& pos, Vector2& deltaPos)
{
	Vector2 nextPos = pos + deltaPos;

	// ���� ���� ������ ���� �����Ѵ�. �� Ÿ���� ũ��� 8�̰� ��ü ���� ũ��� 1024, �ű⼭ 3.5f�� ���� ���� ���� ��
	float halfTileWidthInMapCoord = float(MAP_TILE_SIZE) / MAP_ASSETS_WIDTH / 3.5f;
	float halfTileHeightInMapCoord = float(MAP_TILE_SIZE) / MAP_ASSETS_HEIGHT / 3.5f;

	// ���� �÷��̾ �ٶ󺸰� �ִ� ���⿡�� ������ ������ ����
	// 0(����), M_PI_2(�ð� �������� 90�� Ʋ������)....
	float deltaAngle[5] = { 0, (float)M_PI_2, (float)-M_PI_2, (float)M_PI_4, (float)-M_PI_4 };

	for (int i = 0; i < 5; i++)
	{
		// ���� �÷��̾ �ٶ󺸰��ִ� ���⿡�� ��¦ ������ ���� ��ȭ�� �ش�.
		Vector2 shifting = Vector2(cosf(self->GetAngle() + deltaAngle[i]) * halfTileWidthInMapCoord,
			sinf(self->GetAngle() + deltaAngle[i]) * halfTileHeightInMapCoord);

		CTileMap* currentTileMap = self->GetScene()->GetTileMap();

		if (currentTileMap)
		{
			Vector2 testPos = nextPos + shifting;

			ETileProperty tile = currentTileMap->GetTile(int(testPos.x * MAP_TILES_WIDTH), int(testPos.y * MAP_TILES_HEIGHT))->GetTileProperty();

			// ���� �ӽ���ǥ�� �浹�� ��ġ���
			switch (tile)
			{
			case ETileProperty::BLOCK:
				HandleHitBlock(self, testPos);
				// ��� �ӵ� ��� ���¸� ����
				self->PopStateEnd(EDriverState::SPEED_UP);
				self->PopStateEnd(EDriverState::MORE_SPEED_UP);

				if (self->m_ControlType == EDriverControlType::PLAYER)
				{
					// �浹 �Ҹ� ���
					self->GetScene()->GetSceneResoruce()->SoundPlay("Crash");
				}

				// �浹 ����Ʈ ����
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


// ���� �÷��׸� �����.
void CDriver::PushStateEnd(EDriverState state, const float endTime)
{
	// OR�� ��Ʈ�� �����ϰ�
	this->m_CurrentState |= (int)state;
	// ���¸� log2�� ���ؼ� �ٷ� ���س���. (state 0, 1, 2, 4, 8...) 
	m_StateEnd[(int)log2((int)state)] = endTime;
}

// ���� �÷��׸� �����Ѵ�.
void CDriver::PopStateEnd(EDriverState state)
{
	// AND NOT���� �ش� ���¸� �����ϰ� 
	this->m_CurrentState &= ~(int)state;
	// �ð��� �ʱ�ȭ
	m_StateEnd[(int)log2((int)state)] = 0.0f;
}

// ��� ���¸� ��ȸ�ϸ鼭 ������ �ð��� �귶�ٸ� �����Ѵ�.
int CDriver::PopStateEnd(const float currentTime)
{
	// ���� ���µ�
	int finishedStates = 0;

	if (this->m_CurrentState != 0)
	{
		// 1, 2, 4, 8, 16 ....
		int state = 1;

		for (int i = 0; i < (int)EDriverState::MAX; i++)
		{
			if (this->m_CurrentState & state)
			{
				// ���� ����� �ð��� �귶�ٸ�
				if (m_StateEnd[i] < currentTime)
				{
					// ���� ���¿��� �߰��ϰ�
					finishedStates |= state;
					// ���� ���¿����� �����Ѵ�.
					this->m_CurrentState &= ~state;
				}
			}

			state *= 2;
		}
	}

	return finishedStates;
}

// ���� ��Ʈ��
void CDriver::PlayerControl(float& accelerationLinear)
{
	// ���� ���߿� �� ���� �ʰ�
	if (m_Height == 0.0f)
	{
		// �Ǽ��� �������ִٸ� �ӵ� �׷��� ������ �̿��Ͽ� �ӵ��� �����Ѵ�.
		if (CInput::GetInst()->GetKeyState('X', EKeyState::KeyState_Hold))
			SimulateSpeedGraph(this, accelerationLinear);


		// �극��ũ�� ��������
		bool IsBrake = false;

		// ���� ���� �÷��̾ �극��ũ�� ������������
		if (CInput::GetInst()->GetKeyState('Z', EKeyState::KeyState_Hold))
		{
			// �ӵ��� ���� �ӵ� �̻��̶��
			if (m_SpeedForward > 0.2f * m_Kart->m_MaxNormalLinearSpeed && !IsBrake)
			{
				// �극��ũ�� ������
				IsBrake = true;
				// �극��ũ �Ҹ� ���
				if(!m_Scene->GetSceneResoruce()->IsPlaying("Brake"))
					m_Scene->GetSceneResoruce()->SoundPlay("Brake");
			}
			// �ӵ��� ����
			accelerationLinear += BREAK_ACELERATION;
		}
		else
		{
			// �ƴҰ�� �극��ũ�� ����
			IsBrake = false;
			m_Scene->GetSceneResoruce()->SoundStop("Brake");
		}
	}

	bool drift = false;

	// ���� �÷��̾ ���� �������� ������ Ʋ���ִٸ�
	if (CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) &&
		!CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold))
	{
		float accelerationAngular = 0.0f;
		// ȸ���ӵ��� �����ϰ� ���� ȸ���ӵ��� ���� ��ġ �̻��̶�� �帮��Ʈ ����
		drift = IncreasingAngularAceleration(this, accelerationAngular);

		// �ӵ��� ���ӽ�Ų��.
		m_SpeedTurn = std::fmaxf(m_SpeedTurn - accelerationAngular,
			m_Kart->m_MaxTurningAngularSpeed * -1.0f);

		// �ڳʸ��� ȸ���ӵ��� ����
		ReduceLinearSpeedWhileTurning(this, accelerationLinear, m_SpeedTurn);

		// ���� ���о������� �������� �ʴٸ� �ִϸ��̼��� �ٲ۴�.
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

	// �÷��̾��� ���
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		if (drift && m_Height == 0.0f && !m_Falling)
		{
			// �帮��Ʈ ���� ���
			if (!m_Scene->GetSceneResoruce()->IsPlaying("Skid"))
				m_Scene->GetSceneResoruce()->SoundPlay("Skid");
		}
		else
		{
			// �帮��Ʈ ���� ����
			if(m_Scene->GetSceneResoruce()->IsPlaying("Skid"))
				m_Scene->GetSceneResoruce()->SoundStop("Skid");
		}
	}
}

void CDriver::UseGradientControl(float& accelerationLinear)
{
	Vector2 dirSum(0.0f, 0.0f);
	
	// ������ �� Ÿ���� ���ٺ��� ���Ѵ�.
	int tilesForward = m_SpeedForward < m_Kart->m_MaxNormalLinearSpeed / 4.0f ? 
		1 : m_Scene->GetTileMap()->GetAIFarVision() + m_FarVisionModifier;

	for (int i = 0; i < tilesForward; i++)
	{
		dirSum += CAIGradientDescent::GetNextDirection(m_Position + dirSum);
	}

	// ���� ȸ���ϴ� ����
	Vector2 evadeVector;

	bool evadeFound = false;

	// ���� ����� �ӵ��� ���͸� ���Ѵ�.
	Vector2 scaledForward(cosf(m_Angle + m_SpeedTurn * 0.15f) * m_SpeedForward * 0.06f,
		sinf(m_Angle + m_SpeedTurn * 0.15f) * m_SpeedForward * 0.06f);

	// ���� �ӵ��� ���� ��ġ �̻��̶��
	if (m_SpeedForward / m_Kart->m_MaxNormalLinearSpeed > 0.3f)
	{
		// ȸ�Ǹ� �� ��Ȳ���� ���Ѵ�.
		for (int i = 1; i < 12; i++)
		{
			if (CCollisionHash::GetInst()->Evade(this, m_Position + scaledForward * (float)i, m_HitboxRadius * 2.0f, evadeVector))
			{
				evadeFound = true;
				break;
			}
		}
	}

	// ȸ�� ����
	float evadeAngle = 0.0f;

	if (evadeFound)
	{
		// ������ ���͸� ���Ѵ�.
		Vector2 perpendicularDirection(scaledForward.y, scaledForward.x * -1.0f);

		// �� ���Ͱ��� ���絵�� ���Ѵ�.
		float dotProduct = perpendicularDirection.x * evadeVector.x + perpendicularDirection.y * evadeVector.y;

		// ��ֹ����� �Ÿ��� ���Ѵ�.
		float evadeModule = sqrtf(fabsf(evadeVector.x * evadeVector.x + evadeVector.y * evadeVector.y));

		// ȸ�ǰŸ� �̳����
		if (evadeModule < MAX_EVADE_DISTANCE)
		{
			// ȸ�� ������ ���Ѵ�.
			float evadePct = 1.0f - (evadeModule / MAX_EVADE_DISTANCE);

			// ���� ���絵�� ������� �ð�������� �� ���´�.
			// ���� ���絵�� ������ �ݽð� �������� �� ���´�.
			evadeAngle = evadePct * MAX_EVADE_ANGLE * (dotProduct > 0.0f ? -1.0f : 1.0f);
		}
	}

	CTileMap* tileMap = m_Scene->GetTileMap();

	if (tileMap)
	{
		// ������ ���ɼ��� �ִ��� Ȯ���Ѵ�.
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

		// ������ �ϴ� ��ġ�� ������ ���Ѵ�.
		float targetAngle = std::atan2f(dirSum.y, dirSum.x);

		// �÷��̾�� ����
		float angleP2P = 0.0f;
		// ���� ���
		float goHitBackMultiplier = 1.0f;

		// ���� �ڿ� ����̹��� �������
		if (m_Rank >= 0 && m_Rank < (int)EMenuPlayer::END - 1)
		{
			// �ٷ� �� ����̹��� ���Ѵ�.
			const CDriver* backPlayer = m_Positions[m_Rank + 1];

			// �÷��̾��� ���� �ƴ� ��� �ٸ� ������ �ʱ�ȭ�Ѵ�.
			goHitBackMultiplier = backPlayer->m_ControlType == EDriverControlType::PLAYER ? 1.0f : 8.0f;

			// �ڱ��� ��ġ���� �ٷ� �� ����̹��� ��ġ������ ���͸� ���Ѵ�.
			Vector2 vecP2P = m_Position - backPlayer->GetPos();

			// �Ÿ��� ���Ѵ�.
			float dP2P_2 = vecP2P.x * vecP2P.x + vecP2P.y * vecP2P.y;

			const int NUM_TILES_FOR_OCCLUSION = 6;
			const float DIST_FOR_OCCLUSION = (NUM_TILES_FOR_OCCLUSION / (float)MAP_TILES_WIDTH) * (NUM_TILES_FOR_OCCLUSION / (float)MAP_TILES_WIDTH);

			// ���� �Ÿ��� ���� �Ÿ� ���϶�� �浹�� �Ÿ�
			if (dP2P_2 < DIST_FOR_OCCLUSION)
			{
				// ������ ���Ѵ�.
				angleP2P = atan2f(vecP2P.y, vecP2P.x);
				angleP2P = angleP2P - m_Angle;
				angleP2P = NormalizeAngle(angleP2P);

				// ���� 90�� ���� ũ�ų� 270���� �۴ٸ�
				if (angleP2P > (float)M_PI_2 && angleP2P < 3.0f * M_PI_2)
				{
					angleP2P = 0.0f;
				}
				
				// ���� ���� ������ �����̸�
				if (backPlayer->IsImmune())
				{
					// �޾Ƴ���.
					angleP2P = NormalizeAngle(-angleP2P);
				}
			}
		}

		float goHitBackPlayer = (angleP2P < (float)M_PI ? -1.0f : 1.0f) * angleP2P / (float)m_ImpedimentModifier;

		float diff = targetAngle + evadeAngle - m_Angle - m_SpeedTurn * 0.15f + goHitBackPlayer / goHitBackMultiplier;

		diff = fmodf(diff, 2.0f * (float)M_PI);

		if (diff < 0.0f)
			diff += 2.0f * (float)M_PI;

		// �����ڽ������� �����Ѵ�.
		if (m_Height <= 0.0f && fabsf((float)M_PI - diff) > 0.85f * (float)M_PI)
		{
			if (m_IsRealPlayer && m_SpeedForward >= 0.4f * m_Kart->m_MaxNormalLinearSpeed)
			{
				// ���� ���
				const CDriver* player = m_Scene->GetPlayer();
				long long playerPosValue = CAIGradientDescent::GetPositionValue(player->GetPos()) +
					CAIGradientDescent::MAX_POSITION_MATRIX * player->m_Laps;

				long long currentPosValue = CAIGradientDescent::GetPositionValue(m_Position) + CAIGradientDescent::MAX_POSITION_MATRIX * m_Laps;

				// ���߿��� �޴����� ���� CC �ɼ��� �����ϱ�
				int maxBehind = CAIGradientDescent::MAX_DISTANCE_BEHIND[(int)m_CCoption];

				int minProbBehind = CAIGradientDescent::MIN_PROB_BEHIND[(int)m_CCoption];

				int maxAhead = CAIGradientDescent::MAX_DISTANCE_AHEAD[(int)m_CCoption];

				float minProbAhead = CAIGradientDescent::MIN_PROB_AHEAD[(int)m_CCoption];

				long long distance = std::abs(playerPosValue - currentPosValue);

				// ���� AI�� �� ������ ���� ���
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
					// �ռ������� ���� ���
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

			// �������� ���´�.(�� �ð�)
			if (diff > (float)M_PI)
			{
				m_SpeedTurn = std::fmaxf(m_SpeedTurn - accelerationAngular * turnMultiplier,
					m_Kart->m_MaxTurningAngularSpeed * -totalMultiplier);

				ReduceLinearSpeedWhileTurning(this, accelerationLinear, m_SpeedTurn);

				m_Animator.GoLeft(m_SpeedUpwards < m_Kart->m_MaxTurningAngularSpeed * -0.4f);
			}
			// ���������� ���´�.(�ð�)
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
	// ��ȭ�� ������Ʈ
	const int CONSECUTIVE_INCREMENTS_FOR_BACKWARDS = 5;

	// ��輱 �˻�
	if (m_Position.x < 1e-4f || m_Position.x > 1.0f - 1e-4f || m_Position.y < 1e-4f || m_Position.y > 1.0f - 1e-4f)
		return;

	int gradient = CAIGradientDescent::GetPositionValue(int(m_Position.x * MAP_TILES_WIDTH), int(m_Position.y * MAP_TILES_HEIGHT));

	// ���̳� �������� Ÿ�� ���� �ְų� ��ȭ���� ���ٸ�
	if (gradient == -1 || gradient == m_LastGradient)
		return;

	// ������ ��ȭ���� �ʱ�ȭ ���� �ʾ��� ���
	if (m_LastGradient == -1)
	{
		m_LastGradient = gradient;
		return;
	}

	if (gradient > m_LastGradient)
	{
		m_ConsecutiveGradientIncrement = min(m_ConsecutiveGradientIncrement + 1, CONSECUTIVE_INCREMENTS_FOR_BACKWARDS);

		// ���� ����ŭ �ڷ� ���ٸ�
		if (m_ConsecutiveGradientIncrement == CONSECUTIVE_INCREMENTS_FOR_BACKWARDS)
			m_GoingForward = false;
	}
	else
	{
		m_ConsecutiveGradientIncrement = max(m_ConsecutiveGradientIncrement - 1, CONSECUTIVE_INCREMENTS_FOR_BACKWARDS * -1);

		// ���� ��ġ�� ���� �����ߴٸ� ����ε� ���� ���� �ִٴ� ��
		if (m_ConsecutiveGradientIncrement == CONSECUTIVE_INCREMENTS_FOR_BACKWARDS * -1)
			m_GoingForward = true;
	}

	int diff = gradient - m_LastGradient;

	// ���� �������� �� üũ������ ũ�ٸ� 
	// �� ���� ���Ҵٴ� ��
	if (diff > CAIGradientDescent::GRADIENT_LAP_CHECK)
	{
		m_Laps = m_Laps + 1;

		if (m_Laps == 6)
		{
			// ���̽� ���ֽð� ���
			float finishTime = m_Scene->GetCurrentRaceTime();
			CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");
			gui->AddFinishTime(m_CurrentPlayer, finishTime);
		}

		if (m_Laps == 5 && m_MaxLapSoFar == 4 && m_ControlType == EDriverControlType::PLAYER)
		{
			// ���� ������� ��� ������ �����Ѵ�.
			// �׸��� ������ �� �˸����� ������ �� BGM�� ����Ѵ�.
			m_Scene->GetSceneResoruce()->StopAllMusic();
			m_Scene->GetSceneResoruce()->SoundPlay("LakituFinalLap");
			m_Scene->GetSceneResoruce()->SoundPlay("FinalLap");
		}

		if (m_Laps > m_MaxLapSoFar)
		{
			if (m_ControlType == EDriverControlType::PLAYER && m_Laps < 6)
			{
				// ����ѹ��� ��ȯ���� ���� �� ǥ��
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

	// ��ȭ�� ������Ʈ�Ŀ� ������ üũ�Ѵ�.
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		// �Ųٷΰ����ִٴ� ����ѹ��� ��ȯ.
		m_Scene->GetLakitu()->WarningWrongDir(!m_GoingForward);
	}
}

void CDriver::ApplyMushroom()
{
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		m_Scene->GetSceneResoruce()->SoundPlay("Mushroom");
	}

	// �ӵ��� �ִ� �ӵ��� �ö󰣴�.
	m_SpeedForward = m_Kart->m_MaxNormalLinearSpeed;

	// �ӵ� �� ����
	PushStateEnd(EDriverState::SPEED_UP, m_Scene->GetCurrentRaceTime() + SPEED_UP_DURATION);
}

void CDriver::ApplyStar()
{
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		m_Scene->GetSceneResoruce()->SoundPlay("Star");
	}

	// ��Ÿ ����
	PushStateEnd(EDriverState::STAR, m_Scene->GetCurrentRaceTime() + STAR_DURATION);
	m_Animator.Star(STAR_DURATION);
}

void CDriver::ApplyThunder(float duration)
{
	// ȸ���ӵ��� 0���� �����.
	m_SpeedTurn = 0.0f;

	// ���� �ӵ��� ������ ������ ����
	m_SpeedForward = std::fmin(m_SpeedForward, m_Kart->m_MaxNormalLinearSpeed * 0.3f);

	// ���� �Ҵ� ���� �߰�
	PushStateEnd(EDriverState::UNCONTROLLED, m_Scene->GetCurrentRaceTime() + UNCONTROLLED_DURATION);

	if (m_ControlType == EDriverControlType::PLAYER)
	{
		// �÷��̾� ���� �ǰ� �Ҹ� ���
		m_Scene->GetSceneResoruce()->SoundPlay("Thunder");
		CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");

		if (m_Scene->GetSceneResoruce()->IsPlaying("ItemReel"))
			m_Scene->GetSceneResoruce()->SoundStop("ItemReel");

		gui->SetItem(EItem::NONE);
	}

	// �۾��� ����
	m_Animator.Small(duration);
	m_CurrentItem = EItem::NONE;

	// �ӵ� ���� ����
	PushStateEnd(EDriverState::SPEED_DOWN, m_Scene->GetCurrentRaceTime() + duration);
}

void CDriver::ApplyFeather()
{
	if (m_ControlType == EDriverControlType::PLAYER)
	{
		CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");

		// ���� ����
		const float JUMP_INCLINATION = 80.0f / 90.0f;

		// ��¼ӵ�
		m_SpeedUpwards = JUMP_INCLINATION * m_SpeedForward * 400.0f * 8.0f;
		m_SpeedUpwards = std::fmax(m_SpeedUpwards, 60.0f);
		m_SpeedUpwards = std::fmin(m_SpeedUpwards, 80.0f);
		m_SpeedForward = (1.0f - JUMP_INCLINATION) * m_SpeedForward;

		m_SpeedForward = (float)std::fmax(m_SpeedForward, 0.1);

		// ���� ����ȭ
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
			// �����Ҹ� ���
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

	// ���� ���� ������̶��
	if (land == ETileProperty::RAMP_HORIZONTAL)
	{

		// ���� ������ 0 ~ 180�� ���̶��
		if (normlizedAngle >= 0 && normlizedAngle <= (float)M_PI)
			// ����� ������ 90�� ����
			m_FlightAngle = (float)M_PI_2;
		// �� �ܶ��
		else
			// ����� ������ 270��
			m_FlightAngle = 3 * (float)M_PI_2;
	}
	// ���� ������� ���
	else if (land == ETileProperty::RAMP_VERTICAL)
	{
		// �����ʿ��� �������� �ǳΰ�� �� ���� ������ 90���� 270�� ���̶��
		if (normlizedAngle > (float)M_PI_2 && normlizedAngle < 3 * (float)M_PI_2)
			m_FlightAngle = (float)M_PI;
		else
			// �ƴ϶�� ���ʿ��� �������� ���� ������ 0
			m_FlightAngle = 0;
	}

	if (m_ControlType == EDriverControlType::PLAYER)
	{
		// ���� ���� ���
		m_Scene->GetSceneResoruce()->SoundPlay("Jump");
	}
}

void CDriver::ApplyHit()
{
	// ������ �ƴ϶�� ��Ʈ
	if (!IsImmune())
	{
		// ���ΰ� ���� ����
		AddCoin(-2);
		PopStateEnd(EDriverState::SPEED_UP);
		PopStateEnd(EDriverState::MORE_SPEED_UP);

		// ȸ���ӵ� ����
		m_SpeedTurn = 0.0f;

		m_SpeedForward = std::fmin(m_SpeedForward, m_Kart->m_MaxNormalLinearSpeed * 0.6f);

		if (m_ControlType == EDriverControlType::PLAYER && CanDrive())
		{
			// �ǰ� �Ҹ� ���
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

// �����߰�
void CDriver::AddCoin(int amount)
{
	// ���� ������ 10���� ���� ���
	if (m_Coins + amount > 10)
	{
		// �������� ���μ��� 10���� ���ش�.
		// ��) m_Coins 9, amount 3�� ��� amount�� 10 - 9  = 1
		// ���� �ϳ� �߰��ȴ�.
		amount = 10 - m_Coins;
	}
	// ���� 0���� ������� (����)
	else if (m_Coins + amount < 0)
	{
		// �������� ������ 0���� ���ش�.
		// ��) m_Coins 2, amount 3�ϰ�� 0 - 2 amount�� = -2
		amount = 0 - m_Coins;
	}

	m_Coins += amount;

	// ���� ȿ���� �߰��Ѵ�.
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
	// ���� ���� �����Ҵɵ� �ƴϰ� ���� ��Ȳ�� �ƴ϶�� ���� ���� ����
	return !(m_CurrentState & (int)EDriverState::UNCONTROLLED) &&
		!(m_CurrentState & (int)EDriverState::STOPPED);
}

bool CDriver::IsImmune() const
{
	// ��Ÿ��� ����
	return m_CurrentState & (int)EDriverState::STAR;
}

bool CDriver::IsVisible() const
{
	// ���� ~���¿��� ������¸� AND �Ŀ� ���̶�� ���̴� ����
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
			// ������ ���� ���� ���� ���
			m_Scene->GetSceneResoruce()->SoundPlay("ItemReel");
		}

		// Gui ������ �̹��� ����
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

	// �����ӵ� �ʱ�ȭ.
	float accelerationLinear = 0.0f;

	// ���� ���� ����ִٸ�
	// ���������ŭ �ӵ��� �����Ѵ�.
	if (m_Height <= 0.0f)
		accelerationLinear += FRICTION_LINEAR_ACELERATION;


	// ���� ȸ���� �ϰ� ���� �ʴٸ�
	if ((!CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) &&
		!CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold))
		||
		(CInput::GetInst()->GetKeyState(VK_LEFT, EKeyState::KeyState_Hold) && m_SpeedTurn > 0.0f) ||
		(CInput::GetInst()->GetKeyState(VK_RIGHT, EKeyState::KeyState_Hold) && m_SpeedTurn < 0.0f))
	{
		// ȸ���ӵ��� õõ�� �����Ѵ�.
		if (m_SpeedTurn > 0.001f)
			m_SpeedTurn /= 1.2f;
		else
			m_SpeedTurn = 0.0f;
	}

	// ���� ���´� �������ش�.
	PopStateEnd(m_Scene->GetCurrentRaceTime());

	

	// ���� �����Ҵ� ���¶��
	if (m_CurrentState & (int)EDriverState::UNCONTROLLED)
	{
		// �ǰ� �ִϸ��̼� ���
		m_Animator.Hit();
	}
	else if (m_CurrentState & (int)EDriverState::STOPPED)
	{

	}
	else
	{
		// �ƴ϶�� ���� ���
		if (m_Height <= 0.0f && !m_JumpByFeather)
		{
			m_Animator.GoForward();
		}

		// ���� ��Ʈ�� ���¿����� AI, Player�� ��Ʈ�� �Լ� ȣ��
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

	// ��ũ�� ���� ���ʽ� �ӵ�
	accelerationLinear *= 1.0f + POSITION_ACCELERATION_BONUS_PCT * m_Rank;

	// ���� �����̶�� �ӵ��� 2��
	if (IsImmune())
		accelerationLinear *= 2.0f;

	CTileMap* currentTileMap = m_Scene->GetTileMap();

	if (currentTileMap)
	{
		ETileProperty tile = currentTileMap->GetTile(int(m_Position.x * MAP_TILES_WIDTH), int(m_Position.y * MAP_TILES_HEIGHT))->GetTileProperty();

		// ���� Ÿ���� �������� Ÿ���̰� ���� ���°� ������ �ƴϰų� �ӵ� ��� ���°� �ƴ� ����
		if ((tile == ETileProperty::SLOW || tile == ETileProperty::SLOW_2) && (~m_CurrentState & (int)EDriverState::STAR) &&
			(~m_CurrentState & (int)EDriverState::SPEED_UP))
		{
			// �ӵ��� �پ���.
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


		// ���� ����ְ�
		if (m_Height <= 0.0f)
		{

			// ���� �⸧�� ��Ҵٸ�
			if (tile == ETileProperty::OIL_SLICK)
			{
				// �ǰ�
				ApplyHit();
				m_SpeedForward = std::fmaxf(m_SpeedForward, 0.01f);
			}
			// ����̶�� ����
			else if (tile == ETileProperty::RAMP_HORIZONTAL || tile == ETileProperty::RAMP_VERTICAL)
			{
				JumpByRamp(tile);
			}
			// ���� �г��̶��
			else if (tile == ETileProperty::ACCEL_PANEL)
			{
				if (~m_CurrentState & (int)EDriverState::SPEED_UP &&
					m_ControlType == EDriverControlType::PLAYER)
				{
					// ���� �Ҹ� ���
					m_Scene->GetSceneResoruce()->SoundPlay("Mushroom");
				}

				// cpu�� ��� ���� ���ʽ�
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
				// ������ �г� ���ε�� �浹������
				m_Scene->CollideWithFloor(this);
			}
		}
	}

	// �߷� �ۿ�
	if ((m_Height > 0.0f || m_SpeedUpwards > 0.0f) && !m_OnLakitu)
	{
		// �߷� ���
		const float gravityAcelration = -9.8f * 16.0f;

		// �������� ���� ���̴� ����̴�. ������ �׷������� ������ ����ŭ ���ش�. ��ũ���� y���� ����
		m_Height = m_Height + m_SpeedUpwards * deltaTime + 0.5f * gravityAcelration * deltaTime * deltaTime;


		if (m_Height < 0.0f)
		{
			if (m_ControlType == EDriverControlType::PLAYER)
			{
				// �������� ���
				m_Scene->GetSceneResoruce()->SoundPlay("Landing");

				if (m_Scene->GetSceneResoruce()->IsPlaying("Feather"))
					m_Scene->GetSceneResoruce()->SoundStop("Feather");
			}

			// ���� ������ �帮��Ʈ ���� �ϴ� �� ������ ���
			// �帮��Ʈ Ű ��
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
		
		// ��� �ӵ� ������Ʈ
		m_SpeedUpwards = m_SpeedUpwards + gravityAcelration * deltaTime;

		if (m_Height <= 0.0f)
			m_SpeedUpwards = 0.0f;
	}


	// �ִ� ���� �ӵ�
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


	// �ӵ��� ������ ��ȭ
	float deltaAngle = m_SpeedTurn * deltaTime;
	float deltaSpace = m_SpeedForward * deltaTime + accelerationLinear * (deltaTime * deltaTime) / 2.0f;

	deltaSpace = std::fminf(deltaSpace, maxLinearSpeed * deltaTime);
	deltaSpace = std::fmaxf(deltaSpace, 0.0f);

	// �ӵ� ����
	m_SpeedForward += accelerationLinear * deltaTime;
	m_SpeedForward = std::fminf(m_SpeedForward, maxLinearSpeed);
	m_SpeedForward = std::fmaxf(m_SpeedForward, 0.0f);

	// ���� ���� ���°� ���߿� �ִ� ���¶�� ������ ���ఢ���� �ٲ��.
	float movementAngle = m_Height == 0.0f ? m_Angle : m_FlightAngle;

	Vector2 deltaPosition = Vector2(cosf(movementAngle), sinf(movementAngle)) * deltaSpace;

	// ���� �浹 ������� �ִٸ� �����ش�.
	deltaPosition += m_CollisionMomentum;
	// �浹����� ����
	m_CollisionMomentum /= 1.2f;
	// �浹 �ӵ��� �߰�
	deltaPosition += m_VectorialSpeed * deltaTime;
	m_VectorialSpeed /= 1.3f;

	// ���� ���� ����� �ȴٸ� �߶�
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

		// ���� ���� ������� ���� ������ �ƴϰ� ���з����� ������ �ƴ϶��
		// Ÿ�ϰ� �浹 üũ
		if (!m_JumpByRamp && !m_JumpByFeather && tileProp != ETileProperty::BLOCK)
			ImprovedCheckOfMap(this, m_Position, deltaPosition);
	}

	// �߶����̶��
	if (m_Falling)
	{
		// ȸ���ӵ��� �ʱ�ȭ
		m_SpeedTurn = 0.0f;
		m_SpeedForward = m_SpeedForward / 1.1f;

		if (m_ControlType == EDriverControlType::PLAYER)
		{
			// �߶� �ִϸ��̼� 
			m_Animator.Fall();
			CRaceGUI* gui = m_Scene->FindUIWindow<CRaceGUI>("Race GUI");

			// ���̵� �� �ƿ�
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
		// AI�� �߶����� ���
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

	// ��ȭ�� ����
	UpdateGradientPosition();
	// �ִϸ����� ������Ʈ
	m_Animator.Update(m_SpeedForward, m_SpeedTurn, m_Height, deltaTime, m_Position);

	// ���ο� ��ġ ������ ����Ѵ�. (AI)
	if (!m_Falling && m_Scene->GetCurrentRaceTime() - m_PathLastUpdatedAt >
		FOLLWED_PATH_UPDATE_INTERVAL)
	{
		auto iter = m_FollwedPath.rbegin();
		int numOfUpdatesWithoutMoving = 0;

		// AI�� ���
		if (m_ControlType != EDriverControlType::PLAYER)
		{
			while (iter != m_FollwedPath.rend())
			{
				// ���� Ÿ�� �ϳ��� ��� ������ �������ٸ� �ٷ� ���ο� ��ġ������ ����Ѵ�.
				if (fabs(m_Position.x - iter->x) > (1.0f / MAP_TILES_WIDTH))
					break;

				// ��������
				if (fabs(m_Position.y - iter->y) > (1.0f / MAP_TILES_HEIGHT))
					break;

				// ���� Ư�� Ƚ����ŭ �������� �����ٸ�
				if (++numOfUpdatesWithoutMoving >= STPES_STILL_FOR_RELOCATION)
				{
					// �׵��� ����� ��η� ���ο� ��θ� ������.
					int size = (int)m_FollwedPath.size();

					// �׵��� ����� ��� ��Ͽ��� �����Ѵ�.
					for (int i = max(size - STPES_STILL_FOR_RELOCATION - 5, 1); i < size; i++)
						m_FollwedPath.pop_back();

					// ���ο� ��ġ�� ������
					RelocateToNearestGoodPosition();
					break;
				}

				iter++;
			}
		}

		// ����Ѵ�.
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
		// ���� �ִϸ��̼� �������� ���ؼ� �׸����� �������� �����Ѵ�.
		m_Shadow->SetScale(m_Animator.GetTexture()->GetScale() * m_Scene->GetCircuitHeightPCT() * 2.0f);
		// �׸��ڵ� ���� �ӵ��� �˸°� �����δ�.
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
	// ���� ���� ������ �������� ������� �浹�� ���õȴ�.
	if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
		return false;

	// ���� ���߿� �ϳ��� �����̶�� �浹�� ���ó� ���õȴ�.
	if (IsImmune() && !otherIsImmune)
	{
		// �ڱ��ڽ��� �����̰� ������ ������ �ƴϱ⿡ �浹������ ����
		collisionData = CollisionData(Vector2(0.0f, 0.0f), 0.4f, ECollisionType::HIT);
		return true;
	}
	else if (!IsImmune() && otherIsImmune)
		return false;

	// �Ѵ� ������ �ƴϰų� �Ѵ� ������ ���
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

	// ���� 0�Ͻÿ��� ���� �Ǵ� �� ����
	if (stepsFromGoal == 0)
	{
		// ���� ��ĭ ������ �����δ�.
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

	// ���� ��ġ�� ���� �������� �󸶳� ���̰������� ���� 0�̶�� �ٷ� ��������
	// �ƴ϶�� 0�� ����
	int stepsFromGoal = CAIGradientDescent::GetPositionValue(m_Position) == 0 ? 1 : 0;

	CTileMap* tileMap = m_Scene->GetTileMap();

	if (tileMap)
	{
		// ������������ ��������� ����ؼ� �̵��Ѵ�.
		while (tileMap->GetTile(m_Position * MAP_ASSETS_HEIGHT)->GetTileProperty() == ETileProperty::HAZARD)
		{
			UpdatePosition(m_Position, stepsFromGoal);
		}

		// �ణ�� �������� �ش�.
		int times = rand() % 6 + 2;

		// ������ �縸ŭ �� ��¦ ������Ʈ
		for (int i = 0; i < times; i++)
		{
			UpdatePosition(m_Position, stepsFromGoal);
		}

		// �ٽ� ������������ ���Ƴ������� �ݺ�
		while (tileMap->GetTile(m_Position * MAP_ASSETS_HEIGHT)->GetTileProperty() == ETileProperty::HAZARD)
		{
			UpdatePosition(m_Position, stepsFromGoal);
		}

		// ���� ������ ���Ѵ�.
		Vector2 next = CAIGradientDescent::GetNextDirection(m_Position);

		// ���� 0���� ũ�ٸ�
		if (stepsFromGoal > 0)
		{
			// �� ��ġ��ŭ ��ġ�� ���� �����Ѵ�.
			m_Position -= next * float(stepsFromGoal + 2);
		}

		// ������ ����
		m_Angle = std::atan2f(next.y, next.x);
	}

}