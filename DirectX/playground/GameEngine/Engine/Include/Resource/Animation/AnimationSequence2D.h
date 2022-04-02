#pragma once

#include "../../Ref.h"

// �ϳ��� �ؽ��ĸ� ������
// �׾ȿ� ������ġ�� �����ؼ� ���ϴ� �ؽ��ĸ� ����Ѵ�.
class CAnimationSequence2D :
	public CRef
{
	friend class CAnimationManager;


protected:
	CAnimationSequence2D();
	~CAnimationSequence2D();

private:
	class CScene* m_Scene;
	CSharedPtr<class CTexture> m_Texture;
	std::vector<sAnimationFrameData> m_vecFrameData;

public:
	inline void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

public:
	inline class CTexture* GetTexture() const
	{
		return m_Texture;
	}

	inline const sAnimationFrameData& GetFrameData(int index) const
	{
		return m_vecFrameData[index];
	}

	inline int GetFrameCount() const
	{
		return (int)m_vecFrameData.size();
	}

	void SetTexture(class CTexture* texture);

public:
	// �ؽ��ĸ� ���� �����ؼ� �������� �����.
	bool Init(class CTexture* texture);
	// �ؽ��� �̸��� ���� �̸� ��θ� �����ؼ� �����.
	bool Init(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool Init(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool InitFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);
	void AddFrame(const Vector2& startPos, const Vector2& size);
	void AddFrame(float startX, float startY, float width, float height);
	void SetFrameData(int index, const sAnimationFrameData& data);
	void DeleteFrame(int index);
	void ClearFrame();

public:
	bool SaveFullPath(const char* fullPath);
	bool LoadFullPath(const char* fullPath);

	bool Save(const char* fileName, const std::string& pathName);
	bool Load(const char* fileName, const std::string& pathName);
};

