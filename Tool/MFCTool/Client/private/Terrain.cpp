#include "stdafx.h"
#include "..\public\Terrain.h"
#include "Component.h"
#include "Transform.h"
#include "GameInstacne.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	:CGameObject(rhs)
{

}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CTerrain::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CTerrain::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CTerrain::Render()
{
	__super::Render();



	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CTerrain::Add_Component()
{

	if(FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Terrain"), TEXT("Com_VIBuffer_Terrain"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Terrain"), TEXT("Com_Shader") , reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	if (nullptr != m_pFilterTexture)
	{
		m_pShaderCom->SetUp_TextureConstantTable("g_FilterTexture", m_pFilterTexture);
	}
	if (nullptr != m_pTextureCom)
	{
		m_pShaderCom->SetUp_TextureConstantTable("g_RedDiffuseTexture", m_pTextureCom, 0);
		m_pShaderCom->SetUp_TextureConstantTable("g_GreenDiffuseTexture", m_pTextureCom, 1);
		m_pShaderCom->SetUp_TextureConstantTable("g_BlueDiffuseTexture", m_pTextureCom, 2);
		m_pShaderCom->SetUp_TextureConstantTable("g_AlphaDiffuseTexture", m_pTextureCom, 3);
	}

	D3DLIGHT9		LightDesc = pGameInstance->Get_LightDesc();

	m_pShaderCom->SetUp_ConstantTable("g_vLightDir", &_float4(LightDesc.Direction, 0.f), sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightDiffuse", &LightDesc.Diffuse, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightAmbient", &LightDesc.Ambient, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightSpecular", &LightDesc.Specular, sizeof(_float4));

	m_pShaderCom->SetUp_ConstantTable("g_vCamPosition", pGameInstance->Get_CamPosition(), sizeof(_float4));
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating Terrain_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CGameObject* pInstance = new CTerrain(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Terrain_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
}
