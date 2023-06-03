#pragma once
#include "Base.h"
#include "VIBuffer_Terrain.h"

BEGIN(MFC_TOOL)
class CBrush_Manager final : public CBase
{
	DECLARE_SINGLETON(CBrush_Manager)
public:
	enum BRUSHTYPE { BRUSH_RECT, BRUSH_CIRCLE, BRUSH_END };
	enum BRUSHFUNCTION { TERRAIN_UP, TERRAIN_DOWN, TERRAIN_SPLATTING ,TERRAIN_SMOOTH, TERRAIN_FLAT, TERRAIN_END};
	enum TERRAINRENDERSTATE { TERRAIN_DEFAULT, TERRAIN_WIRE, TERRAIN_RECT, TERRAIN_CIRCLE, RENDER_END };
	enum BRUSHCOLOR{ RED, GREEN, BLUE, ALPHA ,COLOR_END};
private:
	CBrush_Manager();
	virtual ~CBrush_Manager() = default;
public:
	HRESULT	NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device);
public:
	void	Set_BrushSize(float fBrushSize)
	{
		m_fBrushSize = fBrushSize;
	}
	void	Set_BrushPower(float fBrushPower)
	{
		m_fBrushPower = fBrushPower;
	}
public:
	void	Set_BrushType(BRUSHTYPE eType) 
	{
		m_eBrushType = eType;
	}
	void	Set_BrushFuctionType(BRUSHFUNCTION eType)
	{
		m_eBrushFunction = eType;
	}
	void	Set_RenderState(TERRAINRENDERSTATE eType)
	{
		m_eRenderState = eType;
	}
public:
	_float	Get_BrushSize() 
	{ 
		return m_fBrushSize;
	}
	_float	Get_BrushPower()
	{
		return m_fBrushPower;
	}
	D3DXCOLOR Get_BrushColor()
	{
		return m_FilterColor;
	}
public:
	_bool	is_BrushType(BRUSHTYPE eType);
	_bool	is_BrushFunction(BRUSHFUNCTION eType);
	void	is_BrushColor(BRUSHCOLOR eType);
public:
	TERRAINRENDERSTATE	Get_Terrain_RenderState()
	{
		return m_eRenderState;
	}
private:
	_float				m_fBrushSize	 = 0.f;
	_float				m_fBrushPower	 = 0.f;
	D3DXCOLOR			m_FilterColor	 = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
private:
	LPDIRECT3DDEVICE9	m_pGraphicDevice = nullptr;
private:
	BRUSHTYPE			m_eBrushType		= BRUSH_END;
	BRUSHFUNCTION		m_eBrushFunction	= TERRAIN_END;
	TERRAINRENDERSTATE	m_eRenderState		= RENDER_END;
public:
	virtual	void Free() override;
};
END

