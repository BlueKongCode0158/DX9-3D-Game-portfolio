#include "stdafx.h"
#include "MFC_Defines.h"
#include "Free_Camera.h"
#include "GameInstacne.h"

CFree_Camera::CFree_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{

}

CFree_Camera::CFree_Camera(const CFree_Camera & rhs)
	:CCamera(rhs)
{

}

HRESULT CFree_Camera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CFree_Camera::NativeConstruct(void * pArg)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int CFree_Camera::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
	{
		m_pTransform->Walk_Look(TimeDelta);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
	{
		m_pTransform->Walk_Left(TimeDelta);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
	{
		m_pTransform->Walk_Back(TimeDelta);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
	{
		m_pTransform->Walk_Right(TimeDelta);
	}


	_long MouseMove = 0l;


	if (pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM_RBUTTON))
	{
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
	}
	RELEASE_INSTANCE(CGameInstacne);

	return _int();
}

_int CFree_Camera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CFree_Camera::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CFree_Camera::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 10.f;
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CFree_Camera * CFree_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFree_Camera* pInstance = new CFree_Camera(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CFree_Camera_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFree_Camera::Clone(void * pArg)
{
	CGameObject* pInstance = new CFree_Camera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CFree_Camera_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFree_Camera::Free()
{
	__super::Free();
}
