#pragma once

#include "../GameInfo.h"
#include "Scene.h"
/// <summary>
/// 씬을 관리하는 관리자 클래스이다.
/// 씬 매니저는 게임 매니저 초기화시에 같이 초기화되고 소멸시에도 같이 소멸된다.
/// 현재 씬은 이 씬 매니저가 들고있다.
/// </summary>
class CSceneManager
{
private:
	CSceneManager();
	~CSceneManager();

private:
	CScene* m_Scene;
	CScene* m_NextScene;

public:
	// 현재 씬을 반환한다.
	CScene* GetScene()
	{
		return m_Scene;
	}

public:
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	bool Render(HDC hDC);

private:
	bool ChangeScene();

private:
	static CSceneManager* m_Inst;

public:
	static CSceneManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CSceneManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}

public:
	/// <summary>
	/// T타입의 씬을 만든다.
	/// </summary>
	/// <typeparam name="T">만들고 싶은 타입의 씬</typeparam>
	/// <returns>성공 여부 반환</returns>
	template <typename T>
	bool CreateScene()
	{
		m_NextScene = new T;

		if (!m_NextScene->Init())
		{
			SAFE_DELETE(m_NextScene);
			return false;
		}

		return true;
	}
};

