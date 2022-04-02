#pragma once

#include "Component.h"

/// <summary>
/// 게임 오브젝트의 속성 컴포넌트
/// 강체, 애니메이터등 씬에 직접적으로 렌더링 되지는 않지만
/// 게임 오브젝트의 속성을 부여하는 컴포넌트이다.
/// </summary>
class CObjectComponent :
	public CComponent
{
	friend class CGameObject;

protected:
	CObjectComponent();
	CObjectComponent(const CObjectComponent& com);
	virtual ~CObjectComponent() = 0;

public:
	virtual void Start();
	virtual bool Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void PostUpdate(float deltaTime) = 0;
	virtual void PrevRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual CObjectComponent* Clone() = 0;
};

