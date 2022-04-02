#include "KartProperties.h"

#include "../GameInfo.h"

// 균형타입일시
CKartProperties CKartProperties::BALACNED(0.11f, 0.14f, 0.102f, 1.0f, false);

// 가속타입일시
CKartProperties CKartProperties::ACCELERATION(0.15f, 0.13f, 0.10175f, 1.0f, false);

// 무게타입일시
CKartProperties CKartProperties::HEAVY(0.10f, 0.13f, 0.1025f, 2.0f, true);

// 핸들링 타입일시
CKartProperties CKartProperties::HANDLING(0.12f, 0.15f, 0.1015f, 0.5f, false);

CKartProperties CKartProperties::INVISIBLE(0.2f, 0.3f, 0.2f, 10.0f, false);

float CKartProperties::PLAYER_CHARACTER_MULTIPLIER = 1.0f;

CKartProperties CKartProperties::PLAYER = BALACNED;


// 수치를 곱해서 최종 스테이터스를 만들어낸다.
void CKartProperties::SetScaleFactor(const float scaleFactor, const float playerCharacterMultiplier)
{
    BALACNED = std::move(CKartProperties(0.11f * scaleFactor, 0.14f * scaleFactor / 1.5f, 0.102f * scaleFactor, 1.0f, false));

    ACCELERATION = std::move(CKartProperties(0.15f * scaleFactor, 0.13f * scaleFactor / 1.5f, 0.10175f * scaleFactor, 1.0f, false));

    HEAVY = std::move(CKartProperties(0.1f * scaleFactor, 0.13f * scaleFactor / 1.5f, 0.1025f * scaleFactor, 2.0f, true));

    HANDLING = std::move(CKartProperties(0.12f * scaleFactor, 0.15f * scaleFactor / 1.5f, 0.1015f * scaleFactor, 0.5f, false));

    PLAYER_CHARACTER_MULTIPLIER = playerCharacterMultiplier;
}

// 플레이어 스테이터스 조정
const CKartProperties& CKartProperties::MakePlayer() const
{
    PLAYER = std::move(CKartProperties(m_MotorAcceleration * PLAYER_CHARACTER_MULTIPLIER,
        m_TurningAcceleration * PLAYER_CHARACTER_MULTIPLIER,
        m_MaxNormalLinearSpeed * PLAYER_CHARACTER_MULTIPLIER,
        m_Weight,
        m_Convex));

    return PLAYER;
}
