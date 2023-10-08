#pragma once
#include "Base.h"

BEGIN(Engine)
class CImgui_Manager : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)
private:
	explicit CImgui_Manager();
	virtual ~CImgui_Manager() = default;
public:
	HRESULT	NativeConstruct(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
public:
	void	Frame();
	void	Render();
	void	OnOffWindow();
	LRESULT	Engine_ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	_bool	m_isInit		= false;
	_bool	m_bWindowEnable = false;
private:
	_bool	m_show_another_window	= false;
	_bool	m_show_demo_window		= false;
public:
	virtual void Free() override;
};
END

