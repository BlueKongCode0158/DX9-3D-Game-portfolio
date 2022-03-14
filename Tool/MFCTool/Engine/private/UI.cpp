#include "..\public\UI.h"
#include "VIBuffer_Rect.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CUI::CUI(const CUI & rhs)
	:CGameObject(rhs)
{

}

HRESULT CUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDESC));
	}
	else
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&m_ProjMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, m_tUIDesc.m_fWinSizeX, m_tUIDesc.m_fWinSizeY, 0.f, 1.f);

	return S_OK;
}

_int CUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	return _int();
}

HRESULT CUI::Render()
{
	_matrix TransformMatrix, ViewMatrix;
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&TransformMatrix);
	D3DXMatrixIdentity(&ViewMatrix);
	TransformMatrix._11 = m_tUIDesc.m_fSizeX;
	TransformMatrix._22 = m_tUIDesc.m_fSizeY;

	TransformMatrix._41 = m_tUIDesc.m_fX - (m_tUIDesc.m_fWinSizeX * 0.5f);
	TransformMatrix._42 = -m_tUIDesc.m_fY + (m_tUIDesc.m_fWinSizeY * 0.5f);

	/* ½¦ÀÌ´õ·Î ¹Ù²ãÁà¾ßÇÏ´Â ºÎºÐ */

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));

	m_pShaderCom->Begin_Shader(m_iShaderIndex);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Shader();


	return S_OK;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
}
