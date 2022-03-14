#include "..\public\Collider.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CCollider::CCollider(const CCollider & rhs)
	:CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_eType(rhs.m_eType)
	, m_pTransform(reinterpret_cast<CTransform*>(rhs.m_pTransform->Clone()))
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
#ifdef _DEBUG
	,m_pShader(rhs.m_pShader)
#endif // DEBUG
{
	Safe_AddRef(m_pMesh);
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // DEBUG
	memcpy(m_vPoints, rhs.m_vPoints, sizeof(_float3) * 8);
	memcpy(m_iIndeces, rhs.m_iIndeces, sizeof(_uint) * 36);
}

void CCollider::Get_ColliderInfo(CTransform ** ppTransform, LPD3DXMESH * ppMesh)
{
	ppTransform = &m_pTransform;
	ppMesh		= &m_pMesh;
}

_matrix CCollider::Get_WorldMatrix() const
{
	return *m_pTransform->Get_WorldMatrix();
}

HRESULT CCollider::NativeConstruct_Prototype(COLLIDER	eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	if (FAILED(Compile_ShaderDebug()))
	{
		return E_FAIL;
	}
#endif // DEBUG
	m_eType = eType;

	HRESULT		hr = 0;

	switch (m_eType)
	{
	case COLLIDER::TYPE_AABB:
		hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);

		m_vMin = _float3(-0.5f, -0.5f, -0.5f);
		m_vMax = _float3(0.5f, 0.5f, 0.5f);

		m_vPoints[0] = _float3(-0.5f, 0.5f, -0.5f);
		m_vPoints[1] = _float3(0.5f, 0.5f, -0.5f);
		m_vPoints[2] = _float3(0.5f, -0.5f, -0.5f);
		m_vPoints[3] = _float3(-0.5f, -0.5f, -0.5f);

		m_vPoints[4] = _float3(-0.5f, 0.5f, 0.5f);
		m_vPoints[5] = _float3(0.5f, 0.5f, 0.5f);
		m_vPoints[6] = _float3(0.5f, -0.5f, 0.5f);
		m_vPoints[7] = _float3(-0.5f, -0.5f, 0.5f);

		
		(m_iIndeces)[0]	=	0; (m_iIndeces)[1]	=  1;
		(m_iIndeces)[2]	=	2; (m_iIndeces)[3]	=  0;
		(m_iIndeces)[4]	=	2; (m_iIndeces)[5]	=  3;

		
		(m_iIndeces)[6]	=	5;	(m_iIndeces)[7]	=  4;
		(m_iIndeces)[8]	=	7;	(m_iIndeces)[9]	=  5;
		(m_iIndeces)[10]=	7;	(m_iIndeces)[11]=  6;

		
		(m_iIndeces)[12]= 4; (m_iIndeces)[13]  = 0;
		(m_iIndeces)[14]= 3; (m_iIndeces)[15]  = 4;
		(m_iIndeces)[16]= 3; (m_iIndeces)[17]  = 7;

		
		(m_iIndeces)[18]= 1; (m_iIndeces)[19]  = 5;
		(m_iIndeces)[20]= 6; (m_iIndeces)[21]  = 1;
		(m_iIndeces)[22]= 6; (m_iIndeces)[23]  = 2;

		
		(m_iIndeces)[24]= 4; (m_iIndeces)[25]  = 5;
		(m_iIndeces)[26]= 1; (m_iIndeces)[27]  = 4;
		(m_iIndeces)[28]= 1; (m_iIndeces)[29]  = 0;

		
		(m_iIndeces)[30]= 3; (m_iIndeces)[31]  = 2;
		(m_iIndeces)[32]= 6; (m_iIndeces)[33]  = 3;
		(m_iIndeces)[34]= 6; (m_iIndeces)[35]  = 7;

		break;
	case COLLIDER::TYPE_OBB:
		hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);

		m_vPoints[0] = _float3(-0.5f, 0.5f, -0.5f);
		m_vPoints[1] = _float3(0.5f, 0.5f, -0.5f);
		m_vPoints[2] = _float3(0.5f, -0.5f, -0.5f);
		m_vPoints[3] = _float3(-0.5f, -0.5f, -0.5f);

		m_vPoints[4] = _float3(-0.5f, 0.5f, 0.5f);
		m_vPoints[5] = _float3(0.5f, 0.5f, 0.5f);
		m_vPoints[6] = _float3(0.5f, -0.5f, 0.5f);
		m_vPoints[7] = _float3(-0.5f, -0.5f, 0.5f);

		(m_iIndeces)[0] = 0; (m_iIndeces)[1] = 1;
		(m_iIndeces)[2] = 2; (m_iIndeces)[3] = 0;
		(m_iIndeces)[4] = 2; (m_iIndeces)[5] = 3;


		(m_iIndeces)[6] = 5;	(m_iIndeces)[7] = 4;
		(m_iIndeces)[8] = 7;	(m_iIndeces)[9] = 5;
		(m_iIndeces)[10] = 7;	(m_iIndeces)[11] = 6;


		(m_iIndeces)[12] = 4; (m_iIndeces)[13] = 0;
		(m_iIndeces)[14] = 3; (m_iIndeces)[15] = 4;
		(m_iIndeces)[16] = 3; (m_iIndeces)[17] = 7;


		(m_iIndeces)[18] = 1; (m_iIndeces)[19] = 5;
		(m_iIndeces)[20] = 6; (m_iIndeces)[21] = 1;
		(m_iIndeces)[22] = 6; (m_iIndeces)[23] = 2;


		(m_iIndeces)[24] = 4; (m_iIndeces)[25] = 5;
		(m_iIndeces)[26] = 1; (m_iIndeces)[27] = 4;
		(m_iIndeces)[28] = 1; (m_iIndeces)[29] = 0;


		(m_iIndeces)[30] = 3; (m_iIndeces)[31] = 2;
		(m_iIndeces)[32] = 6; (m_iIndeces)[33] = 3;
		(m_iIndeces)[34] = 6; (m_iIndeces)[35] = 7;


		break;
	case COLLIDER::TYPE_SPHERE:
		hr = D3DXCreateSphere(m_pGraphic_Device, 0.5f, 10, 10, &m_pMesh, nullptr);
	default:
		break;
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	m_vOriginScale = m_ColliderDesc.vScale;
	_float3		vPosition = m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);

	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, m_ColliderDesc.vIniPos);
	m_pTransform->Set_MatrixScale(m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y, m_ColliderDesc.vScale.z);
	m_pTransform->Rotation_Aixs_Accumulate(_float3(1.f, 0.f, 0.f), m_ColliderDesc.vRadians.x);
	m_pTransform->Rotation_Aixs_Accumulate(_float3(0.f, 1.f, 0.f), m_ColliderDesc.vRadians.y);
	m_pTransform->Rotation_Aixs_Accumulate(_float3(0.f, 0.f, 1.f), m_ColliderDesc.vRadians.z);
	
	m_OriginalMatrix = *m_pTransform->Get_WorldMatrix();

	D3DXMatrixIdentity(&m_BoneMatrix);

	return S_OK;
}

void CCollider::Update_Collider()
{
	if (nullptr != m_ColliderDesc.pBoneMatrix)
	{
		m_BoneMatrix = *m_ColliderDesc.pBoneMatrix;
	}
	m_ParentMatrix = *m_ColliderDesc.pParentMatrix;

	if (nullptr == m_ColliderDesc.pBoneMatrix)
	{
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_ParentMatrix.m[0][0]), reinterpret_cast<_float3*>(&m_ParentMatrix.m[0][0]));
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_ParentMatrix.m[1][0]), reinterpret_cast<_float3*>(&m_ParentMatrix.m[1][0]));
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_ParentMatrix.m[2][0]), reinterpret_cast<_float3*>(&m_ParentMatrix.m[2][0]));
	
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_BoneMatrix.m[0][0]), reinterpret_cast<_float3*>(&m_BoneMatrix.m[0][0]));
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_BoneMatrix.m[1][0]), reinterpret_cast<_float3*>(&m_BoneMatrix.m[1][0]));
		D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_BoneMatrix.m[2][0]), reinterpret_cast<_float3*>(&m_BoneMatrix.m[2][0]));
	}
	_matrix BoneMatrix = m_BoneMatrix * m_ParentMatrix;
	_matrix WorldMatrix = m_OriginalMatrix * BoneMatrix;

	m_pTransform->Set_WorldMatrix(WorldMatrix);

	if (m_eType == CCollider::TYPE_AABB)
	{
		m_pTransform->Remove_Rotation();
	}
}

void CCollider::Get_PointPolygonIndex(_uint uiPolygonIndex, _float3 * pPoints) const
{
	_uint iIndex = uiPolygonIndex * 3;
	pPoints[0] = m_vPoints[m_iIndeces[iIndex]];
	pPoints[1] = m_vPoints[m_iIndeces[iIndex + 1]];
	pPoints[2] = m_vPoints[m_iIndeces[iIndex + 2]];
}

const CCollider::COLLIDERDESC CCollider::Get_ColliderDesc() const
{
	return m_ColliderDesc;
}

HRESULT CCollider::Reset_ColliderScalse()
{
	m_ColliderDesc.vScale = m_vOriginScale;
	SetUp_ColliderScale();
	return S_OK;
}

HRESULT CCollider::Update_ColliderScale(_float3 vScale)
{
	m_ColliderDesc.vScale = vScale;
	SetUp_ColliderScale();
	return S_OK;
}

HRESULT CCollider::SetUp_ColliderScale()
{
	_float3	vRight	= *reinterpret_cast<_float3*>(&m_OriginalMatrix.m[0][0]);
	_float3	vUp		= *reinterpret_cast<_float3*>(&m_OriginalMatrix.m[1][0]);
	_float3	vLook	= *reinterpret_cast<_float3*>(&m_OriginalMatrix.m[2][0]);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	vRight	*= m_ColliderDesc.vScale.x;
	vUp		*= m_ColliderDesc.vScale.y;
	vLook	*= m_ColliderDesc.vScale.z;

	memcpy(&m_OriginalMatrix.m[0][0], &vRight	, sizeof(_float3));
	memcpy(&m_OriginalMatrix.m[1][0], &vUp		, sizeof(_float3));
	memcpy(&m_OriginalMatrix.m[2][0], &vLook	, sizeof(_float3));

	return S_OK;
}


_bool CCollider::Collision_AABB(CCollider * pTargetCollider)
{
	_float3		vSourMin, vSourMax;
	_float3		vDestMin, vDestMax;

	_matrix		TransformMatrix = *m_pTransform->Get_WorldMatrix();

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &TransformMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &TransformMatrix);

	D3DXVec3TransformCoord(&vDestMin, &pTargetCollider->m_vMin, pTargetCollider->m_pTransform->Get_WorldMatrix());
	D3DXVec3TransformCoord(&vDestMax, &pTargetCollider->m_vMax, pTargetCollider->m_pTransform->Get_WorldMatrix());
	
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
	{
		m_isCollision = false;
		return false;
	}
	
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
	{
		m_isCollision = false;
		return false;
	}

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
	{
		m_isCollision = false;
		return false;
	}

	m_isCollision = true;
	return true;
}

_bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	_float3		vSourPoint[8], vDestPoint[8];

	for (_uint i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&vSourPoint[i], &m_vPoints[i], m_pTransform->Get_WorldMatrix());
		D3DXVec3TransformCoord(&vDestPoint[i], &pTargetCollider->m_vPoints[i], pTargetCollider->m_pTransform->Get_WorldMatrix());
	}

	OBBDESC			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc(vSourPoint);
	OBBDesc[1] = Compute_OBBDesc(vDestPoint);

	_float			fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for(_uint j = 0; j < 3 ; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[2], &OBBDesc[i].vAlignAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[2], &OBBDesc[i].vAlignAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&(OBBDesc[1].vCenter - OBBDesc[0].vCenter), &OBBDesc[i].vAlignAxis[j]));

			if (fDistance[2] > fDistance[0] + fDistance[1])
			{
				m_isCollision = false;
				return false;
			}
		}
	}

	m_isCollision = true;
	return true;
}

_bool CCollider::Collision_SPHERE(CCollider * pTargetCollider)
{
	_float3 vRange	= m_pTransform->Get_Scale();

	_float3	fDir = m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION) - pTargetCollider->m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	_float	fDistance = D3DXVec3Length(&fDir);

	if (vRange.x * 0.5f > fDistance)
	{
		m_isCollision = true;
		return true;
	}
	m_isCollision = false;
	return false;
}
#ifdef _DEBUG

HRESULT CCollider::Render()
{
	if (nullptr == m_pMesh ||
		nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	CPipeLine*	pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pShader->SetUp_ConstantTable("g_WorldMatrix", m_pTransform->Get_WorldMatrix(), sizeof(_matrix))) ||
		FAILED(m_pShader->SetUp_ConstantTable("g_ViewMatrix", pPipeLine->Get_Transform(D3DTS_VIEW), sizeof(_matrix))) ||
		FAILED(m_pShader->SetUp_ConstantTable("g_ProjMatrix", pPipeLine->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix))) ||
		FAILED(m_pShader->SetUp_Bool("g_isCollision", m_isCollision)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CPipeLine);

	m_pShader->Begin_Shader(0);
	m_pMesh->DrawSubset(0);
	m_pShader->End_Shader();

	return S_OK;
}

HRESULT CCollider::Compile_ShaderDebug()
{
	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../bin/ShaderFiles/Shader_Collider.hlsl"));
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}
	return S_OK;
}

#endif // _DEBUG


CCollider::OBBDESC CCollider::Compute_OBBDesc(_float3 * pPoints)
{
	OBBDESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));
	
	OBBDesc.vCenter = (pPoints[5] + pPoints[3]) * 0.5f;
	OBBDesc.vAlignAxis[0] = pPoints[5] - pPoints[4];
	OBBDesc.vAlignAxis[1] = pPoints[5] - pPoints[6];
	OBBDesc.vAlignAxis[2] = pPoints[5] - pPoints[1];

	for (_uint i = 0; i < 3 ; ++i)
	{
		D3DXVec3Normalize(&OBBDesc.vAlignAxis[i], &OBBDesc.vAlignAxis[i]);
	}

	OBBDesc.vCenterAxis[0] = (pPoints[5] + pPoints[2]) * 0.5f - OBBDesc.vCenter;
	OBBDesc.vCenterAxis[1] = (pPoints[5] + pPoints[0]) * 0.5f - OBBDesc.vCenter;
	OBBDesc.vCenterAxis[2] = (pPoints[5] + pPoints[7]) * 0.5f - OBBDesc.vCenter;

	return OBBDesc;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER	eType)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSGBOX("Failed To Creating CCollider_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CComponent* pInstance = new CCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To Creating CCollider_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pMesh);

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // DEBUG
}
