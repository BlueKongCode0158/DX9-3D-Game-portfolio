#include "stdafx.h"
#include "Client_Defines.h"
#include "..\public\Boss_Hp.h"
#include "GameInstacne.h"

CBoss_Hp::CBoss_Hp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBoss_Hp::CBoss_Hp(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_Hp::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBoss_Hp::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CBoss_Hp::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CBoss_Hp::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);

	return _int();
}

HRESULT CBoss_Hp::Render()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);

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

	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom,0);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &m_ProjMatrix, sizeof(_matrix));

	if (FAILED(m_pShaderCom->SetUp_ConstantTable("g_fHpPer", &m_fHpPer, sizeof(_float))))
	{
		return E_FAIL;
	}


	m_pShaderCom->Begin_Shader(3);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

HRESULT CBoss_Hp::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_3D"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_HpBar"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CBoss_Hp::Set_HpPer(_float fHpPer)
{
	m_fHpPer = fHpPer;
}

HRESULT CBoss_Hp::Set_UIDesc(UIDESC * tUIDesc)
{
	memcpy(&m_tUIDesc, tUIDesc, sizeof(UIDESC));

	D3DXMatrixIdentity(&m_ProjMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, m_tUIDesc.m_fWinSizeX, m_tUIDesc.m_fWinSizeY, 0.f, 1.f);

	return S_OK;
}

CBoss_Hp * CBoss_Hp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Hp*	pInstance = new CBoss_Hp(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CBoss_Hp_Prototype");
	}
	return pInstance;
}

CGameObject * CBoss_Hp::Clone(void * pArg)
{
	CBoss_Hp*	pInstance = new CBoss_Hp(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CBoss_Hp_Prototype");
	}
	return pInstance;
}

void CBoss_Hp::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

}
