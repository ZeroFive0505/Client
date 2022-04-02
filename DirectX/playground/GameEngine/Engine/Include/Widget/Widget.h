#pragma once

#include "../Ref.h"
#include "../Resource/Texture/Texture.h"

struct sWidgetImageInfo
{
	CSharedPtr<CTexture> texture;
	Vector4 tint;
	std::vector<sAnimationFrameData> vecFrameData;

	int frame;
	float elapsedTime;
	float frameTime;
	float playTime;
	float playScale;

	sWidgetImageInfo()
	{
		tint = Vector4::White;
		frame = 0;
		elapsedTime = 0.0f;
		frameTime = 0.0f;
		playTime = 1.0f;
		playScale = 1.0f;
	}
};


/// <summary>
/// 윈도우에 들어가는 위젯의 기본 클래스 이 클래스를 상속받아 다양한 위젯을 만든다.
/// </summary>
class CWidget :
	public CRef
{
	friend class CWidgetWindow;

protected:
	CWidget();
	CWidget(const CWidget& widget);
	virtual ~CWidget();

protected:
	class CWidgetWindow* m_Owner;
	int m_ZOrder;
	Vector2 m_Pos;
	Vector2 m_RenderPos;
	Vector2 m_Size;
	float m_Angle;
	bool m_Start;
	Vector4 m_Tint;
	float m_Opacity;
	bool m_MouseHovered;
	bool m_MouseCollisionEnable;
	class CWidgetConstantBuffer* m_CBuffer;
	CSharedPtr<class CShader> m_Shader;
	CSharedPtr<class CMesh> m_Mesh;

public:
	virtual void Enable(bool enable)
	{
		CRef::Enable(enable);
	}

	inline class CWidgetWindow* GetOwner() const
	{
		return m_Owner;
	}

	inline Vector2 GetWindowPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetWindowSize() const
	{
		return m_Size;
	}

	inline int GetZOrder() const
	{
		return m_ZOrder;
	}

	inline float GetAngle() const
	{
		return m_Angle;
	}

	inline float GetOpacity() const
	{
		return m_Opacity;
	}

public:
	inline void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	inline void SetPos(const Vector2& pos)
	{
		m_Pos = pos;
	}

	inline void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	inline virtual void SetSize(const Vector2& size)
	{
		m_Size = size;
	}

	inline virtual void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

	inline void SetOwner(class CWidgetWindow* owner)
	{
		m_Owner = owner;
	}

	inline void SetAngle(float angle)
	{
		m_Angle = angle;
	}

	inline void SetOpacity(float opacity)
	{
		m_Opacity = opacity;
	}

	inline void SetCollisionEnable(bool enable)
	{
		m_MouseCollisionEnable = enable;
	}

	
	void SetShader(const std::string& name);
	void SetUseTexture(bool use);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual bool CollisionMouse(const Vector2& mousePos);
	virtual CWidget* Clone();
};

