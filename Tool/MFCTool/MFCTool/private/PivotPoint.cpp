#include "stdafx.h"
#include "GameInstacne.h"
#include "..\public\PivotPoint.h"
#include "Pivot.h"


CPivotPoint::CPivotPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPivotPoint::CPivotPoint(const CPivotPoint & rhs)
	:CGameObject(rhs)
{
}

HRESULT CPivotPoint::NativeConstruct_Prototype()
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPivotPoint::NativeConstruct(void * pArg)
{
	if (nullptr == pArg)
	{
		return E_FAIL;
	}

	memcpy(&m_tPointInfo, pArg, sizeof(PIVOT_INFO));

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CPivotPoint::Tick(_double TimeDelta)
{
	_float3* vPickingPoint = m_pPicking->Compute_PickingPoint(m_pVIBufferCom, *m_pTransformCom->Get_WorldMatrix());

	if (nullptr != vPickingPoint)
	{
		m_isContect = true;
	}
	else
	{
		m_isContect = false;
	}

	return _int();
}

_int CPivotPoint::Late_Tick(_double TimeDelta)
{

	
	return _int();
}

HRESULT CPivotPoint::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{   
		return E_FAIL;
	}
	m_pShaderCom->Begin_Shader(m_isContect);
	m_pVIBufferCom->Render();
	m_pShaderCom->End_Shader();

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pCollidercom);
#endif // _DEBUG

	return S_OK;
}

HRESULT CPivotPoint::Set_TargetObject(CTransform * pTransform)
{
	if (nullptr == pTransform)
	{
		return E_FAIL;
	}
	m_tPointInfo.pTargetTransform = pTransform;

	if (FAILED(SetUp_DirTransform(m_tPointInfo.eType)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPivotPoint::Set_PivotType(CPivotPoint::DIR_STATE eType)
{
	m_tPointInfo.eType = eType;

	return S_OK;
}

CTransform * CPivotPoint::Get_Transform()
{
	if (nullptr == m_pTransformCom)
	{
		return nullptr;
	}

	return m_pTransformCom;
}

HRESULT CPivotPoint::SetUp_DirTransform(DIR_STATE eType)
{
	_float3 vPosition, vRight, vUp, vLook, vDirPosition;
	const _float	fDir	= 200.f;

	vPosition				= m_tPointInfo.pTargetTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	vRight					= m_tPointInfo.pTargetTransform->Get_MatrixRow(CTransform::STATE::STATE_RIGHT);
	vUp						= m_tPointInfo.pTargetTransform->Get_MatrixRow(CTransform::STATE::STATE_UP);
	vLook					= m_tPointInfo.pTargetTransform->Get_MatrixRow(CTransform::STATE::STATE_LOOK);
	vDirPosition			= _float3(0.f, 0.f, 0.f);

	switch (eType)
	{
	case DIR_STATE::CENTER:
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition);
		break;
	case DIR_STATE::RIGHT:
		vDirPosition = vPosition + fDir * vRight;
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vDirPosition);
		break;
	case DIR_STATE::UP:
		vDirPosition = vPosition + fDir * vUp;
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vDirPosition);
		break;
	case DIR_STATE::LOOK:
		vDirPosition = vPosition + fDir * vLook;
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vDirPosition);
		break;
	default:
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CPivotPoint::Add_Component()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Cube"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 10.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
	{
		return E_FAIL;
	}

	CCollider::COLLIDERDESC CollDesc;
	CollDesc.pBoneMatrix = nullptr;
	CollDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	CollDesc.vScale = _float3(3.f, 3.f, 3.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollidercom))))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Point_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	m_pPicking = CPicking::Get_Instance();
	Safe_AddRef(m_pPicking);

	return S_OK;
}

HRESULT CPivotPoint::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);
	/*	matrix  g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
	texture g_DiffuseTexture;*/
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CPivotPoint * CPivotPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPivotPoint* pInstance = new CPivotPoint(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPivotPoint_Prototype");
	}
	return pInstance;
}

CGameObject * CPivotPoint::Clone(void * pArg)
{
	CPivotPoint* pInstance = new CPivotPoint(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPivotPoint_Clone");
	}
	return pInstance;
}

void CPivotPoint::Free()
{
	__super::Free();

	Safe_Release(m_pPicking);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCollidercom);
	Safe_Release(m_pTransformCom);
}
