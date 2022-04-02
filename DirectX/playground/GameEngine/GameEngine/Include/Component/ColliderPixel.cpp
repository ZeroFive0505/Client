#include "ColliderPixel.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "ColliderBox2D.h"
#include "ColliderCircle.h"
#include "../PathManager.h"
#include "CameraComponent.h"
#include "../Device.h"

CColliderPixel::CColliderPixel()
{
	SetTypeID<CColliderPixel>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Pixel;
}

CColliderPixel::CColliderPixel(const CColliderPixel& com)
{
	m_Info = com.m_Info;

	m_Info.refCount++;

	m_PixelMesh = com.m_PixelMesh;
	m_PixelShader = com.m_PixelShader;
}

CColliderPixel::~CColliderPixel()
{
	m_Info.refCount--;

	if (m_Info.refCount == 0)
	{
		SAFE_RELEASE(m_Info.SRV);
		SAFE_DELETE_ARRAY(m_Info.pixels);
	}
}

void CColliderPixel::SetInfo(const TCHAR* fileName, const std::string& pathName)
{
	const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	TCHAR fullPath[MAX_PATH] = {};

	if (path)
		lstrcpy(fullPath, path->path);
	
	lstrcat(fullPath, fileName);

	SetInfoFullPath(fullPath);
}

void CColliderPixel::SetInfoFullPath(const TCHAR* fullPath)
{
	TCHAR _fileExt[_MAX_EXT] = {};

	_wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, _fileExt, _MAX_EXT);

	char ext[_MAX_EXT] = {};

#ifdef UNICODE

	int convertLength = WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, ext, convertLength, nullptr, nullptr);

#else

	strcpy_s(ext, _fileExt);

#endif // UNICODE

	_strupr_s(ext);

	ScratchImage Image = {};

	if (strcmp(ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, Image)))
			return;
	}
	else if (strcmp(ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(fullPath, nullptr, Image)))
			return;
	}
	else
	{
		if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, Image)))
			return;
	}

	m_Info.width = (unsigned int)Image.GetImages()[0].width;
	m_Info.height = (unsigned int)Image.GetImages()[0].height;

	m_Info.pixels = new unsigned char[Image.GetPixelsSize()];

	memcpy(m_Info.pixels, Image.GetPixels(), Image.GetPixelsSize());

	if (strcmp(ext, ".BMP") == 0)
	{
		// BMP 파일은 위 아래가 반전되어 있으므로 다시 반전시켜야함

		// R(1 바이트)G(1 바이트)B(1 바이트)A(1 바이트) 4바이트 단위로 건너뛰어야한다.
		unsigned int lineSize = m_Info.width * 4;

		unsigned char* line = new unsigned char[lineSize];

		int halfHeight = m_Info.height / 2;

		for (int i = 0; i < halfHeight; i++)
		{
			// 한 라인을 임시변수에 집어넣는다.
			memcpy(line, &m_Info.pixels[i * lineSize], lineSize);
			
			// 현재 라인에 맨 아래 라인의 픽셀값을 넣는다.
			memcpy(&m_Info.pixels[i * lineSize], &m_Info.pixels[(m_Info.height - 1 - i) * lineSize], lineSize);

			// 맨 아래 라인에 임시로 저장해둔 값을 집어넣는다.
			memcpy(&m_Info.pixels[(m_Info.height - 1 - i) * lineSize], line, lineSize);
		}

		SAFE_DELETE_ARRAY(line);
	}

	SetWorldScale((float)m_Info.width, (float)m_Info.height, 1.0f);

	m_Info.box.length.x = m_Info.width / 2.0f;
	m_Info.box.length.y = m_Info.height / 2.0f;

	if (FAILED(CreateShaderResourceView(CDevice::GetInst()->GetDevice(), Image.GetImages(),
		Image.GetImageCount(), Image.GetMetadata(), &m_Info.SRV)))
		return;
}

void CColliderPixel::SetInfoMultibyte(const char* fileName, const std::string& pathName)
{
	const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
		strcpy_s(fullPath, path->pathMultiByte);

	strcat_s(fullPath, fileName);

	SetInfoMultibyteFullPath(fullPath);
}

void CColliderPixel::SetInfoMultibyteFullPath(const char* fullPath)
{
	TCHAR convertPath[MAX_PATH] = {};

	int length = MultiByteToWideChar(CP_ACP, 0, fullPath, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, fullPath, -1, convertPath, length);

	SetInfoFullPath(convertPath);
}

void CColliderPixel::SetPixelCollisionType(PixelCollision_Type type)
{
	m_Info.type = type;
}

void CColliderPixel::SetPixelColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_Info.colors[0] = r;
	m_Info.colors[1] = g;
	m_Info.colors[2] = b;
}

void CColliderPixel::SetPixelAlpha(unsigned char a)
{
	m_Info.colors[3] = a;
}

void CColliderPixel::Start()
{
	CColliderComponent::Start();

	SetWorldScale((float)m_Info.width, (float)m_Info.height, 1.0f);
}

bool CColliderPixel::Init()
{
	if (!CColliderComponent::Init())
		return false;

	// 픽셀 충돌의 경우 축은 고정
	m_Info.box.axis[0] = Vector2(1.0f, 0.0f);
	m_Info.box.axis[1] = Vector2(0.0f, 1.0f);

	m_Mesh = m_Scene->GetSceneResource()->FindMesh("Box2D");

	m_PixelMesh = m_Scene->GetSceneResource()->FindMesh("SpriteMesh");

	m_PixelShader = m_Scene->GetSceneResource()->FindShader("ColliderPixelShader");

	SetWorldScale((float)m_Info.width, (float)m_Info.height, 1.0f);

	return true;
}

void CColliderPixel::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderPixel::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);

	m_Info.box.center.x = GetWorldPos().x + m_Offset.x;
	m_Info.box.center.y = GetWorldPos().y + m_Offset.y;

	
	m_Min.x = m_Info.box.center.x - m_Info.box.length.x;
	m_Min.y = m_Info.box.center.y - m_Info.box.length.y;

	m_Max.x = m_Info.box.center.x + m_Info.box.length.x;
	m_Max.y = m_Info.box.center.y + m_Info.box.length.y;


	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;

	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;
}

void CColliderPixel::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderPixel::Render()
{
	CColliderComponent::Render();

	CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Matrix matWorld, matView, matProj, matWVP;

	matView = camera->GetViewMatrix();
	matProj = camera->GetProjMatrix();

	Matrix matSCale, matTrans;

	matSCale.Scaling((float)m_Info.width, (float)m_Info.height, 1.0f);
	matTrans.Translation(m_Info.box.center);

	matWorld = matSCale * matTrans;

	matWVP = matWorld * matView * matProj;

	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	if (m_PrevCollisionList.empty())
		m_CBuffer->SetColliderColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	else
		m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	if (m_MouseCollision)
		m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	m_CBuffer->UpdateCBuffer();

	// 픽셀 충돌체의 경우에는 외곽선뿐만 아니라 텍스쳐도 출력해야하기에 SRV를 이용한다.
	CDevice::GetInst()->GetContext()->PSSetShaderResources(0, 1, &m_Info.SRV);

	m_PixelShader->SetShader();

	m_PixelMesh->Render();

	m_Shader->SetShader();

	m_Mesh->Render();
}

void CColliderPixel::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderPixel* CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

void CColliderPixel::Save(FILE* pFile)
{
	CColliderComponent::Save(pFile);

	fwrite(&m_Info, sizeof(sPixelInfo), 1, pFile);
}

void CColliderPixel::Load(FILE* pFile)
{
	CColliderComponent::Load(pFile);

	fread(&m_Info, sizeof(sPixelInfo), 1, pFile);
}

bool CColliderPixel::Collision(CColliderComponent* dest)
{
	switch (dest->GetColliderType())
	{
	case Collider_Type::Box2D:
		return CCollision::CollisionBox2DToPixel((CColliderBox2D*)dest, this);
	case Collider_Type::Circle:
		return CCollision::CollisionCircleToPixel((CColliderCircle*)dest, this);
	}

	return false;
}

bool CColliderPixel::CollisionMouse(const Vector2& mousePos)
{
	sCollisionResult result;

	m_MouseCollision = CCollision::CollisionPixelToPoint(m_MouseResult, result, m_Info, mousePos);

	return m_MouseCollision;
}
