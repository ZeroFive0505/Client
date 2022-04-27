#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"
#include "AnimationMesh.h"
#include "../../PathManager.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	// 가장 기본 메쉬로는 SpriteMesh를 만든다.
	CMesh* spriteMesh = new CSpriteMesh;
	spriteMesh->SetName("SpriteMesh");
	if (!spriteMesh->Init())
	{
		spriteMesh->Release();
		return false;
	}

	m_mapMesh.insert(std::make_pair("SpriteMesh", spriteMesh));

	// 에디터에서 사용이 될 와이어 프레임 메쉬를 만든다.
	CMesh* frameRectMesh = new CStaticMesh;
	frameRectMesh->SetName("FrameRect");
	Vector3 frameRectVertices[5] =
	{
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f)
	};

	frameRectMesh->CreateMesh(frameRectVertices, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	m_mapMesh.insert(std::make_pair("FrameRect", frameRectMesh));

	// 충돌체가 이용할 메쉬를 생성한다.
	CMesh* Box2DMesh = new CStaticMesh;

	// 가운데가 0, 0
	Vector3 Box2DPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.0f),
		Vector3(0.5f, 0.5f, 0.0f),
		Vector3(0.5f, -0.5f, 0.0f),
		Vector3(-0.5f, -0.5f, 0.0f),
		Vector3(-0.5f, 0.5f, 0.0f)
	};

	Box2DMesh->SetName("Box2D");
	Box2DMesh->CreateMesh(Box2DPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("Box2D", Box2DMesh));

	CMesh* circleMesh = new CStaticMesh;

	Vector3 circlePos[31];

	circlePos[0] = Vector3(0.5f, 0.0f, 0.0f);

	for (int i = 1; i < 31; i++)
	{
		circlePos[i].x = cosf(DegreeToRadian(i * 12.0f)) * 0.5f;
		circlePos[i].y = sinf(DegreeToRadian(i * 12.0f)) * 0.5f;
	}

	circleMesh->SetName("Circle");
	circleMesh->CreateMesh(circlePos, sizeof(Vector3), 31, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("Circle", circleMesh));

	CMesh* lineMesh = new CStaticMesh;

	Vector3 linePos[2];

	linePos[0] = Vector3(0.0f, 0.0f, 0.0f);
	linePos[1] = Vector3(1.0f, 0.0f, 0.0f);

	lineMesh->SetName("LineMesh");
	lineMesh->CreateMesh(linePos, sizeof(Vector3), 2, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_mapMesh.insert(std::make_pair("LineMesh", lineMesh));

	CMesh* widgetMesh = new CStaticMesh;

	sVertexUV widgetVertices[4] =
	{
		sVertexUV(Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)), // 0 
		sVertexUV(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)), // 1
		sVertexUV(Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)), // 2
		sVertexUV(Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)) // 3
	};

	int widgetIndices[6] = { 0, 1, 3, 0, 3, 2 };

	widgetMesh->SetName("WidgetMesh");
	widgetMesh->CreateMesh(widgetVertices, sizeof(sVertexUV), 4, D3D11_USAGE_IMMUTABLE, 
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		widgetIndices, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);

	m_mapMesh.insert(std::make_pair("WidgetMesh", widgetMesh));

	CMesh* particlePointMesh = new CStaticMesh;

	Vector3 point;

	particlePointMesh->SetName("ParticlePointMesh");
	particlePointMesh->CreateMesh(&point, sizeof(Vector3), 1, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_mapMesh.insert(std::make_pair("ParticlePointMesh", particlePointMesh));

	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& name)
{
	auto iter = m_mapMesh.find(name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

bool CMeshManager::LoadMesh(Mesh_Type type, const std::string& name, const TCHAR* fileName, const std::string& pathName, 
	class CScene* scene)
{
	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	return LoadMeshFullPath(type, name, fullPath, scene);
}

bool CMeshManager::LoadMeshFullPath(Mesh_Type type, const std::string& name, const TCHAR* fullPath,
	class CScene* scene)
{
	char fullPathMultybyte[MAX_PATH] = {};

#ifdef UNICODE

	int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultybyte, convertLength, nullptr, nullptr);
#else
	strcpy_s(fullPathMultybyte, fullPath);

#endif // UNICODE

	return LoadMeshFullPathMultibyte(type, name, fullPathMultybyte, scene);
}

bool CMeshManager::LoadMeshMultibyte(Mesh_Type type, const std::string& name, const char* fileName, const std::string& pathName, 
	class CScene* scene)
{
	char fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		strcpy_s(fullPath, info->pathMultiByte);

	strcat_s(fullPath, fileName);

	return LoadMeshFullPathMultibyte(type, name, fullPath, scene);
}

bool CMeshManager::LoadMeshFullPathMultibyte(Mesh_Type type, const std::string& name, const char* fullPath, class CScene* scene)
{
	CMesh* mesh = FindMesh(name);

	if (mesh)
		return true;

	switch (type)
	{
	case Mesh_Type::Sprite:
		mesh = new CSpriteMesh;
		break;
	case Mesh_Type::Static:
		mesh = new CStaticMesh;
		break;
	case Mesh_Type::Animation:
		mesh = new CAnimationMesh;
		break;
	}

	mesh->SetName(name);
	mesh->SetScene(scene);

	if (!mesh->LoadMeshFullPathMultibyte(fullPath))
	{
		SAFE_RELEASE(mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(name, mesh));

	return true;
}

bool CMeshManager::SetMeshSkeleton(const std::string& name, CSkeleton* skeleton)
{
	CAnimationMesh* mesh = (CAnimationMesh*)FindMesh(name);

	if (!mesh)
		return false;
	else if (!mesh->TypeCheck<CAnimationMesh>())
		return false;

	mesh->SetSkeleton(skeleton);

	return true;
}

void CMeshManager::ReleaseMesh(const std::string& name)
{
	auto iter = m_mapMesh.find(name);

	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMesh.erase(iter);
	}
}
