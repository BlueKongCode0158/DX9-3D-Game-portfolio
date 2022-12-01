#include "..\public\Multy_ITem.h"
#include "Item.h"


CMulty_ITem::CMulty_ITem(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CMulty_ITem::CMulty_ITem(const CMulty_ITem & rhs)
	:CItem(rhs)
{
}

HRESULT CMulty_ITem::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMulty_ITem::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CMulty_ITem::Add_Item()
{
	m_iNums += 1;
}

int CMulty_ITem::Use_Item()
{
	m_iNums -= 1;
	if (m_iNums == 0)
	{
		return ITEM_DELETE;
	}
	return ITEM_OK;
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
