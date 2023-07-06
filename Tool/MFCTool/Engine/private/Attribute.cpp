#include "..\public\Attribute.h"

CAttribute::CAttribute()
{
}

CAttribute::CAttribute(const CAttribute& rhs)
	:m_tInfo(rhs.m_tInfo)
{
}

HRESULT CAttribute::NativeConstruct_Prototype(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tInfo, pArg, sizeof(PDESC));
		GetRandomVector(m_tInfo.m_vPosition, m_tInfo.m_vMinPos, m_tInfo.m_vMaxPos);
	}
	return S_OK;
}

HRESULT CAttribute::NativeConstruct()
{
	return S_OK;
}

_int CAttribute::Update(_float Time_Delta)
{
	m_tInfo.m_fAge += Time_Delta;
	if (m_tInfo.m_fAge > m_tInfo.m_fDuration)
	{
		return ATTRIBUTE_DEAD;
	}

	Move_Position(Time_Delta);
	Fade_Color(Time_Delta);
	return ATTRIBUTE_ALIVE;
}

void CAttribute::Move_Position(_float Time_Delta)
{
	m_tInfo.m_vPosition.x += m_tInfo.m_vDir.x * m_tInfo.m_vVelocity.x * Time_Delta;
	m_tInfo.m_vPosition.y += m_tInfo.m_vDir.y * m_tInfo.m_vVelocity.y * Time_Delta;
	m_tInfo.m_vPosition.z += m_tInfo.m_vDir.z * m_tInfo.m_vVelocity.z * Time_Delta;
}

void CAttribute::Fade_Color(_float Time_Delta)
{
	_float3 vNormalize = _float3(0.f, 0.f, 0.f);
	D3DXVec3Normalize(&vNormalize, &m_tInfo.m_vEndColor);

	m_tInfo.m_vColor.x += vNormalize.x * m_tInfo.m_fColorFade * Time_Delta;
	m_tInfo.m_vColor.y += vNormalize.y * m_tInfo.m_fColorFade * Time_Delta;
	m_tInfo.m_vColor.z += vNormalize.z * m_tInfo.m_fColorFade * Time_Delta;
}

CAttribute * CAttribute::Create(void* pArg)
{
	CAttribute* pInstance = new CAttribute();
	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
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
