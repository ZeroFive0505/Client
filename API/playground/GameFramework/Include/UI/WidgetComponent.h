#pragma once

#include "../Ref.h"
#include "UIWidget.h"

/// <summary>
/// ���� ������Ʈ�� ���� �� �ִ� ���� ������Ʈ
/// �� ���� ������Ʈ�� ��ũ����ǥ�� �ƴ� ���� ��ǥ�� �̿��ؼ� ��µȴ�.
/// �� �������� ������� ��ġ�� ���� ��ġ�� ��� ���Ѵ�.
/// </summary>
class CWidgetComponent :
	public CRef
{
	friend class CGameObject;

private:
	CWidgetComponent();
	CWidgetComponent(const CWidgetComponent& widget);
	~CWidgetComponent();

private:
	// �� ���� �����ʴ�Ʈ�� ������ �ִ� ������
	class CGameObject* m_Owner;
	// ���� � ���� �Ҽӵ��ִ���
	class CScene* m_Scene;
	// ������ �ִ� ����
	CSharedPtr<CUIWidget> m_Widget;
	// ��ġ
	Vector2 m_Pos;
	bool m_Visibility;

public:
	Vector2 GetPos() const
	{
		return m_Pos;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetOwner(class CGameObject* Owner)
	{
		m_Owner = Owner;
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetVisibility(bool Visibilty)
	{
		m_Visibility = Visibilty;
	}

	bool GetVisibility() const
	{
		return m_Visibility;
	}

	void SetWidget(CUIWidget* Widget)
	{
		m_Widget;
	}

	CUIWidget* GetWidget() const
	{
		return m_Widget;
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(HDC hDC);
	CWidgetComponent* Clone();

public:
	/// <summary>
	/// ���� ���� ������Ʈ�� ���ο� ������ ���(�߰�) ��Ų��.
	/// </summary>
	/// <typeparam name="T">������Ű���� �ϴ� ������ Ÿ��</typeparam>
	/// <param name="Name">������ �̸�</param>
	/// <returns>������ ����</returns>
	template <typename T>
	T* CreateWidget(const std::string& Name)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->SetScene(m_Scene);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		// ���� ������ �ٲ۴�.
		m_Widget = Widget;

		return Widget;
	}
};

