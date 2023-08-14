#include "..\public\Imgui_Manager.h"
#include "Graphic_Device.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"

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
	}
}

void CImgui_Manager::Render()
{
	if (m_bWindowEnable)
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

void CImgui_Manager::OnOffWindow()
{
	m_bWindowEnable = !m_bWindowEnable;
}

void CImgui_Manager::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
