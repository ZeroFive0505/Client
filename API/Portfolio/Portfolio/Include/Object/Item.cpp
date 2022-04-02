#include "Item.h"
#include "../Scene/RaceScene.h"
#include "../UI/RaceGUI.h"
#include "Banana.h"
#include "GreenShell.h"
#include "RedShell.h"
#include "../GameInfo.h"
#include "../AI/AIGradientDescent.h"
#include "../Scene/SceneResource.h"

CRaceScene* CItem::m_StaticScene = nullptr;



const float SECOND_PER_LAP = 30.0f;
const float PROB_OF_USING_PER_LAP = 0.5f;
const float CHECKS_PER_LAP = SECOND_PER_LAP * UPDATES_PER_SECOND;
const float PROB = 1.0f - powf(1.0f - PROB_OF_USING_PER_LAP, 1.0f / CHECKS_PER_LAP);
const float BANANA_TRAVEL_DISTANCE = 0.20155464379f;
const float MAX_DIFF = 0.1f;

inline float StrategyHighest()
{
	return 0.6f;
}


inline float StrategyLowest()
{
	return PROB;
}

inline float ScaleProbability(const float percent, const float factor = 0.45f)
{
	float base = factor;

	for (int i = 10; i >= 1; i--)
	{
		if (percent < base)
			return 1.0f / i - 0.1f;

		base += (1.0f - base) * factor;
	}

	return 1.0f;
}

std::pair<float, bool> StrategyLessThanTenCoins(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	if (user->GetCoins() > 8)
	{
		float prob = StrategyLowest();

		return std::make_pair(prob, true);
	}
	else
	{
		float prob = StrategyHighest();

		return std::make_pair(prob, true);
	}
}

std::pair<float, bool> StrategyBetterWhenLast(const CSharedPtr<CDriver>& user,
	const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	int userPos = user->GetRank();

	float prob = StrategyLowest() + StrategyHighest() * ScaleProbability(userPos / 7.0f);

	return std::make_pair(prob, true);
}

std::pair<float, bool> StrategySlowOrStraightLine(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	if (user->m_SpeedForward / user->m_Kart->m_MaxNormalLinearSpeed < 0.3f)
	{
		float prob = StrategyHighest();

		return std::make_pair(prob, true);
	}

	Vector2 delta(0.0f, 0.0f);

	for (unsigned int i = 0; i < 10; i++)
	{
		// AI 코드
		delta += CAIGradientDescent::GetNextDirection(user->GetPos() + delta);
	}

	float angleDiff = atan2f(delta.y, delta.x) - user->GetAngle();

	angleDiff = fmodf(angleDiff, 2.0f * (float)M_PI);

	if (angleDiff < (float)M_PI * -1.0f)
		angleDiff += 2.0f * (float)M_PI;
	if (angleDiff > (float)M_PI)
		angleDiff -= 2.0f * (float)M_PI;

	float prob = StrategyLowest() + StrategyHighest() * ScaleProbability(1.0f - std::abs(angleDiff / (float)M_PI));

	return std::make_pair(prob, true);
}

std::pair<float, bool> StrategyBanana(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	unsigned int userPos = user->GetRank();

	// 뒤에 있는 3명을 노린다.
	for (size_t i = userPos + 1, j = 0; i < ranking.size() && j < 3; i++, j++)
	{
		const CDriver* target = ranking[i];

		Vector2 distance = target->GetPos() - user->GetPos();

		float length2 = distance.x * distance.x + distance.y * distance.y;

		if (length2 > 0.01f)
			continue;

		float angleDiff = atan2f(distance.y, distance.x) - target->GetAngle() * -1.0f;

		if (angleDiff < (float)M_PI * -1.0f)
			angleDiff += 2.0f * (float)M_PI;

		if (angleDiff > (float)M_PI)
			angleDiff -= 2.0f * (float)M_PI;

		if (std::abs(angleDiff) * length2 < (float)M_PI / 4.0f * 0.004f)
		{
			float prob = StrategyHighest() * ScaleProbability(1.0f - std::abs(angleDiff / (float)M_PI), 0.25f);

			return std::make_pair(prob, false);
		}
	}

	Vector2 bananaPos = user->GetPos() + Vector2(cosf(user->GetAngle()), sinf(user->GetAngle())) * BANANA_TRAVEL_DISTANCE;

	// 후방의 5명을 노린다.
	for (unsigned int i = userPos - 2, j = 0; i < ranking.size() && j < 5; i++, j++)
	{
		// 자기 자신이라면 패스
		if (i == userPos)
			continue;

		// 타겟
		const CDriver* target = ranking[i];

		// 거리를 구한다.
		Vector2 distance = target->GetPos() - user->GetPos();

		// 일정 거리 이상이면 패스
		if (distance.x * distance.x + distance.y * distance.y > 0.04f)
			continue;

		Vector2 pos = target->GetPos();

		for (unsigned int k = 0; k < 3; k++)
		{
			// AI 코드
			pos += CAIGradientDescent::GetNextDirection(pos);
		}

		for (unsigned int k = 0; k < 15; k++)
		{
			// AI 코드
			pos += CAIGradientDescent::GetNextDirection(pos);

			if (std::abs(bananaPos.x - pos.x) < 1.0f / MAP_TILES_WIDTH &&
				std::abs(bananaPos.y - pos.y) < 1.0f / MAP_TILES_HEIGHT)
			{
				return std::make_pair(StrategyHighest(), true);
			}
		}
	}

	float prob = StrategyLowest();

	return std::make_pair(prob, false);
}

std::pair<float, bool> StrategyUserInFront(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	const auto CheckAngle = [&user](const CDriver* target,
		const float maxDist2, const float desiredAngle)
	{
		Vector2 distance = target->GetPos() - user->GetPos();

		float length2 = distance.x * distance.x + distance.y * distance.y;

		if (length2 > maxDist2)
			return 2.0f * (float)M_PI * length2;

		Vector2 movementDisplacement = Vector2(cosf(target->GetAngle()), sinf(target->GetAngle())) *
			target->m_SpeedForward * sqrtf(length2);

		Vector2 shellPos = target->GetPos() + movementDisplacement;
		Vector2 throwDelta = shellPos - user->GetPos();
		float angleDiff = atan2f(throwDelta.y, throwDelta.x) - desiredAngle;

		angleDiff = fmodf(angleDiff, 2.0f * (float)M_PI);

		if (angleDiff < (float)M_PI * -1.0f)
			angleDiff += 2.0f * (float)M_PI;
		if (angleDiff > (float)M_PI)
			angleDiff -= 2.0f * (float)M_PI;

		return angleDiff - length2;
	};

	unsigned int userPos = user->GetRank();

	// 꼴지가 아니라면 뒤로 던지는 경우의 수도 생각
	if (userPos != ranking.size() - 1)
	{
		float angleBackwards = CheckAngle(ranking[userPos + 1], 0.015f, user->GetAngle() * -1.0f);

		if (angleBackwards < M_PI / 4.0f * 0.004f)
		{
			float prob = StrategyHighest() * ScaleProbability(1.0f - std::abs(angleBackwards / (float)M_PI), 0.3f);

			return std::make_pair(prob, false);
		}
	}

	// 일등이면 앞으로 등껍질을 던질 확률을 계산할 필요는 없다.
	if (userPos == 0)
	{
		float prob = StrategyLowest();

		return std::make_pair(prob, false);
	}

	float angleForwards = CheckAngle(ranking[userPos - 1], 0.025f, user->GetAngle());

	if (angleForwards > M_PI)
	{
		float prob = StrategyLowest();

		return std::make_pair(prob, false);
	}

	float prob = StrategyHighest() * ScaleProbability(1.0f - std::abs(angleForwards / (float)M_PI), 0.55f);

	return std::make_pair(prob, true);
}

std::pair<float, bool> StrategyUseWhenFarFromNextInRanking(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	int userPos = user->GetRank();

	// 일등이라면 빨간 등껍질은 쓸 필요가 없다.
	if (userPos == 0)
	{
		float prob = StrategyLowest();

		return std::make_pair(prob, false);
	}

	// 마지막 랩
	if (user->GetLaps() == MAX_LAPS)
	{
		float prob = StrategyHighest();

		return std::make_pair(prob, true);
	}

	const CDriver* target = ranking[userPos - 1];

	Vector2 distance = target->GetPos() - user->GetPos();

	float modDiff = fminf(MAX_DIFF, sqrtf(fmaxf(1e-12f, distance.x * distance.x + distance.y * distance.y)));

	float prob = StrategyHighest() * ScaleProbability(modDiff / MAX_DIFF);

	return std::make_pair(prob, true);
}



CItem::CItem(const Vector2& pos, const float visualRadius, const float hitboxRadius, const float height, CRaceScene* scene, const std::string& name)
	: CWallObject(pos, visualRadius, hitboxRadius, height, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, scene, name)
{
	m_Used = false;
}

void CItem::UseItem(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking, const bool isFront, CRaceScene* scene, CRaceGUI* gui, const bool forceToUse)
{	
	m_StaticScene = scene;

	EItem powerUp = user->GetItem();

	if (powerUp == EItem::NONE ||
		(!forceToUse && (!user->CanUseItem() || (user->m_ControlType == EDriverControlType::PLAYER && !gui->CanUseItem()) ||
			!user->CanDrive() || user->m_OnLakitu)))
		return;

	CDriver* player = nullptr;

	for (CDriver* driver : ranking)
	{
		if (driver->m_ControlType == EDriverControlType::PLAYER)
		{
			player = driver;
			break;
		}
	}

	switch (powerUp)
	{
	case EItem::MUSHROOM:

		if (user->m_ControlType == EDriverControlType::PLAYER)
		{
			m_StaticScene->GetSceneResoruce()->SoundPlay("Mushroom");
		}

		user->ApplyMushroom();
		break;
	case EItem::COIN:
		user->AddCoin(2);
		break;
	case EItem::STAR:
		user->ApplyStar();
		break;
	case EItem::BANANA:
		if (user->m_ControlType == EDriverControlType::PLAYER)
		{
			if (isFront)
			{
				// 전방 아이템 사용 소리 재생
				m_StaticScene->GetSceneResoruce()->SoundPlay("Throw");
			}
			else
			{
				// 후방 아이템 사용 소리 재생
				m_StaticScene->GetSceneResoruce()->SoundPlay("UseItem");
			}
		}
		else if (player && Distance(user->GetPos() * 1024, player->GetPos() * 1024) <= 5.0f)
		{
			m_StaticScene->GetSceneResoruce()->SoundPlay("CpuItem");
		}

		m_StaticScene->AddItem(new CBanana(user->GetPos(), user->GetAngle(), isFront, user->GetHeight(), m_StaticScene, "Banana"));

		break;
	case EItem::GREEN_SHELL:
		if (user->m_ControlType == EDriverControlType::PLAYER)
		{
			m_StaticScene->GetSceneResoruce()->SoundPlay("UseItem");
		}

		m_StaticScene->AddItem(new CGreenShell(user->GetPos(), user->GetAngle(), isFront, user->GetHeight(), m_StaticScene, "GreenShell"));

		break;
	case EItem::RED_SHELL:
	{
		if (user->m_ControlType == EDriverControlType::PLAYER)
		{
			m_StaticScene->GetSceneResoruce()->SoundPlay("UseItem");
		}

		bool found = false;

		for (size_t i = 1; i < ranking.size(); i++)
		{
			if (ranking[i] == user)
			{
				m_StaticScene->AddItem(new CRedShell(user->GetPos(), ranking[i - 1], user->GetAngle(), isFront, user->GetHeight(),
					m_StaticScene, "RedShell"));
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_StaticScene->AddItem(new CRedShell(user->GetPos(), nullptr, user->GetAngle(), isFront, user->GetHeight(),
				m_StaticScene, "RedShell"));
		}
	}

		break;
	case EItem::THUNDER:
	{
		float duration = THUNDER_INIT_DURATION;

		bool apply = false;

		for (auto rit = ranking.rbegin(); rit != ranking.rend(); rit++)
		{
			auto driver = *rit;

			if (!apply && driver == user)
				apply = true;
			else if (apply && !driver->IsImmune())
			{
				driver->ApplyThunder(duration);
				duration += THUNDER_INCREMENT_DURATION;
			}
		}

		m_StaticScene->GetSceneResoruce()->SoundPlay("Thunder");
		gui->ThunderEffect();
	}
		break;

	case EItem::FEATHER:
	{
		if (user->m_ControlType == EDriverControlType::PLAYER)
		{
			m_StaticScene->GetSceneResoruce()->SoundPlay("Feather");
			user->ApplyFeather();
		}
	}
	break;
	}

	user->PickUpItem(EItem::NONE);

	if (user->m_ControlType == EDriverControlType::PLAYER)
	{
		gui->SetItem(EItem::NONE);
	}
}


std::pair<float, bool> CItem::GetItemUseProb(const CSharedPtr<CDriver>& user, const std::array<CDriver*, (int)EMenuPlayer::END>& ranking)
{
	switch (user->GetItem())
	{
	case EItem::NONE:
		return std::make_pair(-1.0f, false);
	case EItem::BANANA:
		return StrategyBanana(user, ranking);
	case EItem::COIN:
		return StrategyLessThanTenCoins(user, ranking);
	case EItem::GREEN_SHELL:
		return StrategyUserInFront(user, ranking);
	case EItem::MUSHROOM:
		return StrategySlowOrStraightLine(user, ranking);
	case EItem::RED_SHELL:
		return StrategyUseWhenFarFromNextInRanking(user, ranking);
	case EItem::STAR:
		return StrategySlowOrStraightLine(user, ranking);
	case EItem::THUNDER:
		return StrategyBetterWhenLast(user, ranking);
	}
}
