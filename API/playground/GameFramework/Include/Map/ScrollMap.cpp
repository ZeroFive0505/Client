#include "ScrollMap.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CScrollMap::CScrollMap() :
	m_ScrollRatio(1.0f, 1.0f),
	m_Loop(false)
{
}

CScrollMap::~CScrollMap()
{
}

void CScrollMap::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	SetTexture(Name);
}

void CScrollMap::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	SetTexture(Name);
}

void CScrollMap::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	SetTexture(Name);
}

void CScrollMap::SetTexture(const std::string& Name)
{
	m_ScrollTexture = m_Scene->GetSceneResource()->FindTexture(Name);
}

void CScrollMap::SetTextureColorKey(const unsigned char r, const unsigned char g, const unsigned char b)
{
	if (m_ScrollTexture)
		m_ScrollTexture->SetColorKey(r, g, b);
}

void CScrollMap::Start()
{
	CMapBase::Start();

	if (m_ScrollTexture)
		m_TextureSize = Vector2((float)m_ScrollTexture->GetWidth(), (float)m_ScrollTexture->GetHeight());
}

bool CScrollMap::Init()
{
	CMapBase::Init();

	return true;
}

void CScrollMap::Update(float DeltaTile)
{
	CMapBase::Update(DeltaTile);
}

void CScrollMap::PostUpdate(float DeltaTile)
{
	CMapBase::PostUpdate(DeltaTile);
}

void CScrollMap::PrevRender()
{
	CMapBase::PrevRender();
}

void CScrollMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);

	if (m_ScrollTexture)
	{
		if (!m_Loop)
		{
			CCamera* Camera = m_Scene->GetCamera();

			Vector2 Resolution = Camera->GetResolution();

			Vector2 ImagePos = Camera->GetPos() * m_ScrollRatio;

			unsigned int Width = m_ScrollTexture->GetWidth();
			unsigned int Height = m_ScrollTexture->GetHeight();

			ImagePos.x = ImagePos.x < 0.0f ? 0.0f : ImagePos.x;
			ImagePos.x = ImagePos.x + Resolution.x > Width ? Width - Resolution.x : ImagePos.x;

			ImagePos.y = ImagePos.y < 0.0f ? 0.0f : ImagePos.y;
			ImagePos.y = ImagePos.y + Resolution.y > Height ? Height - Resolution.y : ImagePos.y;

			m_ScrollTexture->Render(hDC, m_Pos, ImagePos, m_Size);
		}
		else
		{
			CCamera* Camera = m_Scene->GetCamera();

			Vector2 CameraPos = Camera->GetPos();

			Vector2 Resolution = Camera->GetResolution();

			Vector2 ImagePos = Camera->GetPos() * m_ScrollRatio;

			unsigned int Width = m_ScrollTexture->GetWidth();
			unsigned int Height = m_ScrollTexture->GetHeight();

			// �»��, ����, ���ϴ�, ���ϴ� ���
			unsigned int WidthRatio = (unsigned int)(CameraPos.x / Width);
			unsigned int HeigthRatio = (unsigned int)(CameraPos.y / Height);

			Vector2 ConvertPos;
			ConvertPos.x = CameraPos.x - WidthRatio * Width;
			ConvertPos.y = CameraPos.y - HeigthRatio * Height;

			Vector2 ConvertLeftTopSize;
			ConvertLeftTopSize.x = Width - ConvertPos.x;
			ConvertLeftTopSize.y = Height - ConvertPos.y;

			// �� ��� ���
			// ConvertPos = (x, y)
			// ���� �»���� ��쿡�� x, y��ǥ�� �״�� ��Ƴ��� �̹����� ��ǥ�� �ȴ�.
			// ������â���� �̹��� �����ġ�� 0, 0���� ����Ѵ�.
			m_ScrollTexture->Render(hDC, Vector2(0.0f, 0.0f), ConvertPos, ConvertLeftTopSize);

			// �� ��� ���
			// �� ����� ��� x������ ���̰� �Ѿ���ȴ�.
			// ���� ConvertPos�� x�� ���� �ٽ� 0���� �ʱ�ȭ�ȴٰ� �����Ѵ�.
			// �̹����� �����ġ�� ConvertLeftTopSize.x ��ŭ ���������� �̵���Ű��
			// �̹����� ��ǥ�� x�� 0, y�� ��� ��ü ���̿��� ConvertLeftTopSize.y������ ��Ȯ�� ��ǥ�� ���Ѵ�.
			// ����� ũ��� x�� ��� ȭ���� �Ѿ���� ����̴� ��ü ī�޶� �ػ󵵿��� ConvertLeftTopSize.x ��ŭ ���ش�.
			// y�� ��� �� ��ܰ� ���� ������ ũ�Ⱑ �ȴ�.
			// ImgPos�� ��� �̹����� ��ǥ�̴� Height(�̹����� ���� ���� �̿��ؼ� ���)
			// ���������� �������� ���� ��� ȭ�鿡 ����� �Ǵ� ���̴� ȭ���� �ػ� �� ī�޶��� �ػ󵵸� �̿��ؼ� ũ�⸦ ����Ѵ�.
			m_ScrollTexture->Render(hDC, Vector2(ConvertLeftTopSize.x, 0.0f), 
				Vector2(0.0f, Height - ConvertLeftTopSize.y),
				Vector2(Resolution.x - ConvertLeftTopSize.x, ConvertLeftTopSize.y));

			// �� �ϴ� ���
			// �� �ϴ��� ��� y�� ���� �ʱ�ȭ�ȴ�.
			// ������󿡼� �̹��� �����ġ�� �� ����� �̹���ũ�⸸ŭ ���� �����ϸ�
			// �̹����� ��ǥ�� x�� ���� �� ����� ���� �״�� �̿��ϰ� y�� ���� 0���� �ʱ�ȭ�Ѵ�.
			// �̹����� x ũ��� �� ����� ũ�⸸ŭ ����ϰ� y�� ��� ��ü �ػ󵵿��� ConvertLeftTopSize.y��ŭ�� �����Ѵ�.
			m_ScrollTexture->Render(hDC, Vector2(0.0f, ConvertLeftTopSize.y),
				Vector2(ConvertPos.x, 0.0f),
				Vector2(ConvertLeftTopSize.x, Resolution.y - ConvertLeftTopSize.y));


			// �� �ϴ� ���
			// �� �ϴ��� ��� x, y���� ��� ������ ��� ����̴�.
			// ������󿡼� �����ġ�� ConverLeftTopSize�̸�
			// �̹����󿡼� ��ǥ�� x, y ��� 0���� �ʱ�ȭ�Ѵ�.
			// ����� ��� ī�޶��� �ػ󵵿��� ConvertLeftSize��ŭ�� �����Ѵ�.
			m_ScrollTexture->Render(hDC, Vector2(ConvertLeftTopSize.x, ConvertLeftTopSize.y),
				Vector2(0.0f, 0.0f), 
				Vector2(Resolution.x - ConvertLeftTopSize.x, Resolution.y - ConvertLeftTopSize.y));
		}
	}
}

void CScrollMap::Save(FILE* pFile)
{
}

void CScrollMap::Load(FILE* pFile)
{
}
