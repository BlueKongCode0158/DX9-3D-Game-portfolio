#include "..\public\Camera.h"
#include "PipeLine.h"
#include "Transform.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
	,m_pPipeLine(CPipeLine::Get_Instance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera & rhs)
	:CGameObject(rhs)
	,m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);
	
	if (nullptr != pArg)
	{
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));
	}
	_float3	vLook = (m_CameraDesc.fAt - m_CameraDesc.fEye);
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vRight;
	D3DXVec3Cross(&vRight, &m_CameraDesc.fAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_float3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_RIGHT, vRight);
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_UP, vUp);
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_LOOK, vLook);
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, m_CameraDesc.fEye);

	Set_Transform();
	return S_OK;
}

_int CCamera::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	Set_Transform();

	return _int();
}

_int CCamera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCamera::Set_Transform()
{
	_matrix ProjMatrix;

	if (FAILED(m_pPipeLine->Set_Transform(D3DTS_VIEW, m_pTransform->Get_WorldMatrixInverse())))
	{
		return E_FAIL;
	}

	D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFov, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	
	if (FAILED(m_pPipeLine->Set_Transform(D3DTS_PROJECTION, ProjMatrix)))
	{
		return E_FAIL;
	}
	//if(FAILED(m_pPipeLine->Set_Transform(D3DTS_PROJECTION,&pro)))
	return S_OK;
}

HRESULT CCamera::Set_CameraTransform()
{
	_float3	vLook = (m_CameraDesc.fAt - m_CameraDesc.fEye);
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vRight;
	D3DXVec3Cross(&vRight, &m_CameraDesc.fAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_float3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_RIGHT, vRight);
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_UP, vUp);
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_LOOK, vLook);
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, m_CameraDesc.fEye);
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pPipeLine);
}
