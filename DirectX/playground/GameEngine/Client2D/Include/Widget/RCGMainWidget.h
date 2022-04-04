#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"
#include "GameObject/GameObject.h"
#include "../Object/RCGPlayer.h"
#include "../ClientManager.h"
#include "../RCGGameManager.h"
#include "Widget/Text.h"

class CRCGMainWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CRCGMainWidget();
	CRCGMainWidget(const CRCGMainWidget& window);
	virtual ~CRCGMainWidget();

private:
	CSharedPtr<CImage> m_Border;
	CSharedPtr<CImage> m_FadeOut;
	CSharedPtr<CImage> m_PlayerHUD;
	CSharedPtr<CImage> m_CharacterIcon;
	CSharedPtr<CProgressBar> m_SpecialBar;
	CSharedPtr<CImage> m_SpecialBarBack;
	std::vector<CSharedPtr<CImage>> m_HPBar;
	CSharedPtr<CImage> m_BattleIntro;
	std::function<void()> m_SceneChangeCallback;

	float m_StartTime;
	float m_TargetValue;
	float m_LerpTime;
	bool m_LerpStart;

	bool m_FadeOutStart;
	bool m_FadeOutComplete;
	float m_FadeOutSpeed;
	float m_FadeOutTime;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();

public:
	inline void SetEmpty()
	{
		size_t size = m_HPBar.size();

		for (size_t i = 0; i < size; i++)
		{
			m_HPBar[i]->SetOpacity(0.0f);
		}
	}

public:
	void FadeOutStart();

	void SavePlayerInfo();

	inline void BattleIntroStart()
	{
		m_BattleIntro->SetAnimStart(true);
	}

	inline void BattleIntroPause()
	{
		m_BattleIntro->SetAnimStart(false);
	}

public:
	inline void SetSpecialPercent(float percent)
	{
		m_LerpStart = true;
		m_LerpTime = 0.0f;
		m_TargetValue = percent;
	}

	inline void FullHPBar(int index)
	{
		m_HPBar[index]->SetOpacity(1.0f);
	}

	inline void SetHPOpcity(int index, float opacity)
	{
		m_HPBar[index]->SetOpacity(opacity);
	}

public:
	inline float GetSpecialPercent() const
	{
		return m_SpecialBar->GetPercent();
	}

	inline bool IsFadingOut() const
	{
		return m_FadeOutStart;
	}

	inline bool BattleIntroEnd() const
	{
		return !m_BattleIntro->IsLooping();
	}

	inline void EnableBattleIntro()
	{
		m_BattleIntro->SetRender(true);
	}

	inline void DisableBattleIntro()
	{
		m_BattleIntro->SetRender(false);
	}

public:
	template <typename T>
	void SetSceneChangeCallback(T* obj, void(T::* func)())
	{
		m_SceneChangeCallback = std::bind(func, obj);
	}
};

