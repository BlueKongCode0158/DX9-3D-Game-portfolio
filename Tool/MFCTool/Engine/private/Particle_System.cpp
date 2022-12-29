#include "..\public\Particle_System.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"

CParticle_System::CParticle_System(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CParticle_System::CParticle_System(const CParticle_System & rhs)
	:m_pGraphic_Device(rhs.m_pGraphic_Device)
{
}

HRESULT CParticle_System::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle_System::NativeConstruct(void * pArg)
{
	if (nullptr == pArg)
	{
		return E_FAIL;
	}
	_float3	vPosition = *static_cast<_float3*>(pArg);
	m_pTransfomCom->Set_WorldMatrixRow(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CParticle_System::Reset()
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		if ((*iter)->GetAlive())
		{
			continue;
		}
		(*iter)->Reset();
	}
	return S_OK;
}

_int CParticle_System::Tick(_float Time_Delta)
{
	Reset();
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); iter++)
	{
		(*iter)->Update(Time_Delta);
	}
	m_pVIBufferCom->Tick(Time_Delta, m_pAttributeList);
	return 0;
}
  
_int CParticle_System::Late_Tick(_float Time_Delta)
{
	return 0;
}

HRESULT CParticle_System::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();
	return S_OK;
}

_bool CParticle_System::Set_ParticleDesc(PSDESC* pArg)
{
	if (nullptr == pArg)
	{
		return false;
	}

	m_tDesc = *pArg;
	return true;
}

HRESULT CParticle_System::SetUp_ConstantTable()
{
	return S_OK;
}

CParticle_System * CParticle_System::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle_System* pInstance = new CParticle_System(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CParticle_System::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransfomCom);
	Safe_Release(m_pShaderCom);

	for (auto iter : m_pAttributeList)
	{
		Safe_Release(iter);
	}
	m_pAttributeList.clear();
}
