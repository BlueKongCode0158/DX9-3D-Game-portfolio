#include "Level.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Deviec(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Deviec);
}

HRESULT CLevel::NativeConstruct()
{
	return S_OK;
}

_int CLevel::Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pGraphic_Deviec);
}
