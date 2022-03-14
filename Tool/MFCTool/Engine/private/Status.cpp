#include "Status.h"

CStatus::CStatus(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CStatus::CStatus(const CStatus & rhs)
	:CComponent(rhs)
{
}

HRESULT CStatus::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CStatus::NativeConstruct(void * pArg)
{
	return S_OK;
}

CStatus * CStatus::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStatus*	pInstance = new CStatus(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CStatus_Prototype");
	}
	return pInstance;
}

CComponent * CStatus::Clone(void * pArg)
{
	CStatus*	pInstance = new CStatus(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CStatus_Clone");
	}
	return pInstance;
}

void CStatus::Free()
{
	__super::Free();
}
