#include "..\public\Attribute.h"

CAttribute::CAttribute()
{
}

CAttribute::CAttribute(const CAttribute& rhs)
{
}

HRESULT CAttribute::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CAttribute::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tInfo, pArg, sizeof(PDESC));
	}
	return S_OK;
}

_int CAttribute::Reset()
{
	if (m_tInfo.m_fAge > m_tInfo.m_fDuration)
	{
		m_tInfo.m_fAge		= 0.f;
		m_tInfo.m_vColor	= _float3(0.f, 0.f, 0.f);
		GetRandomVector(m_tInfo.m_vPosition, m_tInfo.m_vMinPos, m_tInfo.m_vMaxPos);
	}
	return 0;
}

_int CAttribute::Update(_float Time_Delta)
{
	m_tInfo.m_fAge += Time_Delta;
	if (m_tInfo.m_fAge > m_tInfo.m_fDuration)
	{
		return ATTRIBUTE_DEAD;
	}

	Move_Position();
	Fade_Color();
	return ATTRIBUTE_ALIVE;
}

void CAttribute::Move_Position()
{
	m_tInfo.m_vPosition.x += m_tInfo.m_vDir.x * m_tInfo.m_vVelocity.x;
	m_tInfo.m_vPosition.y += m_tInfo.m_vDir.y * m_tInfo.m_vVelocity.y;
	m_tInfo.m_vPosition.z += m_tInfo.m_vDir.z * m_tInfo.m_vVelocity.z;
}

void CAttribute::Fade_Color()
{
	_float3 vNormalize = _float3(0.f, 0.f, 0.f);
	D3DXVec3Normalize(&vNormalize, &m_tInfo.m_vEndColor);

	m_tInfo.m_vColor.x += vNormalize.x * m_tInfo.m_fColorFade;
	m_tInfo.m_vColor.y += vNormalize.y * m_tInfo.m_fColorFade;
	m_tInfo.m_vColor.z += vNormalize.z * m_tInfo.m_fColorFade;
}

CAttribute * CAttribute::Create()
{
	CAttribute* pInstance = new CAttribute();
	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CAttribute * CAttribute::Clone(void* pArg)
{
	CAttribute* pInstance = new CAttribute(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAttribute::Free()
{

}
