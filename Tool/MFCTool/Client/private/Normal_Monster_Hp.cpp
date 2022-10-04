#include "stdafx.h"
#include "GameInstacne.h"
#include "Client_Defines.h"
#include "..\public\Normal_Monster_Hp.h"

CNormal_Monster_Hp::CNormal_Monster_Hp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CNormal_Monster_Hp::CNormal_Monster_Hp(const CNormal_Monster_Hp & rhs)
	: CGameObject(rhs)
{

}

HRESULT CNormal_Monster_Hp::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	D3DXMatrixIdentity(&m_OriginMatrix);

	return S_OK;
}

HRESULT CNormal_Monster_Hp::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	D3DXMatrixIdentity(&m_OriginMatrix);

	m_vScale = _float3(7.f, 0.8f, 1.f);
	m_pTransformCom->Set_MatrixScale(m_vScale.x, m_vScale.y, m_vScale.z);
	return S_OK;
}


_int CNormal_Monster_Hp::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CNormal_Monster_Hp::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_ALPHA, this);

	_matrix ParentMatrix;
	/* 행렬 초기화 */
	D3DXMatrixIdentity(&ParentMatrix);

	if (nullptr != m_tHpdesc.pParentMatrix)
	{
		ParentMatrix = *m_tHpdesc.pParentMatrix;

		D3DXVec3Normalize(reinterpret_cast<_float3*>(&ParentMatrix.m[0][0]), reinterpret_cast<_float3*>(&ParentMatrix.m[0][0]));
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&ParentMatrix.m[1][0]), reinterpret_cast<_float3*>(&ParentMatrix.m[1][0]));
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&ParentMatrix.m[2][0]), reinterpret_cast<_float3*>(&ParentMatrix.m[2][0]));
	}

	_matrix WorldMatrix, ViewMatrix;
	WorldMatrix = m_OriginMatrix * ParentMatrix;

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * m_vScale.x);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * m_vScale.y);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * m_vScale.z);

	RELEASE_INSTANCE(CGameInstacne);

	return _int();
}

HRESULT CNormal_Monster_Hp::Render()
{
	
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	
	m_pShaderCom->Begin_Shader(1);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	return S_OK;
}

void CNormal_Monster_Hp::Set_HpPer(_float fHpPer)
{
	m_fHpPer = fHpPer;
}

void CNormal_Monster_Hp::Set_tagHpDesc(HPDESC* pHpDesc)
{
	memcpy(&m_tHpdesc, pHpDesc, sizeof(HPDESC));

	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, m_tHpdesc.vIniPos);
	m_OriginMatrix = *m_pTransformCom->Get_WorldMatrix();
}

HRESULT CNormal_Monster_Hp::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_HpBar"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CNormal_Monster_Hp::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, 0);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_fHpPer", &m_fHpPer,sizeof(_float));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CNormal_Monster_Hp * CNormal_Monster_Hp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CNormal_Monster_Hp* pInstance = new CNormal_Monster_Hp(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CNormal_Monster_Hp_Prototype");
	}
	return pInstance;
}

CGameObject * CNormal_Monster_Hp::Clone(void * pArg)
{
	CGameObject* pInstance = new CNormal_Monster_Hp(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CNormal_Monster_Hp_Clone");
	}
	return pInstance;
}

void CNormal_Monster_Hp::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
