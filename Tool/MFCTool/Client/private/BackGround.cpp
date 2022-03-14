#include "stdafx.h"
#include "Client_Defines.h"
#include "..\public\BackGround.h"
#include "GameInstacne.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CBackGround::CBackGround(CBackGround & rhs)
	:CUI(rhs)
{
}

HRESULT CBackGround::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}
	
	if (FAILED(D3DXCreateTexture(m_pGraphicDevice, iWinCX, iWinCY, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture)))
	{
		return E_FAIL;
	}

	D3DLOCKED_RECT		lockedRect;

	m_pTexture->LockRect(0, &lockedRect, 0, 0);

	for (_uint i = 0; i < iWinCY; i++)
	{
		for (_uint j = 0; j < iWinCX; j++)
		{
			_uint	iIndex = iWinCX*i + j;
			((_ulong*)lockedRect.pBits)[iIndex] = D3DXCOLOR(
				0,
				0,
				0,
				1);
		}
	}


	m_pTexture->UnlockRect(0);

	return S_OK;
}

_int CBackGround::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	return _int();
}

_int CBackGround::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);

	return _int();
}

HRESULT CBackGround::Render()
{
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

HRESULT CBackGround::Add_Components()
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

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround* pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBackGround_ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CBackGround_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);

}
