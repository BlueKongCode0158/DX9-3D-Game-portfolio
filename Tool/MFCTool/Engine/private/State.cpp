#include "..\public\State.h"

CState::CState(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

void CState::Free()
{
	Safe_Release(m_pGraphic_Device);
}
