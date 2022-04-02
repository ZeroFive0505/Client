#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"
#include "../Animation//Animation.h"
#include "../Collision//Collider.h"
#include "../UI/WidgetComponent.h"

/// <summary>
/// 씬에 생성되는 여러 오브젝트의 가장 기본이 되는 클래스이다.
/// CRef클래스를 상속받으므로 레퍼런스 카운팅이 가능하다.
/// </summary>

class CGameObject :
	public CRef
{
	friend class CScene;

public:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	// 현재 어떤 씬에 속해있는지
	class CScene* m_Scene;
	// 전의 위치
	Vector2		m_PrevPos;
	// 렌더 위치
	Vector2 m_RenderPos;
	// 현재 위치
	Vector2		m_Pos;
	// 크기
	Vector2		m_Size;
	// 중심점
	Vector2		m_Pivot;
	// 속도
	Vector2		m_Velocity;
	// 텍스쳐 오프셋
	Vector2 m_Offset;

	// 이동속도
	float		m_MoveSpeed;
	// 이 오브젝트의 시간 배율
	float		m_TimeScale;
	// 가지고 있는 텍스쳐
	CSharedPtr<CTexture>	m_Texture;
	// 텍스쳐의 시작위치
	Vector2		m_ImageStart;
	std::list<CSharedPtr<CCollider>> m_ColliderList;
	// 카메라에서 짤리는지
	bool m_CameraCull;

	bool m_Start;

	EObject_Type m_ObjType;
	bool m_DamageEnable;

	// 가지고있는 위젯 컴포넌트들.
	std::list<CSharedPtr<CWidgetComponent>> m_WidgetComponentList;

	bool m_PhysicsSimulate;
	float m_GravityAccel;
	bool m_IsGround;
	float m_FallTime;
	float m_FallStartY;
	bool m_Jump;
	float m_JumpVelocity;

	float m_LifeTime;

	bool m_FloorCheck;

	bool m_SideWallCheck;

public:
	void SetSideWallCheck(bool Check)
	{
		m_SideWallCheck = Check;
	}

	void SetFloorCheck(bool Check)
	{
		m_FloorCheck = Check;
	}

	void SetLifeTime(float Time)
	{
		m_LifeTime = Time;
	}

	void SetGravityAccel(float Accel)
	{
		m_GravityAccel = Accel;
	}

	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}

	void SetJumpVelocity(float JumpVelocity)
	{
		m_JumpVelocity = JumpVelocity;
	}

	void Jump()
	{
		if (!m_Jump)
		{
			m_Jump = true;
			m_IsGround = false;

			m_FallTime = 0.0f;
			m_FallStartY = m_Pos.y;
		}
	}

	// 데미지를 줄 수 있는 설정
	void DamageEnable(bool Enable)
	{
		m_DamageEnable = Enable;
	}

	EObject_Type GetObjType() const
	{
		return m_ObjType;
	}

	bool IsCull() const
	{
		return m_CameraCull;
	}

public:
	CCollider* FindCollider(const std::string& Name);

protected:
	// 오브젝트의 애니메이션
	CAnimation* m_Animation;

public:
	// 게임 오브젝트 애니메이션 관련 유틸리티 함수
	void CreateAnimation();
	void AddAnimation(const std::string& SequenceName, bool Loop = true, float PlayTime = 1.0f, float PlayScale = 1.0f,
		bool Reverse = false);
	void SetAniamtionPlayTime(const std::string& Name, float PlayTime);
	void SetAnimationPlayScale(const std::string& Name, float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name, bool Reverse);
	void SetAnimationLoop(const std::string& Name, bool Loop);

public:
	Vector2 GetOffset() const
	{
		return  m_Offset;
	}

	Vector2 GetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetSize()	const
	{
		return m_Size;
	}

	Vector2 GetPivot()	const
	{
		return m_Pivot;
	}

	Vector2 GetVelocity()	const
	{
		return m_Velocity;
	}

	float GetMoveSpeed()	const
	{
		return m_MoveSpeed;
	}

	float GetMoveSpeedFrame()	const
	{
		return m_Velocity.Length();
	}

	float GetBottom() const;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	void SetOffset(const Vector2& Offset)
	{
		m_Offset = Offset;
	}

	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}

	void SetPivot(const Vector2& Pivot)
	{
		m_Pivot = Pivot;
	}

	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}

	void Move(const Vector2& Dir);
	void Move(const Vector2& Dir, float Speed);

	void SetImageStart(float x, float y)
	{
		m_ImageStart.x = x;
		m_ImageStart.y = y;
	}

	void SetImageStart(const Vector2& ImageStart)
	{
		m_ImageStart = ImageStart;
	}

public:
	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}

	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextrueFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r,
		unsigned char g, unsigned char b, int Index = 0);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CGameObject* Clone();
	virtual float SetDamage(float Damage);
	virtual float SetMP(float MP);

public:
	template <typename T>
	void SetAnimationEndNotify(const std::string& SequenceName, T* Obj, void(T::* Func)())
	{
		m_Animation->SetEndNotify(SequenceName, Obj, Func);
	}

	template <typename T>
	void AddAnimationNotify(const std::string& SequenceName, int Frame, T* Obj, void(T::* Func)())
	{
		m_Animation->AddNotify(SequenceName, Frame, Obj, Func);
	}
	
public:
	template <typename T>
	T* AddCollider(const std::string& Name)
	{
		T* Collider = new T;

		Collider->SetName(Name);
		Collider->SetOwner(this);
		Collider->SetScene(m_Scene);

		if (!Collider->Init())
		{
			SAFE_DELETE(Collider);
			return nullptr;
		}

		m_ColliderList.push_back(Collider);

		return Collider;
	}

	CWidgetComponent* FindWidgetComponent(const std::string& Name)
	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}

		return nullptr;
	}

	CWidgetComponent* CreateWidgetComponent(const std::string& Name)
	{
		CWidgetComponent* Widget = FindWidgetComponent(Name);

		if (Widget)
			return nullptr;

		Widget = new CWidgetComponent;

		Widget->SetName(Name);
		Widget->SetOwner(this);
		Widget->SetScene(m_Scene);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_WidgetComponentList.push_back(Widget);

		return Widget;
	}
};

