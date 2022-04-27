#include "AnimationMesh.h"
#include "FBXLoader.h"
#include "../Animation/Skeleton.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CAnimationMesh::CAnimationMesh()
{
	SetTypeID<CAnimationMesh>();
	m_MeshType = Mesh_Type::Animation;
}

CAnimationMesh::~CAnimationMesh()
{
}

bool CAnimationMesh::Init()
{
	return true;
}

void CAnimationMesh::SetSkeleton(CSkeleton* skeleton)
{
	m_Skeleton = skeleton;
}

void CAnimationMesh::SetSkeleton(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_Skeleton = new CSkeleton;

	char fileNameMultibyte[MAX_PATH] = {};

	int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMultibyte, length, 0, 0);

	m_Skeleton->LoadSkeleton(m_Scene, name, fileNameMultibyte, pathName);
}

bool CAnimationMesh::LoadMeshFullPathMultibyte(const char* fullPath)
{
	char ext[_MAX_EXT] = {};

	_splitpath_s(fullPath, 0, 0, 0, 0, 0, 0, ext, _MAX_EXT);
	_strupr_s(ext);

	if (strcmp(ext, ".FBX") == 0)
	{
		CFBXLoader loader;

		if (!loader.LoadFBX(fullPath, false))
			return false;

		return ConvertFBX(&loader, fullPath);
	}

	return LoadMeshFile(fullPath);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* loader, const char* fullPath)
{
	if (!CMesh::ConvertFBX(loader, fullPath))
		return false;

	// 애니메이션 처리
	const std::vector<PFBXBONE>* pvecBone = loader->GetBones();

	if (pvecBone->empty())
		return true;

	std::string strSkeletonName = m_Name + "_Skeleton";

	m_Skeleton = new CSkeleton;

	// 본 수만큼 반복한다.
	std::vector<PFBXBONE>::const_iterator iterB;
	std::vector<PFBXBONE>::const_iterator iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; iterB++)
	{
		Bone* pBone = new Bone;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pBone->matBone[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}

		m_Skeleton->AddBone(pBone);
	}

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = loader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, fullPath);

	int iLength = (int)strlen(strAnimPath);

	for (int i = iLength - 1; i >= 0; i--)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<PFBXANIMATIONCLIP>::const_iterator iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; iterC++)
	{
		m_Scene->GetSceneResource()->LoadAnimationSequence((*iterC)->strName, false, *iterC);

		CAnimationSequence* pSequence = m_Scene->GetSceneResource()->FindAnimationSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		/*char strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, strAnimPath);
		strcat_s(strAnimFullPath, (*iterC)->strName.c_str());
		strcat_s(strAnimFullPath, ".sqc");
		pSequence->SaveFullPathMultibyte(strAnimFullPath);*/

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, fullPath);
		memcpy(&strAnimFullPath[iLength - 3], "sqc", 3);

		pSequence->SaveFullPathMultibyte(strAnimFullPath);
	}

	if (m_Skeleton)
	{
		char skeletonPath[MAX_PATH] = {};
		strcpy_s(skeletonPath, fullPath);
		memcpy(&skeletonPath[iLength - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(skeletonPath);
	}

	return true;
}

bool CAnimationMesh::SaveMesh(FILE* pFile)
{
	if (!CMesh::SaveMesh(pFile))
		return false;

	return true;
}

bool CAnimationMesh::LoadMesh(FILE* pFile)
{
	if (!CMesh::LoadMesh(pFile))
		return false;

	return true;
}
