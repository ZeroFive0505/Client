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

	std::vector<sVertex3D> vecSphere;
	std::vector<Vector3> vecSpherePos;
	std::vector<int> vecSphereIndex;

	CreateSphere(vecSphere, vecSphereIndex, 1.0f, 5);

	size_t sphereSize = vecSphere.size();
	vecSpherePos.resize(sphereSize);

	for (size_t i = 0; i < sphereSize; i++)
	{
		vecSpherePos[i] = vecSphere[i].pos;
	}

	CreateMesh(Mesh_Type::Static, "SpherePos",
		&vecSpherePos[0], sizeof(Vector3), sphereSize,
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&vecSphereIndex[0], sizeof(int), (int)vecSphereIndex.size(),
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	return true;
}

bool CMeshManager::CreateMesh(Mesh_Type type, const std::string& name, void* vtxData, int size, int count,
	D3D11_USAGE usage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt, CScene* scene)
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

	if (!mesh->CreateMesh(vtxData, size, count, usage, primitive, idxData, idxSize, idxCount, idxUsage, fmt))
	{
		SAFE_RELEASE(mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(name, mesh));

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

bool CMeshManager::CreateSphere(std::vector<sVertex3D>& vecVertex, 
	std::vector<int>& vecIndex, float radius, unsigned int subDivision)
{
	// Put a cap on the number of subdivisions.
	subDivision = min(subDivision, 5);

	// Approximate a sphere by tessellating an icosahedron.
	const float X = 0.525731f;
	const float Z = 0.850651f;

	Vector3 pos[12] =
	{
		Vector3(-X, 0.0f, Z),  Vector3(X, 0.0f, Z),
		Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
		Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
		Vector3(0.0f, -Z, X),  Vector3(0.0f, -Z, -X),
		Vector3(Z, X, 0.0f),   Vector3(-Z, X, 0.0f),
		Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vecVertex.resize(12);
	vecIndex.resize(60);

	for (UINT i = 0; i < 12; ++i)
		vecVertex[i].pos = pos[i];

	for (UINT i = 0; i < 60; ++i)
		vecIndex[i] = k[i];

	for (UINT i = 0; i < subDivision; ++i)
		Subdivide(vecVertex, vecIndex);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < vecVertex.size(); ++i)
	{
		// Project onto unit sphere.
		Vector3	vN = vecVertex[i].pos;
		vN.Normalize();

		// Project onto sphere.
		Vector3 p = vN * radius;

		vecVertex[i].pos = p;
		// Normal이 있을 경우 따로 저장한다.

		// Derive texture coordinates from spherical coordinates.
		float theta = AngleFromXY(
			vecVertex[i].pos.x,
			vecVertex[i].pos.z);

		float phi = acosf(vecVertex[i].pos.y / radius);

		vecVertex[i].uv.x = theta / XM_2PI;
		vecVertex[i].uv.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		vecVertex[i].tangent.x = -radius * sinf(phi) * sinf(theta);
		vecVertex[i].tangent.y = 0.0f;
		vecVertex[i].tangent.z = radius * sinf(phi) * cosf(theta);

		vecVertex[i].binormal = vecVertex[i].normal.Cross(vecVertex[i].tangent);
		vecVertex[i].binormal.Normalize();
	}

	return true;
}

void CMeshManager::Subdivide(std::vector<sVertex3D>& vecVertices, std::vector<int>& vecIndices)
{
	// Save a copy of the input geometry.
	std::vector<sVertex3D>	vecCopyVertex = vecVertices;
	std::vector<int>	vecCopyIndex = vecIndices;


	vecVertices.resize(0);
	vecIndices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = vecCopyIndex.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		sVertex3D v0 = vecCopyVertex[vecCopyIndex[i * 3 + 0]];
		sVertex3D v1 = vecCopyVertex[vecCopyIndex[i * 3 + 1]];
		sVertex3D v2 = vecCopyVertex[vecCopyIndex[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		sVertex3D m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.pos = Vector3(
			0.5f * (v0.pos.x + v1.pos.x),
			0.5f * (v0.pos.y + v1.pos.y),
			0.5f * (v0.pos.z + v1.pos.z));

		m1.pos = Vector3(
			0.5f * (v1.pos.x + v2.pos.x),
			0.5f * (v1.pos.y + v2.pos.y),
			0.5f * (v1.pos.z + v2.pos.z));

		m2.pos = Vector3(
			0.5f * (v0.pos.x + v2.pos.x),
			0.5f * (v0.pos.y + v2.pos.y),
			0.5f * (v0.pos.z + v2.pos.z));

		//
		// Add new geometry.
		//

		vecVertices.push_back(v0); // 0
		vecVertices.push_back(v1); // 1
		vecVertices.push_back(v2); // 2
		vecVertices.push_back(m0); // 3
		vecVertices.push_back(m1); // 4
		vecVertices.push_back(m2); // 5

		vecIndices.push_back(i * 6 + 0);
		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 5);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 2);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 1);
		vecIndices.push_back(i * 6 + 4);
	}
}

float CMeshManager::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + PI; // in [0, 2*pi).

	return theta;
}
