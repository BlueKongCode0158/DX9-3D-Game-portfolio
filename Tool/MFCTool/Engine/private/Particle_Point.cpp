#include "..\public\Particle_Point.h"
#include "PipeLine.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"
#include "Texture.h"


CParticle_Point::CParticle_Point(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticle(pGraphic_Device)
{
}

CParticle_Point::CParticle_Point(const CParticle_Point & rhs)
	:CParticle(rhs)
	,m_tInfo(rhs.m_tInfo)
	,m_tParticleState(rhs.m_tParticleState)
	,m_pPrototypeSystem(rhs.m_pPrototypeSystem)
{
	Safe_AddRef(rhs.m_pPrototypeSystem);
}

HRESULT CParticle_Point::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Point::NativeConstruct_Prototype(void* pArg)
{
	if (nullptr != pArg)
	{
		m_pPrototypeSystem = CAttribute::Create(pArg);
	}
	return S_OK;
}

HRESULT CParticle_Point::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tParticleState, pArg, sizeof(PSDESC));
	}

	return S_OK;
}

_int CParticle_Point::Tick(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	Create_Attribute();

	Update_List(fTimeDelta);
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
	m_pVIBuffer->Render_VIBuffer(m_pAttributeList.size());
	m_pShaderCom->End_Shader();
	return S_OK;
}

HRESULT CParticle_Point::SetUp_ConstantTable()
{
	CPipeLine*	pPipeLine = GET_INSTANCE(CPipeLine);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransform->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pPipeLine->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pPipeLine->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTexture);
	RELEASE_INSTANCE(CPipeLine);
	return S_OK;
}

void CParticle_Point::Create_Attribute()
{
	if (nullptr == m_pPrototypeSystem)
	{
		MSGBOX("현재 m_pPrototypeSystem이 할당 되어있지 않습니다.");
	}

	if (m_tParticleState.m_isLoop == true)
	{
		if (m_fAccTime >= m_tParticleState.m_fEmissionTime && m_tParticleState.m_iMaxParticle >= m_pAttributeList.size())
		{
			for (int i = 0; i < m_iCurInitCount; i++)
			{
				CAttribute*	pAttribute = m_pPrototypeSystem->Clone(&m_tInfo);
				m_pAttributeList.push_back(pAttribute);
			}
			m_fAccTime = 0.f;
		}
	}
}

void CParticle_Point::Update_List(_float fTimeDelta)
{
	for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); )
	{
		if (CAttribute::ATTRIBUTE_DEAD == (*iter)->Update(fTimeDelta))
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

void CParticle_Point::Set_Position(_float3 vPosition)
{
	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition);
}

_bool CParticle_Point::Set_Index(_int iIndex)
{
	return __super::Set_Index(iIndex);
}

_bool CParticle_Point::Set_ParticleInfo(PDESC* tPInfo)
{
	memcpy(&m_tInfo, tPInfo, sizeof(PDESC));
	return true;
}

CComponent * CParticle_Point::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CParticle_Point* pInstance = new CParticle_Point(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
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
	Safe_Release(m_pTexture);
	for (auto& iter : m_pAttributeList)
	{
		Safe_Release(iter);
	}
	m_pAttributeList.clear();
}
