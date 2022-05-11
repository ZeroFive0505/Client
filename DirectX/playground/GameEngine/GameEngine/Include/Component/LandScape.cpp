#include "LandScape.h"
#include "../PathManager.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material/Material.h"

CLandScape::CLandScape() :
	m_CountX(0),
	m_CountZ(0)
{
	SetTypeID<CLandScape>();
	m_Render = true;
}

CLandScape::CLandScape(const CLandScape& com) :
	CSceneComponent(com)
{
	m_CountX = com.m_CountX;
	m_CountZ = com.m_CountZ;
	m_Mesh = com.m_Mesh;
	m_vecVtx = com.m_vecVtx;
	m_vecPos = com.m_vecPos;
	m_vecIndex = com.m_vecIndex;
}

CLandScape::~CLandScape()
{
}

void CLandScape::CreateLandScape(const std::string& name, 
	int countX, int countZ, const TCHAR* heightMap, const std::string& pathName)
{
	m_CountX = countX;
	m_CountZ = countZ;

	std::vector<float> vecY;

	if (heightMap)
	{
		TCHAR fullPath[MAX_PATH] = {};

		const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

		if (path)
			lstrcpy(fullPath, path->path);
		lstrcat(fullPath, heightMap);

		ScratchImage img;

		char ext[_MAX_EXT] = {};
		char fullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, convertLength, nullptr, nullptr);

#else
		strcpy_s(fullPathMultibyte, fullPath);

#endif // UNICODE

		_splitpath_s(fullPathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

		_strupr_s(ext);

		if (strcmp(ext, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, img)))
				return;
		}
		else if (strcmp(ext, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(fullPath, nullptr, img)))
				return;
		}
		else
		{
			if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, img)))
				return;
		}

		// 픽셀 값을 가져온다.
		const Image* pixelInfo = img.GetImages();

		for (size_t i = 0; i < pixelInfo->height; i++)
		{
			for (size_t j = 0; j < pixelInfo->width; j++)
			{
				// 색상은 RGBA가 짝을 이뤄서 저장하고 있다 따라서 다음 픽셀르 넘어갈때는 4바이트씩 넘어가게 한다.
				int pixelIndex = (int)i * (int)pixelInfo->width * 4 + (int)j * 4;

				float y = pixelInfo->pixels[pixelIndex] / 60.0f;

				vecY.push_back(y);
			}
		}
	}
	else
		vecY.resize(countX * countZ);

	for (int i = 0; i < m_CountZ; i++)
	{
		for (int j = 0; j < m_CountX; j++)
		{
			sVertex3D vtx = {};

			vtx.pos = Vector3((float)j, vecY[i * m_CountX + j],
				(float)m_CountZ - i - 1);

			vtx.uv = Vector2(j / (float)(m_CountX - 1),
				i / (float)(m_CountZ - 1));

			m_vecVtx.push_back(vtx);
		}
	}

	// 인덱스 정보 만들기
	// 면 법선은 삼각형 수 만큼 만들어져야 한다.
	Vector3 trianglePos[3];
	Vector3 edge1, edge2;
	unsigned int triIndex = 0;

	m_vecFaceNormal.resize((m_CountX - 1) * (m_CountZ - 1) * 2);

	for (unsigned int i = 0; i < m_CountZ - 1; i++)
	{
		for (unsigned int j = 0; j < m_CountX - 1; j++)
		{
			unsigned int index = i * m_CountX + j;

			// 사각형의 우 상단 삼각형
			m_vecIndex.push_back(index);
			m_vecIndex.push_back(index + 1);
			m_vecIndex.push_back(index + m_CountX + 1);

			trianglePos[0] = m_vecVtx[index].pos;
			trianglePos[1] = m_vecVtx[index + 1].pos;
			trianglePos[2] = m_vecVtx[index + m_CountX + 1].pos;

			edge1 = trianglePos[1] - trianglePos[0];
			edge2 = trianglePos[2] - trianglePos[0];

			edge1.Normalize();
			edge2.Normalize();

			m_vecFaceNormal[triIndex] = edge1.Cross(edge2);
			m_vecFaceNormal[triIndex].Normalize();

			triIndex++;

			// 사각형의 좌 하단 삼각형
			m_vecIndex.push_back(index);
			m_vecIndex.push_back(index + m_CountX + 1);
			m_vecIndex.push_back(index + m_CountX);

			trianglePos[0] = m_vecVtx[index].pos;
			trianglePos[1] = m_vecVtx[index + m_CountX + 1].pos;
			trianglePos[2] = m_vecVtx[index + m_CountX].pos;

			edge1 = trianglePos[1] - trianglePos[0];
			edge2 = trianglePos[2] - trianglePos[0];

			edge1.Normalize();
			edge2.Normalize();

			m_vecFaceNormal[triIndex] = edge1.Cross(edge2);
			m_vecFaceNormal[triIndex].Normalize();

			triIndex++;
		}
	}

	ComputeNormal();

	ComputeTangent();

	// 메쉬를 만든다.
	m_Scene->GetSceneResource()->CreateMesh(Mesh_Type::Static, name, &m_vecVtx[0], sizeof(sVertex3D), (int)m_vecVtx.size(),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecIndex[0], sizeof(int), (int)m_vecIndex.size(),
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	m_Mesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh(name);
}

void CLandScape::SetMaterial(CMaterial* material, int index)
{
	m_vecMaterialSlot[index] = material->Clone();

	m_vecMaterialSlot[index]->SetScene(m_Scene);
}

void CLandScape::SetMaterial(const std::string& name, int index)
{
	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial(name);

	m_vecMaterialSlot[index] = material->Clone();

	m_vecMaterialSlot[index]->SetScene(m_Scene);
}

void CLandScape::AddMaterial(CMaterial* material)
{
	m_vecMaterialSlot.push_back(material->Clone());

	m_vecMaterialSlot.back()->SetScene(m_Scene);
}

void CLandScape::AddMaterial(const std::string& name)
{
	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial(name);

	m_vecMaterialSlot.push_back(material->Clone());

	m_vecMaterialSlot.back()->SetScene(m_Scene);
}

void CLandScape::Start()
{
	CSceneComponent::Start();
}

bool CLandScape::Init()
{
	return true;
}

void CLandScape::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CLandScape::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CLandScape::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CLandScape::Render()
{
	CSceneComponent::Render();

	if (!m_Mesh)
		return;

	size_t size = m_vecMaterialSlot.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecMaterialSlot[i]->Render();

		m_Mesh->Render((int)i);

		m_vecMaterialSlot[i]->Reset();
	}
}

void CLandScape::PostRender()
{
	CSceneComponent::PostRender();
}

CLandScape* CLandScape::Clone()
{
	return new CLandScape(*this);
}

void CLandScape::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CLandScape::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}

void CLandScape::ComputeNormal()
{
	size_t size = m_vecFaceNormal.size();

	for (size_t i = 0; i < size; i++)
	{
		// 삼각형의 인덱스를 구하고
		unsigned int idx0 = m_vecIndex[i * 3];
		unsigned int idx1 = m_vecIndex[i * 3 + 1];
		unsigned int idx2 = m_vecIndex[i * 3 + 2];

		// 인덱스를 기준으로 주변 삼각형의 노말을 누적시킨다.
		m_vecVtx[idx0].normal += m_vecFaceNormal[i];
		m_vecVtx[idx1].normal += m_vecFaceNormal[i];
		m_vecVtx[idx2].normal += m_vecFaceNormal[i];
	}

	size = m_vecVtx.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecVtx[i].normal.Normalize();
	}
}

void CLandScape::ComputeTangent()
{
	size_t size = m_vecFaceNormal.size();

	for (size_t i = 0; i < size; i++)
	{
		int idx0 = m_vecIndex[i * 3];
		int idx1 = m_vecIndex[i * 3 + 1];
		int idx2 = m_vecIndex[i * 3 + 2];

		// 두 간선을 구한다.
		float fVtx1[3], fVtx2[3];
		fVtx1[0] = m_vecVtx[idx1].pos.x - m_vecVtx[idx0].pos.x;
		fVtx1[1] = m_vecVtx[idx1].pos.y - m_vecVtx[idx0].pos.y;
		fVtx1[2] = m_vecVtx[idx1].pos.z - m_vecVtx[idx0].pos.z;

		fVtx2[0] = m_vecVtx[idx2].pos.x - m_vecVtx[idx0].pos.x;
		fVtx2[1] = m_vecVtx[idx2].pos.y - m_vecVtx[idx0].pos.y;
		fVtx2[2] = m_vecVtx[idx2].pos.z - m_vecVtx[idx0].pos.z;

		
		// 두 UV 좌표를 구한다. (UV좌표는 X, Z축에 붙어있는 평면 좌표다 이를 이용해서 탄젠트와 바이노말을 구한다.)
		float ftu[2], ftv[2];
		ftu[0] = m_vecVtx[idx1].uv.x - m_vecVtx[idx0].uv.x;
		ftv[0] = m_vecVtx[idx1].uv.y - m_vecVtx[idx0].uv.y;

		ftu[1] = m_vecVtx[idx2].uv.x - m_vecVtx[idx0].uv.x;
		ftv[1] = m_vecVtx[idx2].uv.y - m_vecVtx[idx0].uv.y;

		// 행렬 디터미넌트
		float fDen = 1.0f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vector3 tangent;
		tangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		tangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		tangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		tangent.Normalize();

		m_vecVtx[idx0].tangent = tangent;
		m_vecVtx[idx1].tangent = tangent;
		m_vecVtx[idx2].tangent = tangent;

		m_vecVtx[idx0].binormal = m_vecVtx[idx0].normal.Cross(tangent);
		m_vecVtx[idx1].binormal = m_vecVtx[idx1].normal.Cross(tangent);
		m_vecVtx[idx2].binormal = m_vecVtx[idx2].normal.Cross(tangent);

		m_vecVtx[idx0].binormal.Normalize();
		m_vecVtx[idx1].binormal.Normalize();
		m_vecVtx[idx2].binormal.Normalize();
	}
}
