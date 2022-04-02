#include "CoinEffect.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"


inline void CCoinEffect::SetCurrentCoinTexture(int frame)
{
	m_Texture = m_Scene->GetSceneResoruce()->FindTexture("Coin_" + std::to_string(frame + 1));
	m_Texture->SetPivot(m_Texture->GetWidth() / 2.0f, m_Texture->GetHeight());
}

CCoinEffect::CCoinEffect(const CDriver* driver, CRaceScene* scene, const std::string& name, const float delay, const bool positive) :
	CItem(Vector2(driver->GetPos().x* MAP_ASSETS_WIDTH, driver->GetPos().y* MAP_ASSETS_HEIGHT),
		-0.5f, 0.0f, driver->GetHeight(), scene, name),
	m_Driver(driver),
	m_RelativeHeight(2.0f),
	m_Delay(delay),
	m_Positive(positive),
	m_VerticalSpeed(COIN_JUMP_SPEED),
	m_SpeedForward(0.05f + driver->m_SpeedForward * 2.0f),
	m_Angle(driver->GetAngle()),
	m_CurrentTime(0.0f),
	m_CurrentFrame(0),
	m_Once(true)
{
	m_Texture = nullptr;
}

bool CCoinEffect::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
	return false;
}

void CCoinEffect::Init()
{

}

void CCoinEffect::Update(float deltaTime)
{
	m_CurrentTime += deltaTime;

	// ������ ��ŭ ������Ų��.
	if (m_CurrentTime < m_Delay)
	{
		return;
	}
	else
	{
		// ���� �����̸�ŭ ����� ������ ��ٸ�
		if (m_Delay > 0.0f)
		{	
			// �����Ѵ�.
			// �����̴� �ٽ� 0���� �ʱ�ȭ�ؼ� ����� ������ �ʵ��� �Ѵ�.
			m_CurrentFrame = 0;
			m_Delay = 0.0f;
			SetCurrentCoinTexture(0);
		}
	}

	// ���
	float ratio = 1.0f;

	// ���� �߰��Ǵ� �����̶�� �����ϰ� �÷��̾��� ��ǥ�� �������� ����
	if (m_Positive)
	{
		m_Position = m_Driver->GetPos();
	}
	else
	{
		// ������ �����Ұ�� ��¦ �������� �ش�.
		m_Position += Vector2(cosf(m_Angle), sinf(m_Angle)) * m_SpeedForward * deltaTime;
		ratio += 0.5f;
	}

	// ���� ���ǵ�
	m_VerticalSpeed += COIN_GRAVITY * deltaTime * ratio;

	// ���̸� �����ش�.
	m_RelativeHeight += m_VerticalSpeed * deltaTime;

	// ���̴� �������� ���� + ���� ������ ����
	m_Height = m_Driver->GetHeight() + m_RelativeHeight;


	// ���� ������ ���̰� 0���� �������ٸ� ��
	if (m_RelativeHeight < 0.0f)
	{
		m_Used = true;
	}

	// ���� �����ð��� ��ȯ�ð����� ���ٸ� ������ ��ȯ.
	if (m_CurrentTime > COIN_TIME_BETWEEN_FRAMES)
	{
		m_CurrentFrame = (m_CurrentFrame + 1) % NUM_COIN_TEXTURES;
		// �ð��� �ٽ� 0���� �ʱ�ȭ�Ѵ�.
		m_CurrentTime = 0.0f;

		// ���� ��� �������� �� ��ٸ� ��
		if (m_CurrentFrame >= NUM_COIN_TEXTURES)
			m_Used = true;
		else
		{
			SetCurrentCoinTexture(m_CurrentFrame);
		}
	}
	

	// �Ҹ��� �ѹ��� ����Ѵ�.
	if (m_Once)
	{
		m_Once = false;

		if (m_Driver->m_ControlType == EDriverControlType::PLAYER)
		{
			// ���� ���
			m_Scene->GetSceneResoruce()->SoundPlay("Coin");
		}
	}
}
