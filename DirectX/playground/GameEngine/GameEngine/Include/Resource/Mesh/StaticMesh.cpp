#include "StaticMesh.h"
#include "FBXLoader.h"

CStaticMesh::CStaticMesh()
{
	SetTypeID<CStaticMesh>();
	m_MeshType = Mesh_Type::Static;
}

CStaticMesh::~CStaticMesh()
{
}

bool CStaticMesh::Init()
{
	return true;
}

bool CStaticMesh::LoadMeshFullPathMultibyte(const char* fullPath)
{
	char ext[_MAX_EXT] = {};

	_splitpath_s(fullPath, 0, 0, 0, 0, 0, 0, ext, _MAX_EXT);
	_strupr_s(ext);

	// 만약 FBX파일일 경우 FBXLoader를 이용한다.
	if (strcmp(ext, ".FBX") == 0)
	{
		CFBXLoader loader;

		if (!loader.LoadFBX(fullPath))
			return false;

		return ConvertFBX(&loader, fullPath);
	}

	return LoadMeshFile(fullPath);
}

bool CStaticMesh::ConvertFBX(CFBXLoader* loader, const char* fullPath)
{
	return CMesh::ConvertFBX(loader, fullPath);
}

bool CStaticMesh::SaveMesh(FILE* pFile)
{
	if (!CMesh::SaveMesh(pFile))
		return false;

	return true;
}

bool CStaticMesh::LoadMesh(FILE* pFile)
{
	if (!CMesh::LoadMesh(pFile))
		return false;

	return true;
}
