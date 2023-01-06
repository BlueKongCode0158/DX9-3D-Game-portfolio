#include "..\public\Particle.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CParticle::CParticle(const CParticle & rhs)
	:CComponent(rhs)
{
}

HRESULT CParticle::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CParticle::Render()
{
	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();
	return S_OK;
}

_int CParticle::Tick(_float Time_Delta)
{
	// 간격 초기화
	if (m_fTime >= 1.f)
	{
		m_fTime = 0.f;
		m_tDesc.m_isEmission = true;
	}

	// 파티클 생성
	if (m_tDesc.m_isEmission == true && m_tDesc.m_iMaxParticle >= m_pAttributeList.size())
	{
		CAttribute* pObject = m_pPrototypeSystem->Clone();
		m_pAttributeList.push_back(pObject);
		m_tDesc.m_isEmission = false;
	}
	else
	{
		m_fTime += Time_Delta;
	}

	Reset();

	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		(*iter)->Update(Time_Delta);
	}

	// VIBuffer 초기화
	m_pVIBufferCom->Tick(Time_Delta, m_pAttributeList);

	return 0;
}

void CParticle::Set_Transform(_matrix ParentMatrix)
{
	m_pTransformCom->Set_WorldMatrix(ParentMatrix);
}

void CParticle::Reset()
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		if ((*iter)->GetAlive())
		{
			continue;
		}
		(*iter)->Reset();
	}
}

CComponent * CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CComponent* pInstance = new CParticle(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CParticle::Clone(void * pArg)
{
	CComponent* pInstance = new CParticle(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CParticle::Free()
{
	__super::Free();
	Safe_Release(m_pGraphic_Device);
}
