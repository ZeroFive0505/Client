#pragma once

#include "UIWindow.h"
#include "../Resource/Texture.h"

class CCourseSelectUI :
	public CUIWindow
{
	friend class CScene;

private:
	CCourseSelectUI();
	virtual ~CCourseSelectUI();

private:
	enum class EIcon
	{
		Mushroom,
		Flower,
		Star,
		Special
	};

private:
	class CUIImage* m_BackGround;
	
	class CUIImage* m_MushroomCup;
	class CUIImage* m_FlowerCup;
	class CUIImage* m_StarCup;
	class CUIImage* m_SpecialCup;

	class CUIImage* m_MushroomCupHover;
	class CUIImage* m_FlowerCupHover;
	class CUIImage* m_StarCupHover;
	class CUIImage* m_SpecialCupHover;

	CSharedPtr<CTexture> m_MushroomIcon;
	CSharedPtr<CTexture> m_FlowerIcon;
	CSharedPtr<CTexture> m_StarIcon;

	class CUISpriteText* m_CC;

	EIcon m_CurrentIcon;

	std::vector<Vector2> m_IconPosition;
	std::vector<std::pair<int, Vector2>> m_Icons;

public:
	virtual bool Init() override;
	virtual void Render(HDC hDC) override;
	virtual void Update(float deltaTime) override;

public:
	void SetCCText(ECCOption option);

	void SetMushroomCup();
	void SetFlowerCup();
	void SetStarCup();
	void SetSpecialCup();
};

