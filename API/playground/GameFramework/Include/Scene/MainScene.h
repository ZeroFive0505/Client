#pragma once
#include "Scene.h"
/// <summary>
/// 메인 씬은 씬을 상속받는다.
/// 여기서 필요한 리소스도 로딩을 한다.
/// </summary>
class CMainScene :
    public CScene
{
	// 씬 관리자는 이러한 씬들을 생성할 수 있어야한다. 따라서 프렌드
	friend class CSceneManager;

private:
	CMainScene();
	virtual ~CMainScene();

public:
	virtual bool Init();
	virtual bool Render(HDC hDC);

private:
	void LoadAnimationSequence();
	void LoadSound();
};

