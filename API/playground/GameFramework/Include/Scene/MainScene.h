#pragma once
#include "Scene.h"
/// <summary>
/// ���� ���� ���� ��ӹ޴´�.
/// ���⼭ �ʿ��� ���ҽ��� �ε��� �Ѵ�.
/// </summary>
class CMainScene :
    public CScene
{
	// �� �����ڴ� �̷��� ������ ������ �� �־���Ѵ�. ���� ������
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

