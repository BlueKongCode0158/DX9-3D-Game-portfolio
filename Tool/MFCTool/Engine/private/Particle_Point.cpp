#include "..\public\Particle_Point.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"


CParticle_Point::CParticle_Point(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticle(pGraphic_Device)
{
}

CParticle_Point::CParticle_Point(const CParticle_Point & rhs)
	:CParticle(rhs)
	,m_tInfo(rhs.m_tInfo)
	,m_tParticleState(rhs.m_tParticleState)
{
	Safe_AddRef(rhs.m_pPrototypeSystem);
}

HRESULT CParticle_Point::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Point::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CParticle_Point::Tick(_float fTimeDelta)
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); )
	{
		if (m_tParticleState.m_isLoop == true)
		{
			(*iter)->Update(fTimeDelta);
			(*iter)->Reset();
			iter++;
		}
		else
		{
			if (PARTICLE_DEAD == (*iter)->Update(fTimeDelta))
			{
				Safe_Release(*iter);
				iter = m_pAttributeList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	m_pVIBuffer->Update(m_pAttributeList);
	return PARTICLE_ALIVE;
}

HRESULT CParticle_Point::Render()
{
	if (FAILED(SetUp_ConstantTable()))

	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(m_iShaderNumber);
	m_pVIBuffer->Render_VIBuffer();
	m_pShaderCom->End_Shader();
	return S_OK;
}

HRESULT CParticle_Point::SetUp_ConstantTable()
{
	return S_OK;
}

void CParticle_Point::Set_Position(_float3 vPosition)
{
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition);
}

_bool CParticle_Point::Set_Index(_int iIndex)
{
	return __super::Set_Index(iIndex);
}

CComponent * CParticle_Point::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle_Point* pInstance = new CParticle_Point(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CParticle_Point::Clone(void * pArg)
{
	CParticle_Point* pInstance = new CParticle_Point(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CParticle_Point::Free()
{
	__super::Free();

	Safe_Release(m_pPrototypeSystem);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransform);
	for (auto& iter : m_pAttributeList)
	{
		Safe_Release(iter);
	}
	m_pAttributeList.clear();
}
