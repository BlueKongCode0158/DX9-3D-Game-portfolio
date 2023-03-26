#include "..\public\Attribute.h"

CAttribute::CAttribute()
{
}

CAttribute::CAttribute(const CAttribute& rhs)
	: m_tCreateDesc(rhs.m_tCreateDesc)
{
}

HRESULT CAttribute::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CAttribute::NativeConstruct()
{
	memcpy(&m_tCurretnDesc, &m_tCreateDesc, sizeof(PDESC));

	Engine::GetRandomVector(m_tCreateDesc.m_vDir, m_tCurretnDesc.m_vDirStart, m_tCreateDesc.m_vDirEnd);
	m_tCurretnDesc.m_vDir = m_tCreateDesc.m_vDir;

	return S_OK;
}

_int CAttribute::Reset()
{
	if (m_tCreateDesc.m_vPosition != m_tCurretnDesc.m_vPosition)
	{
		m_tCurretnDesc.m_vPosition = m_tCreateDesc.m_vPosition;
	}
	return 0;
}

_int CAttribute::Update(_float Time_Delta)
{
	if (m_tCreateDesc.m_fAge > m_tCurretnDesc.m_fLifeTime)
	{
		m_tCreateDesc.m_isAlive = false;
		return -1;
	}

	m_tCurretnDesc.m_vPosition += m_tCurretnDesc.m_vDir * Time_Delta * m_tCurretnDesc.m_fAcceleration;
	m_tCurretnDesc.m_fAge += Time_Delta;
	return 0;
}

CAttribute * CAttribute::Create()
{
	CAttribute* pInstance = new CAttribute();
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CAttribute * CAttribute::Clone()
{
	CAttribute* pInstance = new CAttribute(*this);
	if (FAILED(pInstance->NativeConstruct()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAttribute::Free()
{

}
