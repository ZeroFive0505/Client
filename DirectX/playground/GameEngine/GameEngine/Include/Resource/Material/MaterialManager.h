#pragma once

#include "../../GameInfo.h"
#include "Material.h"

class CMaterialManager
{
	friend class CResourceManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	std::unordered_map<std::string, CSharedPtr<CMaterial>> m_mapMaterial;

	// 모든 마테리얼이 공유하는 하나의 상수 버퍼
	class CMaterialConstantBuffer* m_CBuffer;

public:
	bool Init();
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);

public:
	// 어떠한 타입의 마테리얼도 생성 가능하게 함
	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		T* material = (T*)FindMaterial(name);

		if (material)
			return false;

		material = new T;

		material->SetConstantBuffer(m_CBuffer);

		material->SetName(name);

		m_mapMaterial.insert(std::make_pair(name, material));

		return true;
	}

	// 마테리얼은 클론을 이용해서 복제본을 들고있다. 따라서 로드시에는 빈 마테리얼을 전달하고 로드하는 식으로한다.
	template <typename T>
	T* CreateMaterialEmpty()
	{
		T* material = new T;

		material->SetConstantBuffer(m_CBuffer);

		return material;
	}
};

