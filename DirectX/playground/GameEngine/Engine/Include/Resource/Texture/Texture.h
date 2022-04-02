#pragma once

#include "../../Ref.h"

struct sTextureResourceInfo
{
	// 실제 픽셀 정보를 담고있는 배열
	ScratchImage* Image;
	// 쉐이더를 통해서 텍스쳐를 이용할때 사용함
	ID3D11ShaderResourceView* SRV;
	unsigned int width;
	unsigned int height;
	TCHAR* fileName;
	char* pathName;
	TCHAR* fullPath;

	sTextureResourceInfo() :
		Image(nullptr),
		SRV(nullptr),
		fileName(nullptr),
		pathName(nullptr),
		fullPath(nullptr),
		width(0),
		height(0)
	{

	}

	~sTextureResourceInfo()
	{
		SAFE_RELEASE(SRV);
		SAFE_DELETE_ARRAY(fileName);
		SAFE_DELETE_ARRAY(pathName);
		SAFE_DELETE_ARRAY(fullPath);
		SAFE_DELETE(Image);
	}
};


class CTexture :
	public CRef
{
	friend class CTextureManager;

protected:
	CTexture();
	virtual ~CTexture();

protected:
	class CScene* m_Scene;
	std::vector<sTextureResourceInfo*> m_vecTextureInfo;
	Image_Type m_ImageType;

public:
	inline ID3D11ShaderResourceView* GetResource(int index = 0) const
	{
		return m_vecTextureInfo[index]->SRV;
	}

	inline Image_Type GetImageType() const
	{
		return m_ImageType;
	}

	inline unsigned int GetWidth(int index = 0) const
	{
		return m_vecTextureInfo[index]->width;
	}

	inline unsigned int GetHeight(int index = 0) const
	{
		return m_vecTextureInfo[index]->height;
	}

	inline size_t GetImageCount() const
	{
		return m_vecTextureInfo.size();
	}

public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);

	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);

	bool LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);

private:
	bool CreateResource(int index);

public:
	void SetShader(int registerNum, int shaderType, int index);
	void ResetShader(int registerNum, int shaderType, int index);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

