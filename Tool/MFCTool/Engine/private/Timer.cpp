#include "..\public\Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Time()
{
	QueryPerformanceFrequency(&m_tCupTick);
	QueryPerformanceCounter(&m_tBegin);
	QueryPerformanceCounter(&m_tEnd);
	QueryPerformanceCounter(&m_tInvalidate);
	return S_OK;
}

void CTimer::Update_Time()
{
	QueryPerformanceCounter(&m_tEnd);

	if ((m_tEnd.QuadPart - m_tInvalidate.QuadPart) > m_tCupTick.QuadPart)
	{
		m_tInvalidate = m_tEnd;
		QueryPerformanceFrequency(&m_tCupTick);
	}

	m_fTimeDelta = static_cast<float>(m_tEnd.QuadPart - m_tBegin.QuadPart) / m_tCupTick.QuadPart;
	
	m_tBegin = m_tEnd;
}

CTimer * CTimer::Create()
{
	CTimer* pInstance = new CTimer();
	if (FAILED(pInstance->Ready_Time()))
	{
		MSGBOX("Failed To Creating Timer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{
}
