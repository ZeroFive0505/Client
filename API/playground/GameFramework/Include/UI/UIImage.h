#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"

/// <summary>
/// 위젯중 기본적 이미지를 출력을 담당하는 위젯이다.
/// </summary>
class CUIImage :
	public CUIWidget
{
public:
	CUIImage();
	CUIImage(const CUIImage& widget);
	virtual ~CUIImage();

protected:
	// 출력될 이미지
	CSharedPtr<CTexture> m_Texture;
	// 이미지의 애니메이션 정보
	std::vector<AnimationFrameData> m_vecFrameData;
	// 현재 재생되는 인덱스
	int m_FrameIndex;
	// 한 프레임당 재생시간
	float m_PlayTime;
	// 전체 애니메이션 시간
	float m_AnimTime;
	// 오프셋
	Vector2 m_Offset;

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
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index = 0);
	void AddFrameData(const Vector2& Pos, const Vector2& Size)
	{
		AnimationFrameData data = {};
		data.StartPos = Pos;
		data.Size = Size;

		m_vecFrameData.push_back(data);
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CUIImage* Clone();
};

