#include "stdafx.h"
#include "..\public\Camera_Free.h"
#include "GameInstacne.h"


CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{
	
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	:CCamera(rhs)
{

}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();
	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CCamera_Free::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pTransform->Walk_Look(TimeDelta);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransform->Walk_Left(TimeDelta);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransform->Walk_Back(TimeDelta);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransform->Walk_Right(TimeDelta);
	}


	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	_long MouseMove = 0l;

	MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::DIMM::DIMM_X);

	if (0 != MouseMove)
	{
		m_pTransform->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta * MouseMove * 0.2f);
	}

	MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::DIMM::DIMM_Y);

	if (0 != MouseMove)
	{
		m_pTransform->Rotation_Axis(m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_RIGHT), TimeDelta*MouseMove * 0.2f);
	}

	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

_int CCamera_Free::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyBoard_Down(DIK_H))
	{
		_float3 vPosition = m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
		char str[100];

		sprintf(str, "x: %.1f y:%.1f z: %0.1f", vPosition.x, vPosition.y, vPosition.z);
		MessageBoxA(g_hWnd, str, NULL, 0);
	}

	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Add_Component()
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

CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free* pInstance = new CCamera_Free(pGraphic_Device);
	if (pInstance->NativeConstruct_Prototype())
	{
		MSGBOX("Failed to Creating Camera_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CCamera * CCamera_Free::Clone(void * pArg)
{
	CCamera* pInstance = new CCamera_Free(*this);
	if (pInstance->NativeConstruct(pArg))
	{
		MSGBOX("Failed to Creating Camera_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
