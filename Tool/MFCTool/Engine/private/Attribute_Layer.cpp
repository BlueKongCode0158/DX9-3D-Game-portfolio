#include "..\public\Attribute_Layer.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"



CAttribute_Layer::CAttribute_Layer()
{
}

CAttribute_Layer::CAttribute_Layer(const CAttribute_Layer & rhs)
	:m_pShaderCom(dynamic_cast<CShader*>(rhs.m_pShaderCom->Clone()))
	,m_pTransformCom(dynamic_cast<CTransform*>(rhs.m_pTransformCom->Clone()))
	,m_pVIBufferCom(dynamic_cast<CVIBuffer_Point*>(rhs.m_pVIBufferCom->Clone(nullptr)))
{
}

HRESULT CAttribute_Layer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CAttribute_Layer::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CAttribute_Layer::Tick(_float fTimeDelta)
{
	m_tInfo.m_fLifeTime += fTimeDelta;

	if (m_tInfo.m_fAge <= m_tInfo.m_fLifeTime)
	{
		return PARTICLE_DEAD;
	}

	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		(*iter)->Update(fTimeDelta);
	}

	m_pVIBufferCom->Tick(fTimeDelta, m_pAttributeList);
	return PARTICLE_ALIVE;
}

HRESULT CAttribute_Layer::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render();
	m_pShaderCom->End_Shader();
	return S_OK;
}

void CAttribute_Layer::Reset()
{

}

HRESULT CAttribute_Layer::SetUp_ConstantTable()
{
	return S_OK;
}

CAttribute_Layer * CAttribute_Layer::Create()
{
	CAttribute_Layer* pInstance = new CAttribute_Layer();
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CAttribute_Layer * CAttribute_Layer::Clone(void* pArg)
{
	CAttribute_Layer* pInstance = new CAttribute_Layer();
	if (FAILED(pInstance->NativeConstruct(pArg)))
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
