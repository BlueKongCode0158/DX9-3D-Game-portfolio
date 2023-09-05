#pragma once
#include "Base.h"

BEGIN(Engine)
class CGraphic_Device final: public Engine::CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font() { return m_pFont; }
	LPD3DXLINE			Get_Line() { return m_pLine; }
	HWND				Get_hWnd() { return m_hWnd; }
	_float2				Get_WindowSize() { return m_WindowSize; }
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE eMode ,_uint iWinCx, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);
	void	Render_Begin();
	void	Render_End(HWND hWND = nullptr);
private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
	HWND				m_hWnd;
private:
	_float2				m_WindowSize;
public:
	virtual void Free() override;
};
END

