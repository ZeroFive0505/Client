#include "Mode7Demo.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../UI/Mode7UI.h"
#include "Camera.h"
#include "../Input.h"

CMode7Demo::CMode7Demo() :
    m_Start(false),
    m_CircuitTexture(nullptr),
    m_EdgeTexture(nullptr),
    m_Observer(nullptr),
    m_UI(nullptr),
    m_Perspective(true)
{
}

CMode7Demo::~CMode7Demo()
{
}

bool CMode7Demo::Init()
{
    m_Observer = CreateObject<CPlayer>("Observer");

    m_Observer->Init();

    m_Observer->SetPos(0.0f, 0.0f);

    m_Observer->SetAngle(0.0f);

    GetSceneResoruce()->LoadTexture("Circuit", TEXT("Course/Mushroom/Mario circuit2/circuit.bmp"));
    m_CircuitTexture = GetSceneResoruce()->FindTexture("Circuit");
    m_CircuitTexture->SetPixelArray();

    GetSceneResoruce()->LoadTexture("Edge", TEXT("Course/Mushroom/Mario circuit2/edge.bmp"));
    m_EdgeTexture = GetSceneResoruce()->FindTexture("Edge");
    m_EdgeTexture->SetPixelArray();

    m_UI = CreateUIWindow<CMode7UI>("UI");

    m_UI->Init();

    return true;
}

bool CMode7Demo::Render(HDC hDC)
{
    CTexture circuitTexture;

    SetCircuitTexture(m_Observer, circuitTexture);

    circuitTexture.Render(hDC, Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(circuitTexture.GetWidth(), circuitTexture.GetHeight()));

    CScene::Render(hDC);

    return false;
}

bool CMode7Demo::Update(float deltaTime)
{
    if (!m_Start)
    {
        m_Start = true;
        
        CInput::GetInst()->SetMode7DemoKeySet();

        m_Observer->Start();

        CInput::GetInst()->SetCallback<CMode7Demo>("Increase far", KeyState_Hold, this, &CMode7Demo::IncreaseFar);
        CInput::GetInst()->SetCallback<CMode7Demo>("Decrease far", KeyState_Hold, this, &CMode7Demo::DecreaseFar);
        
        CInput::GetInst()->SetCallback<CMode7Demo>("Increase near", KeyState_Hold, this, &CMode7Demo::IncreaseNear);
        CInput::GetInst()->SetCallback<CMode7Demo>("Decrease near", KeyState_Hold, this, &CMode7Demo::DecreaseNear);

        CInput::GetInst()->SetCallback<CMode7Demo>("Increase FOV", KeyState_Hold, this, &CMode7Demo::IncreaseFOV);
        CInput::GetInst()->SetCallback<CMode7Demo>("Decrease FOV", KeyState_Hold, this, &CMode7Demo::DecreaseFOV);

        CInput::GetInst()->SetCallback<CMode7Demo>("BackToTitle", KeyState_Down, this, &CMode7Demo::BackToTitle);

        CInput::GetInst()->SetCallback<CMode7Demo>("Toggle perspective", KeyState_Down, this, &CMode7Demo::TogglePerspective);
    }


    CScene::Update(deltaTime);

    return false;
}

void CMode7Demo::Mode7(const Vector2& cameraPos, const float angle, const float fovHalf, const float clipNear, const float clipFar, const Vector2& circuitSize, const bool perspective, CTexture& sampleMap)
{
    // ���� �������� fov�� ���� ���� ���� ��ŭ�� ��ġ�� �������� ����Ʈ�� ��´�.
    float fovMin = angle - fovHalf;
    float fovMax = angle + fovHalf;

    // fovMin���� �̿��Ͽ� ���������� ������ ��ǥ��
    Vector2 trigMin = Vector2(cosf(fovMin), sinf(fovMin));

    // fovMax���� �̿��Ͽ� ���������� ������ ��ǥ��
    Vector2 trigMax = Vector2(cosf(fovMax), sinf(fovMax));


    // far ����Ʈ�� �� ���� clipFar�� ���ؼ� ���Ѵ�.
    Vector2 far1 = cameraPos + trigMin * clipFar;
    Vector2 far2 = cameraPos + trigMax * clipFar;

    // near ����Ʈ�� �� ���� clipNear�� ���ؼ� ���Ѵ�.
    Vector2 near1 = cameraPos + trigMin * clipNear;
    Vector2 near2 = cameraPos + trigMax * clipNear;

    // ���� �ʺ�� ����
    int width = (int)circuitSize.x;
    int height = (int)circuitSize.y;

    COLORREF* sampleColorArray = new COLORREF[width * height];

    for (int y = 0; y < height; y++)
    {
        // �� ȭ���� ���ø� �Ѵ�.
        float sampleDepth = (float)y / (float)height;

        Vector2 start, end;

        // ���� ���̰��� ���ϰ� �ʹٸ�
        if (perspective)
        {
            // sampleDepth ������ �����ش�.
            // sampleDpeth���� 0�� ����� ������ 1�� ����� ������ �þ��.
            // �÷��̾�� �������ִ� ���� 0�� ����� ���� �����Ƿ�
            // �� ������ ������ start�� end�� ���� �� Ŀ���� �� ��
            // ���� ���ٰ��� ������ �� �ִ�.
            start = near1 + (far1 - near1) / sampleDepth;
            end = near2 + (far2 - near2) / sampleDepth;
        }
        else
        {
            // ���ٰ��� �ʿ�ġ ���� �� ����Ѵ�. 
            start = near1 + (far1 - near1) * (1.0f - sampleDepth);
            end = near2 + (far2 - near2) * (1.0f - sampleDepth);
        }

        for (int x = 0; x < width; x++)
        {
            float sampleWidth = (float)x / (float)width;

            // ������������ ���ؼ� ��ǥ�� ���Ѵ�.
            Vector2 sample = start + (end - start) * sampleWidth;
            COLORREF color;

            // ���� 0 ~ 1�������̶�� �ʵ� ��
            if (0.0f <= sample.x && sample.x <= 1.0f && 0.0f <= sample.y && sample.y <= 1.0f)
            {
                color = SampleMap(sample);
            }
            // �ƴ϶�� �ٱ�
            else
            {
                // ���� �ܰ� �ؽ����� ũ�⸦ �������� ���� ��ǥ�� ���Ѵ�.
                // ���� �������� ���ϰ�
                sample.x = fmodf(sample.x, MAP_EDGES_SIZE / (float)MAP_ASSETS_WIDTH);
                sample.y = fmodf(sample.y, MAP_EDGES_SIZE / (float)MAP_ASSETS_HEIGHT);

                // ���� 0���� �۴ٸ� �����ش�.
                if (sample.x < 0.0f)
                    sample.x += MAP_EDGES_SIZE / (float)MAP_ASSETS_WIDTH;

                if (sample.y < 0.0f)
                    sample.y += MAP_EDGES_SIZE / (float)MAP_ASSETS_HEIGHT;


                sample.x *= MAP_ASSETS_WIDTH / (float)MAP_EDGES_SIZE;
                sample.y *= MAP_ASSETS_HEIGHT / (float)MAP_EDGES_SIZE;

                color = SampleEdge(sample);
            }

            sampleColorArray[y * width + x] = color;
        }
    }

    sampleMap.Create(circuitSize, sampleColorArray);

    SAFE_DELETE_ARRAY(sampleColorArray);
}

COLORREF CMode7Demo::SampleMap(const Vector2& pos)
{
    float xSize = (float)m_CircuitTexture->GetWidth();
    float ySize = (float)m_CircuitTexture->GetHeight();

    unsigned int px = min(m_CircuitTexture->GetWidth() - 1, (unsigned int)(pos.x * xSize));
    unsigned int py = min(m_CircuitTexture->GetHeight() - 1, (unsigned int)(pos.y * ySize));

    return m_CircuitTexture->PixelArrayAt(px, py);
}

COLORREF CMode7Demo::SampleMap(unsigned int x, unsigned int y)
{
    float xSize = (float)m_CircuitTexture->GetWidth();
    float ySize = (float)m_CircuitTexture->GetHeight();

    unsigned int px = min(m_CircuitTexture->GetWidth() - 1, (unsigned int)(x * xSize));
    unsigned int py = min(m_CircuitTexture->GetHeight() - 1, (unsigned int)(y * ySize));

    return m_CircuitTexture->PixelArrayAt(px, py);
}

COLORREF CMode7Demo::SampleEdge(const Vector2& pos)
{
    float xSize = (float)m_EdgeTexture->GetWidth();
    float ySize = (float)m_EdgeTexture->GetHeight();

    unsigned int px = min(m_EdgeTexture->GetWidth() - 1, (unsigned int)(pos.x * xSize));
    unsigned int py = min(m_EdgeTexture->GetHeight() - 1, (unsigned int)(pos.y * ySize));

    return m_EdgeTexture->PixelArrayAt(px, py);
}

COLORREF CMode7Demo::SampleEdge(unsigned int x, unsigned int y)
{
    float xSize = (float)m_EdgeTexture->GetWidth();
    float ySize = (float)m_EdgeTexture->GetHeight();

    unsigned int px = min(m_EdgeTexture->GetWidth() - 1, (unsigned int)(x * xSize));
    unsigned int py = min(m_EdgeTexture->GetHeight() - 1, (unsigned int)(y * ySize));

    return m_EdgeTexture->PixelArrayAt(px, py);
}

void CMode7Demo::SetCircuitTexture(CSharedPtr<CPlayer> player, CTexture& circuitTexture)
{
    float camX = player->GetPos().x - cosf(player->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_WIDTH);
    float camY = player->GetPos().y - sinf(player->GetAngle()) * (CAM_TO_PLAYER_DIST / MAP_ASSETS_HEIGHT);

    GetCamera()->SetPos(camX, camY);

    Vector2 circuitSize = Vector2(BASIC_WIDTH, BASIC_HEIGHT);

    Mode7(GetCamera()->GetPos(), player->GetAngle(), MODE7_FOV_HALF, MODE7_CLIP_NEAR, MODE7_CLIP_FAR, circuitSize, m_Perspective, circuitTexture);
}

void CMode7Demo::IncreaseFar(float deltaTime)
{
    MODE7_CLIP_FAR += 0.01f;
}

void CMode7Demo::DecreaseFar(float deltaTime)
{
    MODE7_CLIP_FAR -= 0.01f;
}

void CMode7Demo::IncreaseNear(float deltaTime)
{
    MODE7_CLIP_NEAR += 0.01f;
}

void CMode7Demo::DecreaseNear(float dletaTime)
{
    MODE7_CLIP_NEAR -= 0.01f;
}

void CMode7Demo::IncreaseFOV(float deltaTime)
{
    MODE7_FOV_HALF += 0.01f;
}

void CMode7Demo::DecreaseFOV(float deltaTime)
{
    MODE7_FOV_HALF -= 0.01f;
}

void CMode7Demo::BackToTitle(float deltaTime)
{
    CSceneManager::GetInst()->CreateScene<CTitleScene>();
}

void CMode7Demo::TogglePerspective(float deltaTime)
{
    m_Perspective = !m_Perspective;
}
