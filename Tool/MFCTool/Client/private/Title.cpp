#include "stdafx.h"
#include "..\public\Title.h"
#include "GameInstacne.h"

CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CTitle::CTitle(const CTitle & rhs)
	:CUI(rhs)
{
}

HRESULT CTitle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTitle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileExW(m_pGraphicDevice, TEXT("../bin/resources/logo/logo_stacked_colored_outlined_dropshadow_hard_small.tga"), 2048, 896, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture)))
	{
		return E_FAIL;
	}


	return S_OK;
}

_int CTitle::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CTitle::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);

	return _int();
}

HRESULT CTitle::Render()
{
	m_iShaderIndex = 0;
	if (FAILED(m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTexture)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTitle::Add_Component()
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

	return S_OK;
}

CTitle * CTitle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTitle*		pInstance = new CTitle(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CTitle_Prorotype");
	}
	return pInstance;
}

CGameObject * CTitle::Clone(void * pArg)
{
	CTitle*		pInstance = new CTitle(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CTitle_Clone");
	}
	return pInstance;
}

void CTitle::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
}
