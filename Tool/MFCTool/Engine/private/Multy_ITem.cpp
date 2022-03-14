#include "..\public\Multy_ITem.h"



HRESULT CMulty_ITem::NativeConstruct_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CMulty_ITem::NativeConstruct(void * pArg)
{
	return E_NOTIMPL;
}

void CMulty_ITem::Add_Item()
{
}

void CMulty_ITem::Use_Item()
{
}

CMulty_ITem * CMulty_ITem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMulty_ITem* pInstance = new CMulty_ITem(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMulty_ITem_Prototype");
	}
	return pInstance;
}

CItem * CMulty_ITem::Clone(void * pArg)
{
	CMulty_ITem* pInstance = new CMulty_ITem(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMulty_ITem_Prototype");
	}
	return pInstance;
}

void CMulty_ITem::Free()
{
	__super::Free();
}
