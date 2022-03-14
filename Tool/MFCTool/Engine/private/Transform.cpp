#include "..\public\Transform.h"
#include "VIBuffer_Terrain.h"
#include "Navigation.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs)
	,m_WorldMatrix(rhs.m_WorldMatrix)
{
}

const _float3 CTransform::Get_Scale()
{
	return _float3(	D3DXVec3Length(&Get_MatrixRow(STATE::STATE_RIGHT)),
					D3DXVec3Length(&Get_MatrixRow(STATE::STATE_UP)),
					D3DXVec3Length(&Get_MatrixRow(STATE::STATE_LOOK)));
}

void CTransform::Set_MatrixScale(_float fScaleX, _float fScaleY, _float _fScaleZ)
{
	_float3 fRightRow	= Get_MatrixRow(STATE::STATE_RIGHT);
	_float3 fUpRow		= Get_MatrixRow(STATE::STATE_UP);
	_float3 fLookRow	= Get_MatrixRow(STATE::STATE_LOOK);

	D3DXVec3Normalize(&fRightRow, &fRightRow);
	D3DXVec3Normalize(&fUpRow, &fUpRow);
	D3DXVec3Normalize(&fLookRow, &fLookRow);

	/* 맨 처음에 Set 할 때만 써줄거니까 상관없나? */
	fRightRow	*= fScaleX;
	fUpRow		*= fScaleY;
	fLookRow	*= _fScaleZ;

	Set_WorldMatrixRow(STATE::STATE_RIGHT, fRightRow);
	Set_WorldMatrixRow(STATE::STATE_UP, fUpRow);
	Set_WorldMatrixRow(STATE::STATE_LOOK, fLookRow);
}

void CTransform::Walk_Back(_float TimeDelta, CNavigation* pNavigation/* = nullptr*/)
{
	_float3 vLook		= Get_MatrixRow(STATE::STATE_LOOK);
	_float3 vPosition	= Get_MatrixRow(STATE::STATE_POSITION);

	D3DXVec3Normalize(&vLook, &vLook);

	vPosition -= vLook*m_TransformInfo.fTransformSpec*TimeDelta;

	_float3*	vNormal = nullptr;

	if (nullptr == pNavigation ||
		true == pNavigation->Move_OnNavigation(vPosition, &vNormal))
	{
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}
	else if (nullptr != vNormal)
	{
		vPosition += vLook*m_TransformInfo.fTransformSpec*TimeDelta;

		D3DXVec3Normalize(vNormal, vNormal);
		vPosition = vPosition + (D3DXVec3Dot(vNormal, &vLook)* *vNormal) * m_TransformInfo.fTransformSpec * TimeDelta;
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}
}

void CTransform::Walk_Look(_float TimeDelta, CNavigation* pNavigation/* = nullptr*/)
{
	_float3 vLook = Get_MatrixRow(STATE::STATE_LOOK);
	_float3 vPosition = Get_MatrixRow(STATE::STATE_POSITION);

	D3DXVec3Normalize(&vLook, &vLook);

	vPosition += vLook*m_TransformInfo.fTransformSpec*TimeDelta;

	_float3*	vNormal = nullptr;

	if (nullptr == pNavigation ||
		true == pNavigation->Move_OnNavigation(vPosition, &vNormal))
	{
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}	
	else if (nullptr != vNormal)
	{
		D3DXVec3Normalize(vNormal, vNormal);
		vPosition = vPosition - (D3DXVec3Dot(vNormal, &vLook)* *vNormal) * m_TransformInfo.fTransformSpec * TimeDelta;
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}

}

void CTransform::Walk_Left(_float TimeDelta, CNavigation* pNavigation /* = nullptr*/)
{
	_float3 vRight = Get_MatrixRow(STATE::STATE_RIGHT);
	_float3 vPosition = Get_MatrixRow(STATE::STATE_POSITION);

	D3DXVec3Normalize(&vRight, &vRight);


	vPosition -= vRight*m_TransformInfo.fTransformSpec*TimeDelta;
	_float3*	vNormal = nullptr;

	if (nullptr == pNavigation ||
		true == pNavigation->Move_OnNavigation(vPosition,&vNormal))
	{
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}
	else if (nullptr != vNormal)
	{
		D3DXVec3Normalize(vNormal, vNormal);
		vPosition = vPosition + (D3DXVec3Dot(vNormal, &vRight)* *vNormal) * m_TransformInfo.fTransformSpec * TimeDelta;
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}
}

void CTransform::Walk_Right(_float TimeDelta, CNavigation* pNavigation/* = nullptr*/)
{

	_float3 vRight = Get_MatrixRow(STATE::STATE_RIGHT);
	_float3 vPosition = Get_MatrixRow(STATE::STATE_POSITION);

	D3DXVec3Normalize(&vRight, &vRight);

	vPosition += vRight*m_TransformInfo.fTransformSpec*TimeDelta;

	_float3*	vNormal = nullptr;

	if (nullptr == pNavigation ||
		true == pNavigation->Move_OnNavigation(vPosition, &vNormal))
	{
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}
	else if (nullptr != vNormal )
	{
		D3DXVec3Normalize(vNormal, vNormal);
		vPosition = vPosition - (D3DXVec3Dot(vNormal, &vRight)* *vNormal) * m_TransformInfo.fTransformSpec * TimeDelta;
		Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
	}

}

void CTransform::Jump(_float TimeDelta)
{
	_float3 vPosition = Get_MatrixRow(CTransform::STATE::STATE_POSITION);

	m_fAccel += TimeDelta;

	vPosition.y += m_fJumpPower*m_fAccel - m_fGravity * m_fAccel * m_fAccel *0.5f;
	Set_WorldMatrixRow(STATE::STATE_POSITION, vPosition);
}

void CTransform::SetUp_RotatinAxis(_float3 fAxis, _float fRadian)
{
	_float3 RightVector		= { 1.f, 0.f, 0.f };
	_float3 UpVector		= { 0.f, 1.f, 0.f };
	_float3 LookVector		= { 0.f, 0.f, 1.f };

	_float3 fScale = Get_Scale();

	RightVector *= fScale.x;
	UpVector	*= fScale.y;
	LookVector	*= fScale.z;

	_matrix		RotatonMatrix;

	D3DXMatrixRotationAxis(&RotatonMatrix, &fAxis, fRadian);

	Set_WorldMatrixRow(STATE::STATE_RIGHT	, *D3DXVec3TransformNormal(&RightVector, &RightVector, &RotatonMatrix));
	Set_WorldMatrixRow(STATE::STATE_UP		, *D3DXVec3TransformNormal(&UpVector, &UpVector, &RotatonMatrix));
	Set_WorldMatrixRow(STATE::STATE_LOOK	, *D3DXVec3TransformNormal(&LookVector, &LookVector, &RotatonMatrix));
}

void CTransform::Rotation_Aixs_Accumulate(_float3 fAxis, _float fRadian)
{
	_float3		vRight	= Get_MatrixRow(STATE::STATE_RIGHT);
	_float3		vUp		= Get_MatrixRow(STATE::STATE_UP);
	_float3		vLook	= Get_MatrixRow(STATE::STATE_LOOK);

	_matrix		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix,&fAxis, fRadian);

	Set_WorldMatrixRow(STATE::STATE_RIGHT, vRight);
	Set_WorldMatrixRow(STATE::STATE_UP, vUp);
	Set_WorldMatrixRow(STATE::STATE_LOOK, vLook);
}

void CTransform::Rotation_Axis(_float3 fAxis, _float TimeDelta)
{
	_float3 RightVector = Get_MatrixRow(STATE::STATE_RIGHT);
	_float3 UpVector	= Get_MatrixRow(STATE::STATE_UP);
	_float3 LookVector	= Get_MatrixRow(STATE::STATE_LOOK);

	_matrix		RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &fAxis, TimeDelta*m_TransformInfo.fRotationSpec);

	Set_WorldMatrixRow(STATE::STATE_RIGHT, *D3DXVec3TransformNormal(&RightVector, &RightVector, &RotationMatrix));
	Set_WorldMatrixRow(STATE::STATE_UP, *D3DXVec3TransformNormal(&UpVector, &UpVector, &RotationMatrix));
	Set_WorldMatrixRow(STATE::STATE_LOOK, *D3DXVec3TransformNormal(&LookVector, &LookVector, &RotationMatrix));
}

void CTransform::TargetToMove(_float TimeDelta,CTransform* tTarget_Transform)
{	
	if (nullptr == tTarget_Transform)
	{
		//MSGBOX("Failed to Find Target_TargetToMove");
		return;
	}
	_float3	vecPosition = Get_MatrixRow(STATE::STATE_POSITION);
	_float3 TargetPostion = tTarget_Transform->Get_MatrixRow(STATE::STATE_POSITION);
	_float3	dirVector = TargetPostion - vecPosition;
	
	if (D3DXVec3Length(&dirVector) <= 1)
	{
		return;
	}

	D3DXVec3Normalize(&dirVector, &dirVector);

	vecPosition += dirVector*m_TransformInfo.fTransformSpec*TimeDelta;

	Set_WorldMatrixRow(STATE::STATE_POSITION, vecPosition);

	TargetToRotation(tTarget_Transform);
}

void CTransform::TargetToMove(_float TimeDelta, _float3 TargetPosition)
{
	if (false == m_isMove)
	{
		return;
	}
	_float3	vecPosition = Get_MatrixRow(STATE::STATE_POSITION);
	_float3 TargetPostion = TargetPosition;
	_float3	dirVector = TargetPostion - vecPosition;

	if (D3DXVec3Length(&dirVector) <= 1)
	{
		m_isMove = false;
		return;
	}

	D3DXVec3Normalize(&dirVector, &dirVector);

	vecPosition += dirVector*m_TransformInfo.fTransformSpec*TimeDelta;

	Set_WorldMatrixRow(STATE::STATE_POSITION, vecPosition);

	TargetToRotation(TargetPosition);

}

void CTransform::Stand_On_Plane(const CTransform* pPlaneTransform, const CVIBuffer_Terrain* pTerrain)
{
	_float3	vTransformPosition = Get_MatrixRow(CTransform::STATE_POSITION);
	_float3 vWorldPosition = Get_MatrixRow(CTransform::STATE_POSITION);
	_matrix PlaneWorldMatrixInverse = *D3DXMatrixInverse(&PlaneWorldMatrixInverse, nullptr, pPlaneTransform->Get_WorldMatrix());
	_float3 vLocalPosition = *D3DXVec3TransformCoord(&vLocalPosition, &vWorldPosition, &PlaneWorldMatrixInverse);

	_float3 vLocalPositionScale = *D3DXVec3TransformCoord(&vLocalPositionScale, &Get_MatrixRow(CTransform::STATE::STATE_UP), &PlaneWorldMatrixInverse);
	_float	vLocalScaleY = D3DXVec3Length(&vLocalPositionScale);

	D3DXPLANE Plane = pTerrain->Get_Plane(vLocalPosition);

	vLocalPosition.y = vLocalPosition.y - D3DXPlaneDotCoord(&Plane, &vLocalPosition) + (vLocalScaleY * 0.5f);
	D3DXVec3TransformCoord(&vWorldPosition, &vLocalPosition, pPlaneTransform->Get_WorldMatrix());

	if (vWorldPosition.y > vTransformPosition.y)
	{
		m_fAccel = 0.f;
		m_isJump = false;
	}

	if (m_isJump == false)
	{
		Set_WorldMatrixRow(CTransform::STATE_POSITION, vWorldPosition);
	}

}
void CTransform::TargetToRotation(CTransform* tTarget_Transform)
{
	_float3 vRight,vUp;

	_float3 vPosition = Get_MatrixRow(STATE::STATE_POSITION);
	_float3 vTargetPosition = tTarget_Transform->Get_MatrixRow(STATE::STATE_POSITION);
	_float3 vLook = vTargetPosition - vPosition;
	vLook.y = 0.f;
	_float3 vScale = Get_Scale();

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Cross(&vRight, &Get_MatrixRow(STATE::STATE_UP), &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	Set_WorldMatrixRow(STATE::STATE_RIGHT, vRight*vScale.x);
	Set_WorldMatrixRow(STATE::STATE_LOOK, vLook*vScale.z);
}

void CTransform::Remove_Rotation()
{
	Set_WorldMatrixRow(STATE::STATE_RIGHT,	_float3(1.f, 0.f, 0.f) * Get_Scale().x);
	Set_WorldMatrixRow(STATE::STATE_UP,		_float3(0.f, 1.f, 0.f) * Get_Scale().y);
	Set_WorldMatrixRow(STATE::STATE_LOOK,	_float3(0.f, 0.f, 1.f) * Get_Scale().z);
}

void CTransform::TargetToRotation(_float3 vTargetPosition)
{
	_float3 vPosition = Get_MatrixRow(STATE::STATE_POSITION);
	_float3 vLook = vTargetPosition - vPosition;
	vLook.y = 0.f;

	_float3 vRight;

	D3DXVec3Normalize(&vLook, &vLook);
	vLook = vLook*Get_Scale().z;

	D3DXVec3Cross(&vRight, &Get_MatrixRow(STATE::STATE_UP), &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight = vRight*Get_Scale().x;
	
	Set_WorldMatrixRow(STATE::STATE_LOOK, vLook);
	Set_WorldMatrixRow(STATE::STATE_RIGHT, vRight);
}


HRESULT CTransform::NativeConstruct_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_TransformInfo, pArg, sizeof(TRANSFORMDESC));
	}
	return S_OK;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Faild to Creating CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CComponent* pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
