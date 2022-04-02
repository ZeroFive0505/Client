#pragma once

#include "../GameInfo.h"

/// <summary>
/// �浹 ���������� ���� �����ϴ� �Ŵ���(�̱���) Ŭ����
/// </summary>
class CCollisionManager
{
private:
	std::unordered_map<std::string, sCollisionProfile*> m_mapProfile;

private:
	inline void IgnoreAllButThis(const std::string& name, Collision_Channel channel)
	{
		sCollisionProfile* profile = FindProfile(name);

		if (!profile)
			return;

		for (int i = 0; i < (int)Collision_Channel::MAX; i++)
		{
			if (i != (int)channel)
				profile->vecInteraction[i] = Collision_Interaction::Ignore;
		}
	}

public:
	bool Init();
	bool CreateProfile(const std::string& name, Collision_Channel channel, bool enable,
		Collision_Interaction interaction = Collision_Interaction::Collision);
	bool SetCollisionState(const std::string& name, Collision_Channel channel, Collision_Interaction interaction);
	sCollisionProfile* FindProfile(const std::string& name);

	DECLARE_SINGLE(CCollisionManager)
};

