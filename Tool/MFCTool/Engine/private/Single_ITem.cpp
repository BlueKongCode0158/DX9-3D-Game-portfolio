#include "..\public\Single_ITem.h"

CSingle_ITem::CSingle_ITem(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CSingle_ITem::CSingle_ITem(const CSingle_ITem & rhs)
	:CItem(rhs)
{
}

HRESULT CSingle_ITem::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSingle_ITem::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CSingle_ITem::Add_Item()
{
}

int CSingle_ITem::Use_Item()
{
	return ITEM_OK;
}

CSingle_ITem * CSingle_ITem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSingle_ITem* pInstance = new CSingle_ITem(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CSingle_Item_Prototype");
	}
	return pInstance;
}

CItem * CSingle_ITem::Clone(void * pArg)
{
	CSingle_ITem* pInstance = new CSingle_ITem(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CSingle_Item_Prototype");
	}
	return pInstance;
}

void CSingle_ITem::Free()
{
	__super::Free();
}
