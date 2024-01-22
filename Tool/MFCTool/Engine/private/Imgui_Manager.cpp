#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "..\public\Imgui_Manager.h"
#include "Graphic_Device.h"
#include "ImGuizmo.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMPLEMENT_SINGLETON(CImgui_Manager)
CImgui_Manager::CImgui_Manager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();
}

HRESULT CImgui_Manager::NativeConstruct(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
	ImGuiIO& io = ImGui::GetIO();
	CGraphic_Device* pGraphicInstance = GET_INSTANCE(CGraphic_Device);
	io.DisplaySize.x = pGraphicInstance->Get_WindowSize().x;
	io.DisplaySize.y = pGraphicInstance->Get_WindowSize().y;
	RELEASE_INSTANCE(CGraphic_Device);
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	m_isInit = true;
	return S_OK;
}



void CImgui_Manager::Frame()
{
	if (m_bWindowEnable)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		//ImGui::Begin("Transform");
		//ImGuizmo::BeginFrame();
		//ImGui::Text("Hello, world!");
		//ImGuizmo::Enable(true);
		if (ImGui::Begin("Window Title"))
		{
			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &m_show_another_window);
			ImGui::End();
		}

		if (m_show_demo_window == true)
		{
			ImGui::ShowDemoWindow();
		}
	}
}

void CImgui_Manager::Render()
{
	if (m_bWindowEnable)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

void CImgui_Manager::OnOffWindow()
{
	m_bWindowEnable = !m_bWindowEnable;
}

LRESULT CImgui_Manager::Engine_ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_isInit == false || m_bWindowEnable == false)
	{
		return false;
	}
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

LRESULT CImgui_Manager::EditTransform(CGameObject * pObject)
{

	return S_OK;
}


void CImgui_Manager::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
