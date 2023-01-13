#include "..\public\Attribute_Layer.h"
#include "VIBuffer_Point.h"
#include "Attribute.h"



CAttribute_Layer::CAttribute_Layer()
{
}

HRESULT CAttribute_Layer::NativeConstruct()
{
	return S_OK;
}

_int CAttribute_Layer::Tick(_float fTimeDelta)
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		(*iter)->Update(fTimeDelta);
	}

	m_pVIBufferCom->Tick(fTimeDelta, m_pAttributeList);
	return 0;
}

HRESULT CAttribute_Layer::Render()
{
	return S_OK;
}

HRESULT CAttribute_Layer::SetUp_ConstantTable()
{
	return S_OK;
}

CAttribute_Layer * CAttribute_Layer::Create()
{
	CAttribute_Layer* pInstance = new CAttribute_Layer();
	if (FAILED(pInstance->NativeConstruct()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAttribute_Layer::Free()
{
	for (auto pointer : m_pAttributeList)
	{
		Safe_Release(*pointer);
	}
	m_pAttributeList.clear();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
