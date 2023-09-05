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
	HRESULT	Set_Viewport();
public:
	void	Frame();
	void	Render();
	void	OnOffWindow();
private:
	_bool	m_bWindowEnable = false;
public:
	virtual void Free() override;
};
END

