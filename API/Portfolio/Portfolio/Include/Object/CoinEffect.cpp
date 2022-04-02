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

	// 딜레이 만큼 지연시킨다.
	if (m_CurrentTime < m_Delay)
	{
		return;
	}
	else
	{
		// 만약 딜레이만큼 충분히 지연이 됬다면
		if (m_Delay > 0.0f)
		{	
			// 시작한다.
			// 딜레이는 다시 0으로 초기화해서 여기로 들어오지 않도록 한다.
			m_CurrentFrame = 0;
			m_Delay = 0.0f;
			SetCurrentCoinTexture(0);
		}
	}

	// 배속
	float ratio = 1.0f;

	// 만약 추가되는 코인이라면 간단하게 플레이어의 좌표를 기준으로 생성
	if (m_Positive)
	{
		m_Position = m_Driver->GetPos();
	}
	else
	{
		// 코인이 감소할경우 살짝 오프셋을 준다.
		m_Position += Vector2(cosf(m_Angle), sinf(m_Angle)) * m_SpeedForward * deltaTime;
		ratio += 0.5f;
	}

	// 수직 스피드
	m_VerticalSpeed += COIN_GRAVITY * deltaTime * ratio;

	// 높이를 낮춰준다.
	m_RelativeHeight += m_VerticalSpeed * deltaTime;

	// 높이는 운전자의 높이 + 현재 코인의 높이
	m_Height = m_Driver->GetHeight() + m_RelativeHeight;


	// 만약 코인의 높이가 0보다 적어진다면 끝
	if (m_RelativeHeight < 0.0f)
	{
		m_Used = true;
	}

	// 현재 누적시간이 전환시간보다 높다면 프레임 전환.
	if (m_CurrentTime > COIN_TIME_BETWEEN_FRAMES)
	{
		m_CurrentFrame = (m_CurrentFrame + 1) % NUM_COIN_TEXTURES;
		// 시간은 다시 0으로 초기화한다.
		m_CurrentTime = 0.0f;

		// 만약 모든 프레임을 다 썼다면 끝
		if (m_CurrentFrame >= NUM_COIN_TEXTURES)
			m_Used = true;
		else
		{
			SetCurrentCoinTexture(m_CurrentFrame);
		}
	}
	

	// 소리는 한번만 재생한다.
	if (m_Once)
	{
		m_Once = false;

		if (m_Driver->m_ControlType == EDriverControlType::PLAYER)
		{
			// 사운드 재생
			m_Scene->GetSceneResoruce()->SoundPlay("Coin");
		}
	}
}
