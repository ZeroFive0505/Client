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

	// ��� ���׸����� �����ϴ� �ϳ��� ��� ����
	class CMaterialConstantBuffer* m_CBuffer;

public:
	bool Init();
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);

public:
	// ��� Ÿ���� ���׸��� ���� �����ϰ� ��
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

	// ���׸����� Ŭ���� �̿��ؼ� �������� ����ִ�. ���� �ε�ÿ��� �� ���׸����� �����ϰ� �ε��ϴ� �������Ѵ�.
	template <typename T>
	T* CreateMaterialEmpty()
	{
		T* material = new T;

		material->SetConstantBuffer(m_CBuffer);

		return material;
	}
};

