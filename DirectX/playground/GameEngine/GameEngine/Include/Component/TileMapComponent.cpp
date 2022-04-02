#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "../Resource/Shader/TileMapConstantBuffer.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Engine.h"

CTileMapComponent::CTileMapComponent()
{
	SetTypeID<CTileMapComponent>();
	m_Render = true;

	m_CountX = 0;
	m_CountY = 0;
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_OffsetX = 0.0f;
	m_OffsetY = 0.0f;
	m_RenderCount = 0;
	m_TileShape = Tile_Shape::Rect;
	m_LayerName = "Back";

	m_TileInfoBuffer = nullptr;

	for (int i = 0; i < (int)Tile_Type::End; i++)
	{
		m_TileColor[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_TileColor[(int)Tile_Type::Wall] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_TileColor[(int)Tile_Type::Pilar] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_TileColor[(int)Tile_Type::SideWall] = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

	m_EditMode = false;
	m_SortDisable = true;
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& com) :
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

	if (com.m_BackMaterial)
		m_BackMaterial = com.m_BackMaterial->Clone();

	if (com.m_TileMaterial)
		m_TileMaterial = com.m_TileMaterial->Clone();

	if (com.m_TileCBuffer)
		m_TileCBuffer = com.m_TileCBuffer->Clone();

	if (com.m_TileInfoBuffer)
		m_TileInfoBuffer = com.m_TileInfoBuffer->Clone();

	for (int i = 0; i < (int)Tile_Type::End; i++)
	{
		m_TileColor[i] = com.m_TileColor[i];
	}

	m_EditMode = com.m_EditMode;
}

CTileMapComponent::~CTileMapComponent()
{
	SAFE_DELETE(m_TileInfoBuffer);
	SAFE_DELETE(m_TileCBuffer);

	size_t size = m_vecTile.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();
}

void CTileMapComponent::SetBackMaterial(CMaterial* material)
{
	m_BackMaterial = material->Clone();

	m_BackMaterial->SetScene(m_Scene);

	m_BackMaterial->SetRenderState("AlphaBlend");
}

void CTileMapComponent::SetTileMaterial(CMaterial* material)
{
	m_TileMaterial = material->Clone();

	m_TileMaterial->SetScene(m_Scene);

	if(m_TileMaterial->HasTexture())
		m_TileCBuffer->SetImageSize(Vector2((float)m_TileMaterial->GetTextureWidth(),
			(float)m_TileMaterial->GetTextureHeight()));
}

void CTileMapComponent::SetBackBaseColor(const Vector4& color)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetBaseColor(color);
}

void CTileMapComponent::SetBackBaseColor(float r, float g, float b, float a)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetBaseColor(r, g, b, a);
}

void CTileMapComponent::SetBackRenderState(CRenderState* state)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetRenderState(state);
}

void CTileMapComponent::SetBackRenderState(const std::string& name)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetRenderState(name);
}

void CTileMapComponent::SetBackTransparency(bool enable)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTransparency(enable);
}

void CTileMapComponent::SetBackOpacity(float opacity)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetOpacity(opacity);
}

void CTileMapComponent::AddBackOpacity(float opacity)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddOpacity(opacity);
}

void CTileMapComponent::AddBackTexture(int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(registerNum, shaderType, name, texture);
}

void CTileMapComponent::AddBackTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(registerNum, shaderType, name, fileName, pathName);
}

void CTileMapComponent::AddBackTextureFullPath(int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTextureFullPath(registerNum, shaderType, name, fullPath);
}

void CTileMapComponent::AddBackTexture(int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(registerNum, shaderType, name, vecFileName, pathName);
}

void CTileMapComponent::SetBackTexture(int index, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(index, registerNum, shaderType, name, texture);
}

void CTileMapComponent::SetBackTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(index, registerNum, shaderType, name, fileName, pathName);
}

void CTileMapComponent::SetBackTextureFullPath(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTextureFullPath(index, registerNum, shaderType, name, fullPath);
}

void CTileMapComponent::SetBackTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(index, registerNum, shaderType, name, vecFileName, pathName);
}

void CTileMapComponent::CreateTile(Tile_Shape shape, int countX, int countY, const Vector3& size)
{
	m_TileShape = shape;
	m_CountX = countX;
	m_CountY = countY;
	m_TileSize = size;

	size_t vecSize = m_vecTile.size();

	if (vecSize != 0)
	{
		for (size_t i = 0; i < vecSize; i++)
		{
			SAFE_DELETE(m_vecTile[i]);
		}

		m_vecTile.clear();
	}

	m_vecTile.resize(m_CountX * m_CountY);

	for (int i = 0; i < m_CountY; i++)
	{
		for (int j = 0; j < m_CountX; j++)
		{
			CTile* tile = new CTile;

			tile->m_Owner = this;

			int index = i * m_CountX + j;

			tile->SetIndex(j, i, index);
			tile->SetSize(m_TileSize);
			tile->SetShape(m_TileShape);

			m_vecTile[index] = tile;
		}
	}

	switch (m_TileShape)
	{
	case Tile_Shape::Rect:
	case Tile_Shape::RectWire:
	{
		Vector3 pos;

		for (int i = 0; i < m_CountY; i++)
		{
			pos.x = 0.0f;
			pos.y = i * m_TileSize.y;

			for (int j = 0; j < m_CountX; j++)
			{
				pos.x = j * m_TileSize.x;

				int index = i * m_CountX + j;

				m_vecTile[index]->SetPos(pos);
			}
		}
	}
		break;
	case Tile_Shape::Rhombus:
	{
		// 마름모의 경우 홀수 짝수 행에 따라 위치가 달라진다.

		Vector3 pos;

		for (int i = 0; i < m_CountY; i++)
		{
			if (i % 2 == 0)
				pos.x = 0.0f;
			// 홀수일 경우 X의 크기 반만큼 오른쪽으로 옮긴다.
			else
				pos.x = m_TileSize.x * 0.5f;
			
			// 마름모의 경우 높이는 Y의 절반만큼 올라간다.
			pos.y = i * m_TileSize.y * 0.5f;

			for (int j = 0; j < m_CountX; j++)
			{
				// 두번째 부터는 가로크기만큼 오프셋을 더해준다.
				if ((float)j > 0.0f)
					pos.x += m_TileSize.x;

				int index = i * m_CountX + j;

				m_vecTile[index]->SetPos(pos);
			}
		}
	}
		break;
	}

	m_TileCBuffer->SetTileSize(Vector2(m_TileSize.x, m_TileSize.y));

	if (m_TileShape == Tile_Shape::RectWire)
	{
		for (int i = 0; i < (int)Tile_Type::End; i++)
		{
			m_TileColor[i] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		}
	}
	else
	{
		for (int i = 0; i < (int)Tile_Type::End; i++)
		{
			m_TileColor[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	m_TileColor[(int)Tile_Type::Wall] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_TileColor[(int)Tile_Type::Pilar] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_TileColor[(int)Tile_Type::SideWall] = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

	m_Count = m_CountX * m_CountY;

	SetWorldInfo();

	if(m_Scene->GetNavigationManager())
		m_Scene->GetNavigationManager()->SetNavData(this);
}

void CTileMapComponent::SetTileDefaultFrame(const Vector2& start, const Vector2& end)
{
	for (int i = 0; i < m_Count; i++)
	{
		m_vecTile[i]->SetFrameStart(start);
		m_vecTile[i]->SetFrameEnd(end);
	}
}

void CTileMapComponent::SetTileDefaultFrame(float startX, float startY, float endX, float endY)
{
	for (int i = 0; i < m_Count; i++)
	{
		m_vecTile[i]->SetFrameStart(startX, startY);
		m_vecTile[i]->SetFrameEnd(endX, endY);
	}
}

void CTileMapComponent::SetTileFrame(int indexX, int indexY, float startX, float startY, float endX, float endY)
{
	m_vecTile[indexY * m_CountX + indexX]->SetFrameStart(startX, startY);
	m_vecTile[indexY * m_CountX + indexX]->SetFrameEnd(endX, endY);
}

void CTileMapComponent::SetTileFrame(int index, float startX, float startY, float endX, float endY)
{
	m_vecTile[index]->SetFrameStart(startX, startY);
	m_vecTile[index]->SetFrameEnd(endX, endY);
}

void CTileMapComponent::SetTileFrame(const Vector3& pos, float startX, float startY, float endX, float endY)
{
}

void CTileMapComponent::SetTileOpacity(int indexX, int indexY, float opacity)
{
	m_vecTile[indexY * m_CountX + indexX]->SetOpacity(opacity);
}

void CTileMapComponent::SetTileOpacity(int index, float opacity)
{
	m_vecTile[index]->SetOpacity(opacity);
}

void CTileMapComponent::SetTileOpacity(const Vector3& pos, float opacity)
{
}

void CTileMapComponent::SetTileColor(Tile_Type type, float r, float g, float b, float a)
{
	m_TileColor[(int)type] = Vector4(r, g, b, a);
}

void CTileMapComponent::SetTileColor(Tile_Type type, const Vector4& color)
{
	m_TileColor[(int)type] = color;
}

int CTileMapComponent::GetTileRenderIndexX(const Vector3& pos)
{
	if (m_TileShape == Tile_Shape::Rect || m_TileShape == Tile_Shape::RectWire)
	{
		float convertX = pos.x - GetWorldPos().x;

		int indexX = (int)(convertX / m_TileSize.x);

		if (indexX < 0)
			return 0;
		else if (indexX >= m_CountX)
			return m_CountX - 1;

		return indexX;
	}

	int indexY = GetTileRenderIndexY(pos);

	if (indexY < 0)
		indexY = 0;
	else if (indexY >= m_CountY)
		indexY = m_CountY - 1;

	int indexX = -1;

	float convertX = pos.x - GetWorldPos().x;

	// 짝수 라인의 경우 그대로 
	if (indexY % 2 == 0)
		indexX = (int)(convertX / m_TileSize.x);
	// 홀수 라인의 경우 0.5만큼 빼준다. 위에서 위치를 구할때는 더해줬으니 이번엔 뺀다.
	else
		indexX = (int)((convertX - m_TileSize.x * 0.5f) / m_TileSize.x);

	if (indexX < 0)
		return 0;
	else if (indexX >= m_CountX)
		return m_CountX - 1;

	return indexX;
}

int CTileMapComponent::GetTileRenderIndexY(const Vector3& pos)
{
	if (m_TileShape == Tile_Shape::Rect || m_TileShape == Tile_Shape::RectWire)
	{
		float convertY = pos.y - GetWorldPos().y;

		int indexY = (int)(convertY / m_TileSize.y);

		if (indexY < 0)
			return 0;
		else if (indexY >= m_CountY)
			return m_CountY - 1;

		return indexY;
	}

	// 먼저 위치 변환을 해준다.
	Vector3 convertPos = pos - GetWorldPos();

	// 현재 위치에서 타일의 크기만큼 0 ~ 1 사이의 좌표로 만들어준다.
	float ratioX = convertPos.x / m_TileSize.x;
	float ratioY = convertPos.y / m_TileSize.y;

	// 정수부분을 구한다.
	int indexX = (int)ratioX;
	int indexY = (int)ratioY;

	// 만약 0보다 작은 수거나 m_CountX보다 큰 수라면
	if (indexX < 0)
		indexX = 0;
	else if (indexX >= m_CountX)
		indexX = m_CountX - 1;

	// 정수 부분을 제거하여 소수점 부분만을 남겨준다.
	ratioX -= (int)ratioX;
	ratioY -= (int)ratioY;

	// 현재 위치를 변환하고 그 변환한 위치를 0 ~ 1 사이의 값으로 바꿔줬다.
	// 따라서 ratioY가 0.5보다 작다면 현재 마름모 타일안에서 아래 부분에 위치해있다는 뜻
	// 사각형의 아래쪽 부분일 경우
	if (ratioY < 0.5f)
	{
		// ratioX가 0.5보다 크면 오른쪽 하단 사각형이 되는데 이 경우 0.5에서 빼면 음수가 나온다. 따라서
		// ratioY는 절대로 이 값보다 작을 수 없다. 즉 아래의 식은 좌 하단 사각형일 경우에만 성립이 가능하다.
		// 좌 하단 사각형에서 좌 하단 삼각형
		if (ratioY < 0.5f - ratioX)
		{
			// 좌측 사각형들의 좌 하단 삼각형은 비어있는 공간이다.
			if (indexX == 0)
			{
				if (indexY < 0)
					return 0;
				else if (indexY >= m_CountY)
					return m_CountY - 1;
				else
					return indexY * 2 + 1;
			}
			// 가장 아래 사각형들의 좌 하단 사각형의 좌 하단 삼각형은 비어있는 공간이다.
			else if (indexY == 0)
				return 0;
			else if (indexY == 1)
				return 1;
			else
				return indexY * 2 - 1;
		}
		// 우 하단 사각형의 우 하단 삼각형일 경우
		else if (ratioY < ratioX - 0.5f)
		{
			if (indexY == 0)
				return 0;
			else if (indexY == 1)
				return 1;
			else
				return indexY * 2 - 1;
		}
		// 하단 삼각형 안에 존재할 경우(마름모 안에 위치)
		else
			return indexY * 2;
	}
	// 사각형의 위쪽 부분일 경우
	else if (ratioY > 0.5f)
	{
		// 좌 상단 사각형의 좌 상단 삼각형
		if (ratioY - 0.5f > ratioX)
		{
			if (indexX == 0)
			{
				if (indexY < 0)
					return 0;
				else if (indexY >= m_CountY)
					return m_CountY - 1;
			}

			if (indexY * 2 + 1 >= m_CountY)
				return m_CountY - 1;

			return indexY * 2 + 1;
		}

		// 우 상단 사각형의 우 상단 삼각형일 경우
		else if (ratioY - 0.5f > 1.0f - ratioX)
		{
			if (indexX >= m_CountX)
			{
				if (indexY < 0)
					return indexY;
				else if (indexY >= m_CountY)
					return m_CountY - 1;
			}

			if (indexY * 2 + 1 >= m_CountY)
				return m_CountY - 1;

			return indexY * 2 + 1;
		}
		else
			return indexY * 2;
	}
	else
		return indexY * 2;

	return -1;
}

int CTileMapComponent::GetTileIndexX(const Vector3& pos)
{
	if (m_TileShape == Tile_Shape::Rect || m_TileShape == Tile_Shape::RectWire)
	{
		float convertX = pos.x - GetWorldPos().x;

		if (convertX < 0.0f)
			return -1;

		int indexX = (int)(convertX / m_TileSize.x);

		if (indexX < 0 || indexX >= m_CountX)
			return -1;

		return indexX;
	}

	int indexY = GetTileIndexY(pos);

	if (indexY < 0 || indexY >= m_CountY)
		return -1;

	int indexX = -1;

	float convertX = pos.x - GetWorldPos().x;

	if (indexY % 2 == 0)
		indexX = (int)(convertX / m_TileSize.x);
	else
		indexX = (int)((convertX - m_TileSize.x * 0.5f) / m_TileSize.x);

	if (indexX < 0 || indexX >= m_CountX)
		return -1;

	return indexX;
}

int CTileMapComponent::GetTileIndexY(const Vector3& pos)
{
	if (m_TileShape == Tile_Shape::Rect || m_TileShape == Tile_Shape::RectWire)
	{
		float convertY = pos.y - GetWorldPos().y;

		if (convertY < 0.0f)
			return -1;

		int indexY = (int)(convertY / m_TileSize.y);

		if (indexY < 0 || indexY >= m_CountY)
			return -1;

		return indexY;
	}

	Vector3 convertPos = pos - GetWorldPos();

	float ratioX = convertPos.x / m_TileSize.x;
	float ratioY = convertPos.y / m_TileSize.y;

	int indexX = (int)ratioX;
	int indexY = (int)ratioY;

	if (indexX < 0 || indexX > m_CountX)
		return -1;

	ratioX -= (int)ratioX;
	ratioY -= (int)ratioY;

	if (ratioY < 0.5f)
	{
		if (ratioY < 0.5f - ratioX)
		{
			if (indexX == 0)
				return -1;
			else if (indexY == 0)
				return -1;
			else if (indexY == 1)
				return 1;
			else
				return indexY * 2 - 1;
		}
		else if (ratioY < ratioX - 0.5f)
		{
			if (indexY == 0)
				return -1;
			else if (indexY == 1)
				return 1;
			else
				return indexY * 2 - 1;
		}
		else
			return indexY * 2;
	}
	else if (ratioY > 0.5f)
	{
		if (ratioY - 0.5f > ratioX)
		{
			if (indexX == 0)
				return -1;

			if (indexY * 2 + 1 >= m_CountY)
				return -1;

			return indexY * 2 + 1;
		}
		else if (ratioY - 0.5f > 1.0f - ratioX)
		{
			if (indexX >= m_CountX)
				return -1;

			if (indexY * 2 + 1 >= m_CountY)
				return -1;

			return indexY * 2 + 1;
		}
		else
			return indexY * 2;
	}
	else
		return indexY * 2;

	return -1;
}

int CTileMapComponent::GetTileIndex(const Vector3& pos)
{
	int indexX = GetTileIndexX(pos);
	int indexY = GetTileIndexY(pos);

	if (indexX == -1 || indexY == -1)
		return -1;

	return indexY * m_CountX + indexX;
}

CTile* CTileMapComponent::GetTile(const Vector3& pos)
{
	int index = GetTileIndex(pos);

	if (index == -1)
		return nullptr;

	return m_vecTile[index];
}

CTile* CTileMapComponent::GetTile(int x, int y)
{
	if (x < 0 || x >= m_CountX || y < 0 || y >= m_CountY)
		return nullptr;

	return m_vecTile[y * m_CountX + x];
}

CTile* CTileMapComponent::GetTile(int index)
{
	if (index < 0 || index >= m_CountX * m_CountY)
		return nullptr;

	return m_vecTile[index];
}

void CTileMapComponent::Start()
{
	CSceneComponent::Start();

	size_t size = m_vecTile.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecTile[i]->Start();
	}
}

bool CTileMapComponent::Init()
{
	m_Mesh = (CSpriteMesh*)m_Scene->GetSceneResource()->FindMesh("SpriteMesh");
	m_WireMesh = (CMesh*)m_Scene->GetSceneResource()->FindMesh("FrameRect");

	SetMeshSize(1.0f, 1.0f, 0.0f);
	// SetWorldScale((float)m_BackMaterial->GetTextureWidth(),
	// 	(float)m_BackMaterial->GetTextureHeight(), 1.0f);

	m_TileCBuffer = new CTileMapConstantBuffer;

	m_TileCBuffer->Init();

	return true;
}

void CTileMapComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (m_TileMaterial)
	{
		if (m_TileMaterial->HasTexture())
			m_TileCBuffer->SetImageSize(Vector2((float)m_TileMaterial->GetTextureWidth(), (float)m_TileMaterial->GetTextureHeight()));

		if (m_TileShape == Tile_Shape::RectWire)
		{
			m_TileCBuffer->SetWire(true);
			m_TileCBuffer->SetImageSize(Vector2(1.0f, 1.0f));
		}
		else
			m_TileCBuffer->SetWire(false);
	}
}

void CTileMapComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();

	if (m_vecTile.empty())
		return;

	CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	sResolution rs = camera->GetResolution();

	// 좌하단과 우상단의 위치를 구한다.
	Vector3 LB = camera->GetWorldPos();
	Vector3 RT = LB + Vector3((float)rs.width, (float)rs.height, 0.0f);

	int startX, startY, endX, endY;

	startX = GetTileRenderIndexX(LB);
	startY = GetTileRenderIndexY(LB);

	endX = GetTileRenderIndexX(RT);
	endY = GetTileRenderIndexY(RT);

	if (m_TileShape == Tile_Shape::Rhombus)
	{
		startX--;
		startY--;

		endX++;
		endY++;

		startX = startX < 0 ? 0 : startX;
		startY = startY < 0 ? 0 : startY;

		endX = endX >= m_CountX ? m_CountX - 1 : endX;
		endY = endY >= m_CountY ? m_CountY - 1 : endY;
	}

	Matrix matView, matProj;
	matView = camera->GetViewMatrix();
	matProj = camera->GetProjMatrix();

	// 전체 렌더링 될 타일의 갯수를 구한다.
	m_RenderCount = 0;

	// 카메라에 보이는 영역만 업데이트 루프를 돌린다.
	// 구조화 버퍼에 담아서 인스턴싱을 한다.
	for (int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			int index = i * m_CountX + j;

			m_vecTile[index]->Update(CEngine::GetInst()->GetDeltaTime(), m_SortDisable);

			if (m_vecTile[index]->GetRender())
			{
				if (m_EditMode)
				{
					m_vecTileInfo[m_RenderCount].tileColor = m_TileColor[(int)m_vecTile[index]->GetTileType()];
				}

				if (m_TileShape != Tile_Shape::RectWire)
				{
					m_vecTileInfo[m_RenderCount].tileStart = m_vecTile[index]->GetFrameStart();
					m_vecTileInfo[m_RenderCount].tileEnd = m_vecTile[index]->GetFrameEnd();
				}
				else
				{
					m_vecTileInfo[m_RenderCount].tileStart = Vector2(1.0f, 1.0f);
					m_vecTileInfo[m_RenderCount].tileEnd = Vector2(1.0f, 1.0f);
				}
					
				m_vecTileInfo[m_RenderCount].opacity = m_vecTile[index]->GetOpacity();	
				m_vecTileInfo[m_RenderCount].matWVP = m_vecTile[index]->GetWorldMatrix() * matView * matProj;
				m_vecTileInfo[m_RenderCount].matWVP.Transpose();
				m_RenderCount++;
			}
		}
	}

	m_TileInfoBuffer->UpdateBuffer(&m_vecTileInfo[0], m_RenderCount);
}

void CTileMapComponent::Render()
{
	CSceneComponent::Render();
	
	if (!m_vecTile.empty() && m_TileMaterial)
	{
		SetWorldScale(1.0f, 1.0f, 1.0f);

		// PostUpdate에서 계산한 구조화 버퍼를 쉐이더로 보낸다.
		m_TileInfoBuffer->SetShader();

		m_TileCBuffer->UpdateCBuffer();

		m_TileMaterial->Render();

		if (m_TileShape == Tile_Shape::Rect || m_TileShape == Tile_Shape::Rhombus)
			m_Mesh->RenderInstancing(m_RenderCount);
		else if(m_EditMode)
			m_WireMesh->RenderInstancing(m_RenderCount);

		m_TileMaterial->Reset();

		m_TileInfoBuffer->ResetShader();
	}
	

	if (m_BackMaterial)
	{
		SetWorldScale((float)m_BackMaterial->GetTextureWidth() * m_ScaleX, (float)m_BackMaterial->GetTextureHeight() * m_ScaleY, 1.0f);

		if (m_EditMode)
			m_BackMaterial->SetOpacity(0.5f);
		else
			m_BackMaterial->SetOpacity(1.0f);

		m_BackMaterial->Render();

		m_Mesh->Render();

		m_BackMaterial->Reset();
	}
}

void CTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileMapComponent* CTileMapComponent::Clone()
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::Save(FILE* pFile)
{
	std::string meshName = m_Mesh->GetName();

	int length = (int)meshName.length();

	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(meshName.c_str(), sizeof(char), length, pFile);

	meshName = m_WireMesh->GetName();

	length = (int)meshName.length();

	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(meshName.c_str(), sizeof(char), length, pFile);

	bool hasBackMat = false;

	if (m_BackMaterial)
		hasBackMat = true;

	fwrite(&hasBackMat, sizeof(bool), 1, pFile);

	if (hasBackMat)
	{
		fwrite(&m_ScaleX, sizeof(float), 1, pFile);
		fwrite(&m_ScaleY, sizeof(float), 1, pFile);
		m_BackMaterial->Save(pFile);
	}

	bool hasTileMat = false;

	if (m_TileMaterial)
		hasTileMat = true;

	fwrite(&hasTileMat, sizeof(bool), 1, pFile);

	if(hasTileMat)
		m_TileMaterial->Save(pFile);

	fwrite(&m_TileShape, sizeof(Tile_Shape), 1, pFile);
	fwrite(&m_CountX, sizeof(int), 1, pFile);
	fwrite(&m_CountY, sizeof(int), 1, pFile);
	fwrite(&m_Count, sizeof(int), 1, pFile);
	fwrite(&m_OffsetX, sizeof(float), 1, pFile);
	fwrite(&m_OffsetY, sizeof(float), 1, pFile);
	fwrite(&m_TileSize, sizeof(Vector3), 1, pFile);
	fwrite(m_TileColor, sizeof(Vector4), (int)Tile_Type::End, pFile);

	for (int i = 0; i < m_Count; i++)
	{
		m_vecTile[i]->Save(pFile);
	}

	CSceneComponent::Save(pFile);
}

void CTileMapComponent::Load(FILE* pFile)
{
	char meshName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, pFile);
	fread(meshName, sizeof(char), length, pFile);

	m_Mesh = (CSpriteMesh*)m_Scene->GetSceneResource()->FindMesh(meshName);

	memset(meshName, 0, sizeof(char) * 256);

	fread(&length, sizeof(int), 1, pFile);
	fread(meshName, sizeof(char), length, pFile);

	m_WireMesh = (CMesh*)m_Scene->GetSceneResource()->FindMesh(meshName);

	bool hasBackMat = false;

	fread(&hasBackMat, sizeof(bool), 1, pFile);

	if (hasBackMat)
	{
		m_BackMaterial = m_Scene->GetSceneResource()->CreateMaterialEmpty<CMaterial>();

		fread(&m_ScaleX, sizeof(float), 1, pFile);
		fread(&m_ScaleY, sizeof(float), 1, pFile);

		m_BackMaterial->Load(pFile);
	}

	bool hasTileMat = false;

	fread(&hasTileMat, sizeof(bool), 1, pFile);

	if (hasTileMat)
	{
		m_TileMaterial = m_Scene->GetSceneResource()->CreateMaterialEmpty<CMaterial>();

		m_TileMaterial->Load(pFile);
	}

	fread(&m_TileShape, sizeof(Tile_Shape), 1, pFile);
	fread(&m_CountX, sizeof(int), 1, pFile);
	fread(&m_CountY, sizeof(int), 1, pFile);
	fread(&m_Count, sizeof(int), 1, pFile);
	fread(&m_OffsetX, sizeof(float), 1, pFile);
	fread(&m_OffsetY, sizeof(float), 1, pFile);
	fread(&m_TileSize, sizeof(Vector3), 1, pFile);
	fread(m_TileColor, sizeof(Vector4), (int)Tile_Type::End, pFile);

	size_t size = m_vecTile.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	m_vecTile.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		CTile* tile = new CTile;

		tile->m_Owner = this;

		tile->Load(pFile);

		m_vecTile[i] = tile;
	}

	SetOffsetX(m_OffsetX);
	SetOffsetY(m_OffsetY);


	SAFE_DELETE(m_TileCBuffer);

	m_TileCBuffer = new CTileMapConstantBuffer;

	m_TileCBuffer->Init();

	SetWorldInfo();

	if (m_Scene->GetNavigationManager())
		m_Scene->GetNavigationManager()->SetNavData(this);

	CSceneComponent::Load(pFile);
}

void CTileMapComponent::SetWorldInfo()
{
	SAFE_DELETE(m_TileInfoBuffer);

	m_TileInfoBuffer = new CStructuredBuffer;

	m_TileInfoBuffer->Init("TileInfo", sizeof(sTileInfo), m_CountX * m_CountY, 40, true, (int)Buffer_Shader_Type::Vertex);

	m_vecTileInfo.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		m_vecTileInfo[i].tileColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vecTileInfo[i].opacity = 1.0f;
	}
}
