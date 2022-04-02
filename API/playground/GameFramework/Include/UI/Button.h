#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"
#include "../Resource/Sound.h"

/// <summary>
/// ������ ��ư ������ ����Ѵ�.
/// </summary>
class CButton :
	public CUIWidget
{
public:
	CButton();
	CButton(const CButton& Widget);
	virtual ~CButton();

protected:
	// ��ư �ؽ��� 
	CSharedPtr<CTexture> m_Texture;
	// ��ư ���¿� ���� �ִϸ��̼�
	AnimationFrameData m_FrameData[(int)EButton_State::End];
	// ��ư�� ����
	EButton_State m_ButtonState;
	// ��ư Ŭ���� ȣ��� �ݹ� �Լ�
	std::function<void()> m_ButtonClickCallback;
	std::function<void()> m_ButtonMouseOnCallback;

	// ��ư ����
	CSharedPtr<CSound> m_ButtonSound[2];

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
	void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index = 0);
	void SetFrameData(EButton_State State, const Vector2& Start, const Vector2& Size);
	
	void Enable(bool Enable)
	{
		m_ButtonState = Enable ? EButton_State::Normal : EButton_State::Disable;
	}

	void SetMouseOnSound(const std::string& Name);
	void SetClickSound(const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CButton* Clone();

protected:
	virtual void CollisionMouseHoveredCallback(float DeltaTime);
	virtual void CollisionMouseReleaseCallback(float DeltaTime);

public:
	template <typename T>
	void SetClickCallback(T* Obj, void(T::* Func)())
	{
		m_ButtonClickCallback = std::bind(Func, Obj);
	}

	template <typename T>
	void SetMouseOnCallback(T* Obj, void(T::* Func)())
	{
		m_ButtonMouseOnCallback = std::bind(Func, Obj);
	}
};

