#include "..\public\Item.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(pGraphic_Device);
}

CItem::CItem(const CItem & rhs)
	:m_pGraphic_Device(rhs.m_pGraphic_Device),
	 m_OrhProjMatrix(rhs.m_OrhProjMatrix)
{
	Safe_AddRef(m_pGraphic_Device);
	D3DXMatrixIdentity(&m_OrhProjMatrix);
}

HRESULT CItem::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CItem::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CItem::Free()
{
	Safe_Release(m_pGraphic_Device);
}
