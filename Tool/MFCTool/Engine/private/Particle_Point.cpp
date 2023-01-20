#include "..\public\Particle_Point.h"
#include "Attribute_Layer.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Shader.h"


CParticle_Point::CParticle_Point(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticle(pGraphic_Device)
{
}

CParticle_Point::CParticle_Point(const CParticle_Point & rhs)
	:CParticle(rhs)
{
}

HRESULT CParticle_Point::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Point::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CParticle_Point::Render()
{
	return S_OK;
}

_int CParticle_Point::Tick(_float fTimeDelta)
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); )
	{
		if (m_isLoop == true)
		{
			(*iter)->Reset();
			iter++;
			continue;
		}
		else
		{
			if (PARTICLE_DEAD == (*iter)->Tick(fTimeDelta))
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
	return 0;
}

_int CParticle_Point::Reset()
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		if ((*iter)->GetAlive())
		{
			continue;
		}
		(*iter)->Reset();
	}
	return 0;
}

_int CParticle_Point::Set_Transform(_matrix tMatrix)
{
	return _int();
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
	for (auto iter : m_pAttributeList)
	{
		Safe_Release(*iter);
	}
	m_pAttributeList.clear();
}
