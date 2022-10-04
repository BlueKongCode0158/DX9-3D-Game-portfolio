#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstacne.h"
#include "..\public\Mouse_Cursor.h"

CMouse_Cursor::CMouse_Cursor(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

HRESULT CMouse_Cursor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMouse_Cursor::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int CMouse_Cursor::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_tUIDesc.m_fX = pt.x + m_tUIDesc.m_fSizeX * 0.2f;
	m_tUIDesc.m_fY = pt.y + m_tUIDesc.m_fSizeY * 0.5f;



	return _int();
}

_int CMouse_Cursor::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);

	return _int();
}

HRESULT CMouse_Cursor::Render()
{
	m_iShaderIndex = 4;

	if (FAILED(m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, m_isActive)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMouse_Cursor::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_2D"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Cursor"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

CMouse_Cursor * CMouse_Cursor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMouse_Cursor* pInstance = new CMouse_Cursor(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMouse_Cursor_Prototype");
	}
	return pInstance;
}

CGameObject * CMouse_Cursor::Clone(void * pArg)
{
	CMouse_Cursor* pInstance = new CMouse_Cursor(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMouse_Cursor_Clone");
	}
	return pInstance;
}

void CMouse_Cursor::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
