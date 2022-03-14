#include "stdafx.h"
#include "Client_Defines.h"
#include "..\public\BossHp_BackGround.h"
#include "GameInstacne.h"

CBossHp_BackGround::CBossHp_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBossHp_BackGround::CBossHp_BackGround(const CBossHp_BackGround & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBossHp_BackGround::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBossHp_BackGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileExW(m_pGraphicDevice, TEXT("../bin/resources/Image/UI/Hp/texUIBackdropFadedEnds.tga"), 256, 16, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture)))
	{
		return E_FAIL;
	}


	return S_OK;
}

_int CBossHp_BackGround::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CBossHp_BackGround::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_ALPHA, this);

	return _int();
}

HRESULT CBossHp_BackGround::Render()
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

	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTexture);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &m_ProjMatrix, sizeof(_matrix));

	//m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, 0);
	//m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	//m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	//m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	//m_pShaderCom->SetUp_ConstantTable("g_fHpPer", &m_fHpPer, sizeof(_float));

	m_pShaderCom->Begin_Shader(2);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

void CBossHp_BackGround::Set_HpPer(_float fHpPer)
{
	m_fHpPer = fHpPer;
}

HRESULT CBossHp_BackGround::Set_UIDesc(UIDESC* tUIDesc)
{
	memcpy(&m_tUIDesc, tUIDesc, sizeof(UIDESC));

	D3DXMatrixIdentity(&m_ProjMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, m_tUIDesc.m_fWinSizeX, m_tUIDesc.m_fWinSizeY, 0.f, 1.f);

	return S_OK;
}

HRESULT CBossHp_BackGround::Add_Component()
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

	return S_OK;
}

CBossHp_BackGround * CBossHp_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossHp_BackGround* pInstance = new CBossHp_BackGround(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating BossHp_BackGround_Clone");
	}
	return pInstance;
}

CGameObject * CBossHp_BackGround::Clone(void * pArg)
{
	CBossHp_BackGround* pInstance = new CBossHp_BackGround(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating BossHp_BackGround_Clone");
	}
	return pInstance;
}

void CBossHp_BackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

}
