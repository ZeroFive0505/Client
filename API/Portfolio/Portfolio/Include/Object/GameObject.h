#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"
#include "../Animation/Animation.h"

class CGameObject :
	public CRef
{
	friend class CScene;

public:
	CGameObject();
	CGameObject(const Vector2& position, const float visualRadius, const float hitBoxRadius, const float height, const int mapWidth,
		const int mapHeight);
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	class CScene* m_Scene;
	Vector2 m_PrevPos;
	Vector2 m_Pos;
	Vector2 m_Size;
	Vector2 m_Pivot;
	Vector2 m_Offset;

	float m_Angle;

	Vector2 m_ImageStart;
	CSharedPtr<CTexture> m_Texture;

	bool m_Physics;
	float m_GravityAccel;
	float m_JumpVelocity;

	float m_PivotY;

	float m_MoveSpeed;

	bool m_Start;

public:
	inline void SetGravityAccel(float accel)
	{
		m_GravityAccel = accel;
	}

	inline void SetPhysics(bool val)
	{
		m_Physics = val;
	}

	inline void SetJumpVelocity(float val)
	{
		m_JumpVelocity = val;
	}

	inline Vector2 GetOffset() const
	{
		return m_Offset;
	}

	inline Vector2 GetPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetSize() const
	{
		return m_Size;
	}

	inline float GetAngle() const
	{
		return m_Angle;
	}

	inline CSharedPtr<CTexture> GetTexture() const
	{
		return m_Texture;
	}

	inline Vector2 GetPivot() const
	{
		return m_Pivot;
	}

	inline float GetPivotY() const
	{
		return m_PivotY;
	}

	inline float GetMoveSpeed() const
	{
		return m_MoveSpeed;
	}

	float GetBottom() const;

public:
	void SetScene(class CScene* scene);

	inline void SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	inline void SetOffset(const Vector2& offset)
	{
		m_Offset = offset;
	}

	inline void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	inline void SetSize(const Vector2& size)
	{
		m_Size = size;
	}

	inline void SetAngle(const float angle)
	{
		m_Angle = angle;
	}

	inline void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}

	inline void SetPivot(const Vector2& pivot)
	{
		m_Pivot = pivot;
	}

	inline void SetPos(const Vector2& pos)
	{
		m_PivotY = pos.y;
		m_Pos = pos;
	}

	inline void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	inline void SetMoveSpeed(float speed)
	{
		m_MoveSpeed = speed;
	}

	void Move(const Vector2& dir);
	void Move(const Vector2& dir, float speed);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(HDC hDC);
	virtual CGameObject* Clone();

public:
	void SetTexture(CTexture* texture)
	{
		m_Texture = texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}

	void SetTexture(const std::string& name);
	void SetTexture(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& name, const std::vector<std::wstring>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int index = 0);

	
protected:
	CAnimation* m_Animation;

	
public:
	void CreateAnimation();
	void AddAnimation(const std::string& sequenceName, bool loop = true, float playTime = 1.0f, float playScale = 1.0f,
		bool reverse = false);
	void SetAnimationPlayTime(const std::string& name, float playTime);
	void SetAnimationPlayScale(const std::string& name, float playScale);
	void SetCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	bool CheckCurrentAnimation(const std::string& name);
	void SetAnimationReverse(const std::string& name, bool reverse);
	void SetAnimationLoop(const std::string& name, bool loop);
	
public:
	template <typename T>
	void SetAnimationEndNotify(const std::string& sequneceName, T* obj, void(T::* func)())
	{
		m_Animation->SetEndNotify(sequneceName, obj, func);
	}

	template <typename T>
	void AddAnimationNotify(const std::string& sequenceName, int frame, T* obj, void(T::* func)())
	{
		m_Animation->AddNotify(sequenceName, frame, obj, func);
	}
};

