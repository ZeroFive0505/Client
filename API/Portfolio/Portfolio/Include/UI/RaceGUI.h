#pragma once

#include "UIWindow.h"
#include "../Object/ObjectInfo.h"

const float GUI_FADEOUT_TIME = 1.5f;
const float GUI_FADEIN_TIME = 0.5f;

class CRaceGUI :
	public CUIWindow
{
	friend class CScene;

private:
	CRaceGUI();
	virtual ~CRaceGUI();

private:
	class CCoinWidget* m_CoinWidget;

	class CItemWidget* m_ItemWidget;

	class CRankWidget* m_RankWidget;

	std::vector<class CUISpriteText*> m_FinishTimes;

	bool m_Thunder;
	bool m_BlackScreen;
	bool m_EffectEnd;
	bool m_FadeOut;
	bool m_FadeIn;
	bool m_RaceFinished;
	float m_BlackScreenTime;

	int m_ThunderCnt;

	Vector2 m_LT;
	Vector2 m_RB;

public:
	void AddCoin(int amount);
	void SetItem(EItem item);
	void SetRank(int rank);
	bool CanUseItem();

	inline void ThunderEffect()
	{
		m_Thunder = true;
		m_ThunderCnt = 0;
	}
	
	void EffectStart();

	inline bool IsBlackScreen() const
	{
		return m_BlackScreen;
	}

	inline bool HasEffectEnd() const
	{
		return m_EffectEnd;
	}

	inline void RaceFinished()
	{
		m_RaceFinished = true;
	}

	inline bool IsFadeOutFinish() const
	{
		return m_FadeOut;
	}

	inline void ResetEffectEnd()
	{
		m_EffectEnd = false;
	}

	void ForceStop();

	void AddFinishTime(EMenuPlayer character, float time);

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hDC) override;
};

