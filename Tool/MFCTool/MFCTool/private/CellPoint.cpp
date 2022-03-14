#include "stdafx.h"
#include "..\public\CellPoint.h"
#include "GameInstacne.h"
#include "CellPoint_Manager.h"



CCellPoint::CCellPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CCellPoint::CCellPoint(const CCellPoint & rhs)
	:CGameObject(rhs)
{

}


HRESULT CCellPoint::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCellPoint::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	if (nullptr != pArg)
	{
		_float3	vPosition = *reinterpret_cast<_float3*>(pArg);
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition);
	}
	m_pTransformCom->Set_MatrixScale(0.25f, 0.25f, 0.25f);


	CCellPoint_Manager::Get_Instance()->Add_CellPoint(this);

	return S_OK;
}

_int CCellPoint::Tick(double TimeDelta)
{
	if (true == m_isSelect && true == m_isDead)
	{
		return CGameObject::STATE::OBJ_DEAD;
	}

	return _int();
}

_int CCellPoint::Late_Tick(double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CCellPoint::Render()
{

	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	_uint uiShaderNum = m_isSelect == true ? 2 : 1;

	m_pShaderCom->Begin_Shader(uiShaderNum);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	return S_OK;
}

_float3 CCellPoint::Get_Position()
{
	return m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
}

_float3 CCellPoint::Get_LocalPosition()
{
	_matrix WorldMatrix = *m_pTransformCom->Get_WorldMatrix();
	_matrix WorldMatrixInverse;

	D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	_float3 LocalPosition;
	D3DXVec3TransformCoord(&LocalPosition, &m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION),&WorldMatrixInverse);

	return LocalPosition;
}

HRESULT CCellPoint::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototyoe_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Cube"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Point_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCellPoint::SetUp_ConstantTable()
{
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

CCellPoint * CCellPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCellPoint*	pInstance = new CCellPoint(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CCellPoint_Prototype");
	}
	return pInstance;
}

CGameObject * CCellPoint::Clone(void * pArg)
{
	CCellPoint* pInstance = new CCellPoint(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CCellPoint_Clone");
	}
	return pInstance;
}

void CCellPoint::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
