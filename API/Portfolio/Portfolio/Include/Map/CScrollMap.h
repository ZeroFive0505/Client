#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"

class CScrollMap :
	public CMapBase
{
	friend class CScene;

 public:
	CScrollMap();
	virtual ~CScrollMap();

protected:
	CSharedPtr<CTexture> m_ScrollTexture;
	Vector2 m_TextureSize;
	Vector2 m_ScrollRatio;
	bool m_Loop;

public:
	void SetScrollRatio(float x, float y)
	{
		m_ScrollRatio = Vector2(x, y);
	}

	void SetLoop(bool loop)
	{
		m_Loop = loop;
	}

public:
	void SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
	void SetTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	void SetTexture(const std::string& name);
	void SetTextureColorKey(const unsigned char r, const unsigned char g, const unsigned char b);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

