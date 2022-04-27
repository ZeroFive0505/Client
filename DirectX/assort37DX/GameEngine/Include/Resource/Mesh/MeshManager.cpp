
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
	CMesh* SpriteMesh = new CSpriteMesh;

	if (!SpriteMesh->Init())
	{
		SpriteMesh->Release();
		return false;
	}

	SpriteMesh->SetName("SpriteMesh");
	m_mapMesh.insert(std::make_pair("SpriteMesh", SpriteMesh));

	CMesh* FrameRectMesh = new CStaticMesh;

	Vector3	FrameRectPos[5] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};

	FrameRectMesh->SetName("FrameRect");
	FrameRectMesh->CreateMesh(FrameRectPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("FrameRect", FrameRectMesh));



	CMesh* Box2DMesh = new CStaticMesh;

	Vector3	Box2DPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	Box2DMesh->SetName("Box2D");
	Box2DMesh->CreateMesh(Box2DPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("Box2D", Box2DMesh));



	CMesh* CircleMesh = new CStaticMesh;

	Vector3	CirclrPos[31];

	CirclrPos[0] = Vector3(0.5f, 0.f, 0.f);

	for (int i = 1; i < 31; ++i)
	{
		CirclrPos[i].x = cosf(DegreeToRadian(12.f * i)) * 0.5f;
		CirclrPos[i].y = sinf(DegreeToRadian(12.f * i)) * 0.5f;
	}

	CircleMesh->SetName("Circle");
	CircleMesh->CreateMesh(CirclrPos, sizeof(Vector3), 31, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("Circle", CircleMesh));


	CMesh* WidgetMesh = new CStaticMesh;

	VertexUV	WidgetVtx[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	int	WidgetIdx[6] = { 0, 1, 3, 0, 3, 2 };

	WidgetMesh->SetName("WidgetMesh");
	WidgetMesh->CreateMesh(WidgetVtx, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		WidgetIdx, 4, 6, D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT_R32_UINT);

	m_mapMesh.insert(std::make_pair("WidgetMesh", WidgetMesh));

	CMesh* ParticlePointMesh = new CStaticMesh;

	Vector3	Point;

	ParticlePointMesh->SetName("ParticlePointMesh");
	ParticlePointMesh->CreateMesh(&Point, sizeof(Vector3), 1, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_mapMesh.insert(std::make_pair("ParticlePointMesh", ParticlePointMesh));

	return true;
}

bool CMeshManager::LoadMesh(Mesh_Type Type, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName, class CScene* Scene)
{
	TCHAR	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	return LoadMeshFullPath(Type, Name, FullPath, Scene);
}

bool CMeshManager::LoadMeshFullPath(Mesh_Type Type, 
	const std::string& Name, const TCHAR* FullPath, class CScene* Scene)
{
	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int	ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, FullPath);

#endif // UNICODE

	return LoadMeshFullPathMultibyte(Type, Name, FullPathMultibyte, Scene);
}

bool CMeshManager::LoadMeshMultibyte(Mesh_Type Type,
	const std::string& Name, const char* FileName, 
	const std::string& PathName, class CScene* Scene)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	return LoadMeshFullPathMultibyte(Type, Name, FullPath, Scene);
}

bool CMeshManager::LoadMeshFullPathMultibyte(Mesh_Type Type, const std::string& Name, const char* FullPath, class CScene* Scene)
{
	CMesh* Mesh = FindMesh(Name);

	if (Mesh)
		return true;

	switch (Type)
	{
	case Mesh_Type::Sprite:
		Mesh = new CSpriteMesh;
		break;
	case Mesh_Type::Static:
		Mesh = new CStaticMesh;
		break;
	case Mesh_Type::Animation:
		Mesh = new CAnimationMesh;
		break;
	}

	Mesh->SetName(Name);
	Mesh->SetScene(Scene);

	if (!Mesh->LoadMeshFullPathMultibyte(FullPath))
	{
		SAFE_RELEASE(Mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, Mesh));

	return true;
}

bool CMeshManager::SetMeshSkeleton(const std::string& Name, CSkeleton* Skeleton)
{
	CAnimationMesh* Mesh = (CAnimationMesh*)FindMesh(Name);

	if (!Mesh)
		return false;

	else if (!Mesh->CheckType<CAnimationMesh>())
		return false;

	Mesh->SetSkeleton(Skeleton);

	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMesh.erase(iter);
	}
}
