#include "..\public\Attribute_Layer.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"

CAttribute_Layer::CAttribute_Layer()
{
}

CAttribute_Layer::CAttribute_Layer(const CAttribute_Layer & rhs)
	:m_pTransformCom(dynamic_cast<CTransform*>(rhs.m_pTransformCom->Clone()))
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
	m_tInfo.m_fDuration += fTimeDelta;

	if (m_tInfo.m_fAge <= m_tInfo.m_fDuration)
	{
		return PARTICLE_DEAD;
	}

	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		(*iter)->Update(fTimeDelta);
	}
	
	// Attribute 의 역할은 Point 위치를 옮겨주는 것에 있다.
	// 정보값은 Attribute가 가지고 있고 해당 정보값을 넘겨주는 걸로 VIBuffer가 작동하고 있음.
	m_pVIBufferCom->Tick(fTimeDelta, m_pAttributeList);
	return PARTICLE_ALIVE;
}

HRESULT CAttribute_Layer::Render(CShader* pShader)
{
	if (FAILED(SetUp_ConstantTable()))

	{
		return E_FAIL;
	}

	pShader->Begin_Shader(0);
	m_pVIBufferCom->Render();
	pShader->End_Shader();
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
	for (auto& pointer : m_pAttributeList)
	{
		Safe_Release(pointer);
	}
	m_pAttributeList.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
