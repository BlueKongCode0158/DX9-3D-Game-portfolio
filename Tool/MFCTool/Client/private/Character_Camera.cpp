#include "stdafx.h"
#include "..\public\Character_Camera.h"
#include "GameInstacne.h"
#include "Pod.h"

CCharacter_Camera::CCharacter_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{
}

CCharacter_Camera::CCharacter_Camera(const CCharacter_Camera & rhs)
	:CCamera(rhs)
{
}

HRESULT CCharacter_Camera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCharacter_Camera::NativeConstruct(void * pArg)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&m_OriginMatrix);
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	
	CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Pod"), TEXT("Com_Transform"), 0));

	if (nullptr == pTransform)
	{
		return E_FAIL;
	}

	m_pParentWorldMatrix = pTransform->Get_WorldMatrix();

	CDynamic_Mesh*	pPodMesh = reinterpret_cast<CDynamic_Mesh*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Pod"), TEXT("Com_Mesh"), 0));
	if (nullptr == pPodMesh)
	{
		return E_FAIL;
	}
	m_pForwardMatrix = pPodMesh->Get_BoneMatrixPointer("CameraForward");
	m_pEndMatrix = pPodMesh->Get_BoneMatrixPointer("Camera_end");

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

_int CCharacter_Camera::Tick(_double TimeDelta)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	__super::Tick(TimeDelta);

	CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));
	CPod*		pPod = dynamic_cast<CPod*>(pGameInstance->Find_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Pod"), 0));

	if (nullptr != pTransform && pPod->isFinish() == true)
	{
		_float3 vPosition = pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
		_float3 vLook = pTransform->Get_MatrixRow(CTransform::STATE::STATE_LOOK);
		_long MouseMove = 0l;

		MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::DIMM::DIMM_Y);

		if (0 != MouseMove)
		{
			m_AccYAxisVector -= _float3(0.f, 1.f, 0.f)*TimeDelta*MouseMove*0.2;
			m_pTransform->Rotation_Axis(m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_RIGHT), TimeDelta * MouseMove * 0.2f);
		}
		
		__super::Set_CameraTransform();

		if (m_isCameraSetting == false)
		{
			_float3 fAt{}, fEye{};

			fAt = vPosition + _float3(0.f, 5.f, 0.f) ;
			fEye = vPosition - vLook*100.f + _float3(0.f, 5.f, 0.f);

			_float3 fDirAt = fAt - m_CameraDesc.fAt;
			_float3 fDirEye = fEye - m_CameraDesc.fEye;

			_float fLengthAt = D3DXVec3Length(&fDirAt);
			_float fLengthEye = D3DXVec3Length(&fDirEye);

			D3DXVec3Normalize(&fDirAt, &fDirAt);
			D3DXVec3Normalize(&fDirEye, &fDirEye);
			m_CameraDesc.fAt += fDirAt*TimeDelta*10.f;
			m_CameraDesc.fEye += fDirEye*TimeDelta*10.f;

			if (fLengthAt <= 0.1f)
			{
				m_CameraDesc.fAt -= fDirAt*TimeDelta*10.f;
				fLengthAt = 0.05f;
			}
			if (fLengthEye <= 0.1f)
			{
				m_CameraDesc.fEye -= fDirEye*TimeDelta*10.f;
				fLengthEye = 0.05f;
			}

			if ((fLengthAt <= 0.1f) && (fLengthEye <= 0.1f))
			{
				m_isCameraSetting = true;
			}
		}
		else
		{
			m_CameraDesc.fAt = vPosition + _float3(0.f, 5.f, 0.f) + m_AccYAxisVector;
			m_CameraDesc.fEye = vPosition - vLook*100.f + _float3(0.f, 5.f, 0.f);
		}
	}
	else
	{
		_matrix End_WorldMatrix = *m_pEndMatrix * *m_pParentWorldMatrix;
		_matrix Forward_WorldMatrix = *m_pForwardMatrix * *m_pParentWorldMatrix;

		__super::Set_CameraTransform();

		m_CameraDesc.fEye = *reinterpret_cast<_float3*>(&Forward_WorldMatrix.m[3][0]);
		m_CameraDesc.fAt = *reinterpret_cast<_float3*>(&End_WorldMatrix.m[3][0]);
	}
	
	
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

_int CCharacter_Camera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCharacter_Camera::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCharacter_Camera::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CCharacter_Camera * CCharacter_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCharacter_Camera* pInstance = new CCharacter_Camera(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating Camera_Prototype");
	}
	return pInstance;
}

CCamera * CCharacter_Camera::Clone(void * pArg)
{
	CCharacter_Camera* pInstance = new CCharacter_Camera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating Camera_Clone");
	}
	return pInstance;
}

void CCharacter_Camera::Free()
{
	__super::Free();
}
