#include "stdafx.h"
#include "..\public\Brush_Manager.h"
#include "MFC_Defines.h"

IMPLEMENT_SINGLETON(CBrush_Manager)
CBrush_Manager::CBrush_Manager()
{
}


CBrush_Manager::~CBrush_Manager()
{
}

HRESULT CBrush_Manager::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphicDevice = pGraphic_Device;

	Safe_AddRef(m_pGraphicDevice);

	return S_OK;
}

_bool CBrush_Manager::is_BrushType(BRUSHTYPE eType)
{
	if (eType == m_eBrushType)
	{
		return true;
	}
	return false;
}

_bool CBrush_Manager::is_BrushFunction(BRUSHFUNCTION eType)
{
	if (eType == m_eBrushFunction)
	{
		return true;
	}
	return false;
}

void CBrush_Manager::is_BrushColor(BRUSHCOLOR eType)
{
	if (eType == BRUSHCOLOR::RED)
	{
		m_FilterColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}
	else if (eType == BRUSHCOLOR::GREEN)
	{
		m_FilterColor = D3DXCOLOR(0.f, 1.f, 0.f, 0.f);
	}
	else if (eType == BRUSHCOLOR::BLUE)
	{
		m_FilterColor = D3DXCOLOR(0.f, 0.f, 1.f, 0.f);
	}
	else if (eType == BRUSHCOLOR::ALPHA)
	{
		m_FilterColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	}
}

void CBrush_Manager::Free()
{
	Safe_Release(m_pGraphicDevice);
}
