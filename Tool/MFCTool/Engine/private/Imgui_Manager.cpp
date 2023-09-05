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
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	return S_OK;
}

HRESULT CImgui_Manager::Set_Viewport()
{
	
	return S_OK;
}

void CImgui_Manager::Frame()
{
	CGraphic_Device* pGraphicInstance = GET_INSTANCE(CGraphic_Device);
	if (m_bWindowEnable)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Transform");
		ImGuizmo::BeginFrame();

		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
		ImGuizmo::SetRect(0, 0, pGraphicInstance->Get_WindowSize().x, pGraphicInstance->Get_WindowSize().y);

		ImGuizmo::Enable(true);
	}
	RELEASE_INSTANCE(CGraphic_Device);
}

void CImgui_Manager::Render()
{
	if (m_bWindowEnable)
	{
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		// ¿À·ù ÅÍÁü 2023.8.21
		ImGui::End();
		ImGui::Render();
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
