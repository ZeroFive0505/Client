#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"
#include "Component/WidgetComponent.h"
#include "../Widget/RCGMainWidget.h"

class CRCGDoor :
	public CGameObject
{
	friend class CScene;

protected:
	CRCGDoor();
	CRCGDoor(const CRCGDoor& obj);
	virtual ~CRCGDoor();

private:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CWidgetComponent> m_WidgetComponent;

	class CRCGDoorHUD* m_DoorHUD;
	class CRCGMainWidget* m_Widget;

	float m_Progress;

public:
	inline void SetWidget(class CRCGMainWidget* widget)
	{
		m_Widget = widget;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CRCGDoor* Clone();
};

