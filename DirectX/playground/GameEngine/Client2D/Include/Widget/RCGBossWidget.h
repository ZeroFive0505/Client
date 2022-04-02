#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"
#include "../Object/RCGPlayer.h"
#include "../ClientManager.h"
#include "../RCGGameManager.h"
#include "Widget/Text.h"

class CRCGBossWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CRCGBossWidget();
	CRCGBossWidget(const CRCGBossWidget& window);
	virtual ~CRCGBossWidget();

private:
	CSharedPtr<CImage> m_BossHealthBarBack;
	CSharedPtr<CProgressBar> m_BossHealth;

	bool m_FadeIn;
	float m_FadeTime;
	bool m_FadeOut;

	int m_MaxBossHealth;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();

public:
	inline void WidgetFadeInStart()
	{
		m_FadeIn = true;
		m_FadeTime = 0.0f;
	}

	inline void WidgetFadeOutStart()
	{
		m_FadeOut = true;
		m_FadeTime = 0.0f;
	}

	inline void SetBossHP(int hp)
	{
		m_MaxBossHealth = hp;
	}

	inline void SetBossHPMeter(float percent)
	{
		m_BossHealth->SetPercent(percent);
	}
};

