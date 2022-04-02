#include "IMGUIManager.h"
#include "IMGUIWindow.h"
#include "IMGUITestWindow.h"
#include "Device.h"
#include "PathManager.h"

DEFINITION_SINGLE(CIMGUIManager)

CIMGUIManager::CIMGUIManager() :
	m_Context(nullptr),
	m_CurrentFont(nullptr)
{
}

CIMGUIManager::~CIMGUIManager()
{
	auto iter = m_mapWindow.begin();
	auto iterEnd = m_mapWindow.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool CIMGUIManager::Init(HWND hWnd)
{
	m_Context = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	// Baekend setup
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());


	AddFont("Default", "NotoSansKR-Regular.otf", 15.f, true);
	AddFont("DefaultBlack", "NotoSansKR-Black.otf", 15.f, true);
	AddFont("DefaultBold", "NotoSansKR-Bold.otf", 15.f, true);

	m_CurrentFont = FindFont("DefaultBold");

	// CIMGUITestWindow* window = AddWindow<CIMGUITestWindow>("TestWindow");

	return true;
}

void CIMGUIManager::Update(float deltaTime)
{
	if (m_mapWindow.empty())
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// ImGui::PushFont(m_CurrentFont);
	
	auto iter = m_mapWindow.begin();
	auto iterEnd = m_mapWindow.end();

	for (; iter != iterEnd; iter++)
	{
		iter->second->Update(deltaTime);
	}

	// ImGui::PopFont();

	// Window에서 그려낸 위젯들은 실제 화면에 그려지는 것이 아니라
	// IMGUI의 내부 버퍼에 그려지게 된다.
	// 이 버퍼를 최종적으로 한번 그려내는 방식으로 처리가 된다.
	ImGui::Render();
}

void CIMGUIManager::Render()
{
	if (m_mapWindow.empty())
		return;

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

CIMGUIWindow* CIMGUIManager::FindIMGUIWindow(const std::string& name)
{
	auto iter = m_mapWindow.find(name);

	if (iter == m_mapWindow.end())
		return nullptr;

	return iter->second;
}

bool CIMGUIManager::AddFont(const std::string& name, const char* fileName, float size, bool korean, int overH, int overV, float spacing, const std::string& pathName)
{
	ImFont* font = FindFont(name);

	if (font)
		return false;

	char fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		strcpy_s(fullPath, info->pathMultiByte);

	strcat_s(fullPath, fileName);

	return AddFontFullPath(name, fullPath, size, korean, overH, overV, spacing);
}

bool CIMGUIManager::AddFontFullPath(const std::string& name, const char* fullPath, float size, bool korean, int overH, int overV, float spacing)
{
	ImFont* font = FindFont(name);

	if (font)
		return false;

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig config;

	config.OversampleH = overH;
	config.OversampleV = overV;
	config.GlyphExtraSpacing.x = spacing;
	config.PixelSnapH = 1;

	if (!korean)
		font = io.Fonts->AddFontFromFileTTF(fullPath, size, &config, io.Fonts->GetGlyphRangesDefault());
	else
		font = io.Fonts->AddFontFromFileTTF(fullPath, size, &config, io.Fonts->GetGlyphRangesKorean());

	m_mapFont.insert(std::make_pair(name, font));

	return true;
}

void CIMGUIManager::SetCurrentFont(const std::string& name)
{
	ImFont* font = FindFont(name);

	if (!font)
		return;

	m_CurrentFont = font;
}

ImFont* CIMGUIManager::FindFont(const std::string& name)
{
	auto iter = m_mapFont.find(name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CIMGUIManager::SetCurrentFont()
{
	ImGui::PushFont(m_CurrentFont);
}

void CIMGUIManager::ResetCurrentFont()
{
	ImGui::PopFont();
}
