#include "Mesh.h"
#include "../../Device.h"
#include "../../PathManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"

#include "FBXLoader.h"

CMesh::CMesh() :
	m_Scene(nullptr),
	m_Min(FLT_MAX, FLT_MAX, FLT_MAX),
	m_Max(FLT_MIN, FLT_MIN, FLT_MIN)
{
}

CMesh::~CMesh()
{
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}

	size = m_vecMeshSlot.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}
}

void CMesh::AddMaterialSlot(const std::string& name)
{
	CMaterial* material = nullptr;

	if (m_Scene)
	{
		material = m_Scene->GetSceneResource()->FindMaterial(name);
	}
	else
	{
		material = CResourceManager::GetInst()->FindMaterial(name);
	}

	if (material)
		m_vecMaterialSlot.push_back(material);
}

bool CMesh::CreateMesh(void* vtxData, int size, int count, D3D11_USAGE usage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	sMeshContainer* container = new sMeshContainer;

	container->VB.Size = size;
	container->VB.Count = count;
	container->Primitive = primitive;

	if (!CreateBuffer(Buffer_Type::Vertex, vtxData, size, count, usage, &container->VB.Buffer))
		return false;

	container->VB.Data = new char[size * count];
	memcpy(container->VB.Data, vtxData, size * count);

	sMeshSlot* slot = new sMeshSlot;

	m_vecMeshSlot.push_back(slot);

	slot->VB = &container->VB;
	slot->primitive = container->Primitive;

	if (idxData != nullptr)
	{
		container->vecIB.resize(1);

		container->vecIB[0].Size = idxSize;
		container->vecIB[0].Count = idxCount;
		container->vecIB[0].Fmt = fmt;

		if (!CreateBuffer(Buffer_Type::Index, idxData, idxSize, idxCount, idxUsage, &container->vecIB[0].Buffer))
			return false;

		container->vecIB[0].Data = new char[idxSize * idxCount];
		memcpy(container->vecIB[0].Data, idxData, idxSize * idxCount);

		slot->IB = &container->vecIB[0];
	}

	m_vecContainer.push_back(container);

	return true;
}

bool CMesh::LoadMesh(const TCHAR* fileName, const std::string& pathName)
{
	const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	TCHAR fullPath[MAX_PATH] = {};

	if (path)
		lstrcpy(fullPath, path->path);

	lstrcat(fullPath, fileName);

	return LoadMeshFullPath(fullPath);
}

bool CMesh::LoadMeshFullPath(const TCHAR* fullPath)
{
	char fullPathMultybyte[MAX_PATH] = {};

	int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultybyte, length, 0, 0);

	return LoadMeshFullPathMultibyte(fullPathMultybyte);
}

bool CMesh::LoadMeshMultibyte(const char* fileName, const std::string& pathName)
{
	const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
		strcpy_s(fullPath, path->pathMultiByte);

	strcat_s(fullPath, fileName);

	return LoadMeshFullPathMultibyte(fullPath);
}

bool CMesh::LoadMeshFullPathMultibyte(const char* fullPath)
{
	return false;
}

bool CMesh::Init()
{
	return true;
}

bool CMesh::CreateBuffer(Buffer_Type type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	// ???? ?????? ??????
	D3D11_BUFFER_DESC desc = {};

	// ?????? ?? ????
	desc.ByteWidth = size * count;

	// ?????? ????????
	desc.Usage = usage;

	if (type == Buffer_Type::Vertex)
	{
		// ???? ?????? ?????? ?????? ??????????.
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		// ???? ?????? ?????? ?????? ??????????.
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}

	if (usage == D3D11_USAGE_DYNAMIC)
	{
		// ???????????? CPU?? ?? ?????? ?? ?? ????.
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (usage == D3D11_USAGE_STAGING)
	{
		// ?????????? ???? ???? ???? ?? ?? ????
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	// ?????? ?????? ?????????? ???????? ??
	D3D11_SUBRESOURCE_DATA bufferData = {};

	// ???????? ??????.
	bufferData.pSysMem = data;

	// ???? ???????? bufferData?? ?????????? ???? buffer?? ????????.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, &bufferData, buffer)))
		return false;

	if (type == Buffer_Type::Vertex)
	{
		// char?? ???? ???? 1??????, ??????
		// data?? 1???????? ?????? ??????????.
		char* vertexData = (char*)data;

		for (int i = 0; i < count; i++)
		{
			// ?? 1?????? ???????????? 3???????? Vector3?????? ????????.
			Vector3* pos = (Vector3*)vertexData;
			// ?????? ?????? ???? ???? vertexData?? ?????? ?????? ???????? ????????.
			vertexData += size;

			if (m_Min.x > pos->x)
				m_Min.x = pos->x;

			if (m_Min.y > pos->y)
				m_Min.y = pos->y;

			if (m_Min.z > pos->z)
				m_Min.z = pos->z;

			if (m_Max.x < pos->x)
				m_Max.x = pos->x;

			if (m_Max.y < pos->y)
				m_Max.y = pos->y;

			if (m_Max.z < pos->z)
				m_Max.z = pos->z;
		}
	}

	return true;
}

bool CMesh::SaveMeshFile(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "wb");

	if (!pFile)
		return false;

	SaveMesh(pFile);

	fclose(pFile);

	return true;
}

bool CMesh::LoadMeshFile(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "rb");

	if (!pFile)
		return false;

	LoadMesh(pFile);

	fclose(pFile);

	return true;
}

bool CMesh::ConvertFBX(CFBXLoader* loader, const char* fullPath)
{
	// FBX?????? ???? ?????????? ???? ???????????? ????????.
	const std::vector<std::vector<PFBXMATERIAL>>* vecMaterials = loader->GetMaterials();
	const std::vector<PFBXMESHCONTAINER>* vecContainers = loader->GetMeshContainers();

	// ???? ?????????? ????????.
	auto iter = vecContainers->begin();
	auto iterEnd = vecContainers->end();

	// ???? ??????, ?????????? ?????? ?????? ????
	bool bumpEnable = false;
	bool animationEnable = false;

	// ?? ?????? ?????? ?????? ???? ?????? ????
	std::vector<std::vector<bool>> vecEmptyIndex;

	// 0?? ???? ????????.
	int containerIndex = 0;

	for (; iter != iterEnd; iter++, containerIndex++)
	{
		// ???? ???????? ???????? ???? ??????????.
		sMeshContainer* container = new sMeshContainer;

		m_vecContainer.push_back(container);
		
		std::vector<bool> vecEmpty;
		vecEmptyIndex.push_back(vecEmpty);

		// ???? ?????? ???????????? ??????
		if ((*iter)->Bump)
			bumpEnable = true;

		if ((*iter)->Animation)
			animationEnable = true;

		// ???? ?????? ???? ?????? ???????? ???? ????
		std::vector<sVertex3D> vecVtx;

		size_t vtxCount = (*iter)->vecPos.size();

		// ?????????? ???? ?????? ??????????????.
		vecVtx.resize(vtxCount);

		for (size_t i = 0; i < vtxCount; i++)
		{
			// ???? ??????
			sVertex3D vtx = {};

			vtx.pos = (*iter)->vecPos[i];
			vtx.normal = (*iter)->vecNormal[i];
			vtx.uv = (*iter)->vecUV[i];

			// ?????? ???? ???? Tangent, Binormal?? ???????? ????????.
			if (bumpEnable)
			{
				if (!(*iter)->vecTangent.empty())
					vtx.tangent = (*iter)->vecTangent[i];

				if (!(*iter)->vecBinormal.empty())
					vtx.binormal = (*iter)->vecBinormal[i];
			}

			if (!(*iter)->vecBlendWeight.empty() && animationEnable)
			{
				vtx.blendWeight = (*iter)->vecBlendWeight[i];
				vtx.blendIdx = (*iter)->vecBlendIndex[i];
			}

			// ????
			vecVtx[i] = vtx;
		}

		// Mesh?? Vertex ????
		if (!CreateBuffer(Buffer_Type::Vertex,
			&vecVtx[0], sizeof(sVertex3D), (int)vecVtx.size(), D3D11_USAGE_DEFAULT,
			&container->VB.Buffer))
			return false;

		// ?????? ?????? ?????? ?????? ??????.
		container->VB.Size = sizeof(sVertex3D);
		container->VB.Count = (int)vecVtx.size();
		container->VB.Data = new char[container->VB.Size * container->VB.Count];
		memcpy(container->VB.Data, &vecVtx[0], container->VB.Size * container->VB.Count);

		container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		size_t idxCount = (*iter)->vecIndices.size();

		for (size_t i = 0; i < idxCount; i++)
		{
			// ?????? ?????? ?????? ???????? ?????? ???? ???????? ????????.
			if ((*iter)->vecIndices[i].empty())
			{
				vecEmptyIndex[containerIndex].push_back(false);
				continue;
			}

			// ?????? ?????? ????????.
			vecEmptyIndex[containerIndex].push_back(true);

			// ?????? ?????? ???? ?????? ?????? ?? ????. ?????? ?????? ???? ?????? ?????? ????????.
			sMeshSlot* slot = new sMeshSlot;

			m_vecMeshSlot.push_back(slot);

			// ?????? ?????? ?????? ?????? ????????.
			slot->VB = &container->VB;
			slot->primitive = container->Primitive;

			sIndexBuffer idxBuffer = {};

			// ?????????? ?????? ?????? ??????.
			container->vecIB.push_back(idxBuffer);

			if (!CreateBuffer(Buffer_Type::Index,
				&(*iter)->vecIndices[i][0], sizeof(UINT),
				(int)(*iter)->vecIndices[i].size(),
				D3D11_USAGE_DEFAULT, &container->vecIB[i].Buffer))
				return false;

			// ?????? ?????? ?????? ???? ?????? ????????.
			container->vecIB[i].Size = sizeof(UINT);
			container->vecIB[i].Count = (int)(*iter)->vecIndices[i].size();
			container->vecIB[i].Fmt = DXGI_FORMAT_R32_UINT;
			container->vecIB[i].Data = new char[container->vecIB[i].Size * container->vecIB[i].Count];
			memcpy(container->vecIB[i].Data, &(*iter)->vecIndices[i][0],
				container->vecIB[i].Size * container->vecIB[i].Count);

			// ?????? ?????? ?????? ????????.
			slot->IB = &container->vecIB[i];
		}
	}

	// ???? ?????? ????????.
	auto iterM = vecMaterials->begin();
	auto iterMEnd = vecMaterials->end();

	containerIndex = 0;

	for (; iterM != iterMEnd; iterM++, containerIndex++)
	{
		// ?????? ?? ???? ????????.
		size_t size = (*iterM).size();

		for (size_t i = 0; i < size; i++)
		{
			if (!vecEmptyIndex[containerIndex][i])
				continue;

			PFBXMATERIAL mtrl = (*iterM)[i];

			CMaterial* material = new CMaterial;

			material->CreateConstantBuffer();

			material->SetBaseColor(mtrl->BaseColor);
			material->SetAmbientColor(mtrl->Amb);
			material->SetSpecularColor(mtrl->Spc);
			material->SetEmissiveColor(mtrl->Emv);
			material->SetSpecularPower(mtrl->Shininess);

			material->SetShader("Standard3DShader");

			AddMaterialSlot(material);

			// Texture
			char fileName[MAX_PATH] = {};
			_splitpath_s(mtrl->DifTex.c_str(), 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);

			TCHAR fullPath[MAX_PATH] = {};

#ifdef UNICODE
			int pathLength = MultiByteToWideChar(CP_ACP, 0, mtrl->DifTex.c_str(),
				-1, 0, 0);
			MultiByteToWideChar(CP_ACP, 0, mtrl->DifTex.c_str(), -1, fullPath, pathLength);
#else
			strcpy_s(fullPath, mtrl->DifTex.c_str());
#endif // UNICODE

			material->AddTextureFullPath(0, (int)Buffer_Shader_Type::Pixel, fileName, fullPath);

			if (!mtrl->BumpTex.empty())
			{
				material->EnableBump();
				memset(fileName, 0, MAX_PATH);
				_splitpath_s(mtrl->BumpTex.c_str(), 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);

				memset(fullPath, 0, sizeof(TCHAR)* MAX_PATH);

#ifdef UNICODE
				int pathLength = MultiByteToWideChar(CP_ACP, 0, mtrl->BumpTex.c_str(),
					-1, 0, 0);
				MultiByteToWideChar(CP_ACP, 0, mtrl->BumpTex.c_str(), -1, fullPath, pathLength);
#else
				strcpy_s(fullPath, mtrl->BumpTex.c_str());
#endif // UNICODE

				material->AddTextureFullPath(1, (int)Buffer_Shader_Type::Pixel, fileName, fullPath);

			}

			if (!mtrl->SpcTex.empty())
			{
				memset(fileName, 0, MAX_PATH);
				_splitpath_s(mtrl->SpcTex.c_str(), 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);

				memset(fullPath, 0, sizeof(TCHAR)* MAX_PATH);

#ifdef UNICODE
				int pathLength = MultiByteToWideChar(CP_ACP, 0, mtrl->SpcTex.c_str(),
					-1, 0, 0);
				MultiByteToWideChar(CP_ACP, 0, mtrl->SpcTex.c_str(), -1, fullPath, pathLength);
#else
				strcpy_s(fullPath, mtrl->SpcTex.c_str());
#endif // UNICODE

				material->AddTextureFullPath(2, (int)Buffer_Shader_Type::Pixel, fileName, fullPath);

				material->EnableSpecularTex();
			}


			if (bumpEnable)
				material->EnableBump();

			if (animationEnable)
				material->EnableAnimation3D();
		}
	}

	// ???? ???????? ??????????.
	char meshFullPath[MAX_PATH] = {};

	strcpy_s(meshFullPath, fullPath);
	int pathLength = (int)strlen(fullPath);
	memcpy(&meshFullPath[pathLength - 3], "msh", 3);

	SaveMeshFile(meshFullPath);

	return true;
}

bool CMesh::SaveMesh(FILE* pFile)
{
	// ?????? ???????? ???? ???? ?????? ????????.
	fwrite(&m_MeshType, sizeof(Mesh_Type), 1, pFile);

	// ???????? ?????????? ???? ????????.
	fwrite(&m_Min, sizeof(Vector3), 1, pFile);
	fwrite(&m_Max, sizeof(Vector3), 1, pFile);

	// ?????? ????????.
	int length = (int)m_Name.length();

	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), length, pFile);

	// ???? ?????? ?????????? ?????? ????????.
	int containerCount = (int)m_vecContainer.size();
	fwrite(&containerCount, sizeof(int), 1, pFile);

	// ???????? ???????? ?????????? ?????????? ????????.
	for (int i = 0; i < containerCount; i++)
	{
		sMeshContainer* container = m_vecContainer[i];

		fwrite(&container->Primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, pFile);

		fwrite(&container->VB.Size, sizeof(int), 1, pFile);
		fwrite(&container->VB.Count, sizeof(int), 1, pFile);
		fwrite(container->VB.Data, container->VB.Size, container->VB.Count, pFile);

		// ???? ???? ?????????? ?????? ?????? ?????? ????????.
		int idxCount = (int)container->vecIB.size();

		fwrite(&idxCount, sizeof(int), 1, pFile);

		// ?????? ?????? ?????? ?????????? ????????.
		for (int j = 0; j < idxCount; j++)
		{
			fwrite(&container->vecIB[j].Size, sizeof(int), 1, pFile);
			fwrite(&container->vecIB[j].Count, sizeof(int), 1, pFile);
			fwrite(&container->vecIB[j].Fmt, sizeof(DXGI_FORMAT), 1, pFile);
			fwrite(container->vecIB[j].Data, container->vecIB[j].Size, container->vecIB[j].Count, pFile);
		}
	}

	// ?????????? ????????.
	int materialCount = (int)m_vecMaterialSlot.size();

	fwrite(&materialCount, sizeof(int), 1, pFile);

	for (int i = 0; i < materialCount; i++)
	{
		m_vecMaterialSlot[i]->Save(pFile);
	}

	return true;
}

bool CMesh::LoadMesh(FILE* pFile)
{
	fread(&m_MeshType, sizeof(Mesh_Type), 1, pFile);

	fread(&m_Min, sizeof(Vector3), 1, pFile);
	fread(&m_Max, sizeof(Vector3), 1, pFile);

	int length = 0;

	char name[256] = {};

	fread(&length, sizeof(int), 1, pFile);
	fread(name, sizeof(char), length, pFile);

	m_Name = name;

	// ?????????? ?????? ???????? ?? ?? ???? ????????.
	int containerCount = 0;
	fread(&containerCount, sizeof(int), 1, pFile);

	for (int i = 0; i < containerCount; i++)
	{
		// ?????? ?????? ??????????.
		sMeshContainer* container = new sMeshContainer;

		m_vecContainer.push_back(container);

		// ?????? ???? ?????? ????????.
		fread(&container->Primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, pFile);

		// ?????? ?????? ?????? ?????? ????????.
		fread(&container->VB.Size, sizeof(int), 1, pFile);
		fread(&container->VB.Count, sizeof(int), 1, pFile);

		// 1?????? ?????? ?????? ???????? ???????? ?????? ??????????.
		container->VB.Data = new char[container->VB.Size * container->VB.Count];

		// ???????? ?????? ???????? ??????????.
		fread(container->VB.Data, container->VB.Size, container->VB.Count, pFile);

		// ???????? ???????? ???????? ?????? ??????????.
		if (!CreateBuffer(Buffer_Type::Vertex, container->VB.Data, container->VB.Size, container->VB.Count,
			D3D11_USAGE_DEFAULT, &container->VB.Buffer))
			return false;

		int idxCount = 0;

		fread(&idxCount, sizeof(int), 1, pFile);

		container->vecIB.resize(idxCount);


		for (int j = 0; j < idxCount; j++)
		{
			fread(&container->vecIB[j].Size, sizeof(int), 1, pFile);
			fread(&container->vecIB[j].Count, sizeof(int), 1, pFile);
			fread(&container->vecIB[j].Fmt, sizeof(DXGI_FORMAT), 1, pFile);

			container->vecIB[j].Data = new char[container->vecIB[j].Size * container->vecIB[j].Count];

			fread(container->vecIB[j].Data, container->vecIB[j].Size, container->vecIB[j].Count, pFile);

			if (!CreateBuffer(Buffer_Type::Index, container->vecIB[j].Data, container->vecIB[j].Size,
				container->vecIB[j].Count, D3D11_USAGE_DEFAULT, &container->vecIB[j].Buffer))
				return false;

			sMeshSlot* slot = new sMeshSlot;

			m_vecMeshSlot.push_back(slot);

			slot->VB = &container->VB;
			slot->primitive = container->Primitive;

			slot->IB = &container->vecIB[j];
		}
	}

	int materialCount = 0;

	fread(&materialCount, sizeof(int), 1, pFile);

	for (int i = 0; i < materialCount; i++)
	{
		CMaterial* material = new CMaterial;

		m_vecMaterialSlot.push_back(material);

		material->Load(pFile);
	}

	return true;
}

void CMesh::Render()
{
	// ???? ?????? ???? ???? ???? ???????????? ????????.
	// ?????? ???? ???????? ???????? ???????? ???? ?????????? ???? ???? ???? ????
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		// ??????????: ???????? ???? ???????? ???????? ?????? ????????????.
		unsigned int stride = m_vecContainer[i]->VB.Size;
		// ??????: ?????????? ????????.
		unsigned int offset = 0;

		// IA ?????????? ???? ?????? ?????? ???? ?????? ????.
		// ?? ?????? ?????? ???? ???????? ???????? ????
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		// ?????????????? ???? ?????? ??????.
		// ?????? ?????? ????
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecContainer[i]->VB.Buffer, &stride, &offset);

		size_t idxCount = m_vecContainer[i]->vecIB.size();

		// ???????? ?????? ????
		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; j++)
			{
				// ?????? ?????? ???? ????????.
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				// ?????? ???????? ???????? ???? ?? ???? ????????.
				// ???????? ?????? ???????? ???????? ??????????
				// ?????? ?????? ?????? ????
				CDevice::GetInst()->GetContext()->DrawIndexed(m_vecContainer[i]->vecIB[j].Count, 0, 0);
			}
		}
		// ???????? ???????? ???? ????
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(m_vecContainer[i]->VB.Count, 0);
		}
	}
}

void CMesh::Render(int slotNumber)
{
	unsigned int stride = (unsigned int)m_vecMeshSlot[slotNumber]->VB->Size;
	unsigned int offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[slotNumber]->primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshSlot[slotNumber]->VB->Buffer, &stride, &offset);

	if (m_vecMeshSlot[slotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[slotNumber]->IB->Buffer,
			m_vecMeshSlot[slotNumber]->IB->Fmt, 0);
		CDevice::GetInst()->GetContext()->DrawIndexed(
			m_vecMeshSlot[slotNumber]->IB->Count, 0, 0);
	}
	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(m_vecMeshSlot[slotNumber]->VB->Count, 0);
	}
}

void CMesh::RenderInstancing(int count)
{
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		unsigned int stride = (unsigned int)m_vecContainer[i]->VB.Size;
		unsigned int offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, 
			&m_vecContainer[i]->VB.Buffer, &stride, &offset);

		size_t idxCount = m_vecContainer[i]->vecIB.size();

		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; j++)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				CDevice::GetInst()->GetContext()->DrawIndexedInstanced(m_vecContainer[i]->vecIB[j].Count, count, 0, 0, 0);
			}
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN,
				0);
			CDevice::GetInst()->GetContext()->DrawInstanced(m_vecContainer[i]->VB.Count, count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(ID3D11Buffer* instancingBuffer, unsigned int instanceSize, int count)
{
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		unsigned int stride[2] = { (unsigned int)m_vecContainer[i]->VB.Size, instanceSize };
		unsigned int offset[2] = {};

		ID3D11Buffer* buffer[2] = { m_vecContainer[i]->VB.Buffer, instancingBuffer };

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 2, buffer, stride, offset);

		size_t idxCount = m_vecContainer[i]->vecIB.size();

		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; j++)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
					m_vecContainer[i]->vecIB[j].Count, count, 0, 0, 0
				);
			}
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(m_vecContainer[i]->VB.Count, count, 0, 0);
		}
	}
}
