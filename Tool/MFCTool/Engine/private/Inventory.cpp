#include "..\public\Inventory.h"
#include "Item.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{
}

CInventory::CInventory(const CInventory & rhs)
	:CComponent(rhs)
{
}

HRESULT CInventory::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CInventory::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CInventory::Add_Item(const _tchar * pItemName, CItem * pItem)
{
	CItem* pItem_Element = Find_Item(pItemName);
	if (nullptr != pItem_Element)
	{
		Safe_Release(pItem);
	}


	return S_OK;
}

HRESULT CInventory::Use_Item(const _tchar * pItemName)
{
	return S_OK;
}

CItem * CInventory::Find_Item(const _tchar * pItemName)
{
	if (nullptr == pItemName)
	{
		return nullptr;
	}
	auto pItem = find_if(m_mapItems.begin(), m_mapItems.end(), CTagFinder(pItemName));

	if (pItem == m_mapItems.end())
	{
		return nullptr;
	}

	return pItem->second;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory* pInstance = new CInventory(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CInventory_Prototype");
	}
	return pInstance;
	//¤»¤»·ç»æ»Í
	//´©±¸¼¼¿ä?!
}

CComponent * CInventory::Clone(void * pArg)
{
	CInventory* pInstance = new CInventory(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CInventory_Clone");
	}
	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	for (auto Pair : m_mapItems)
	{
		Safe_Release(Pair.second);
	}
	m_mapItems.clear();

}
