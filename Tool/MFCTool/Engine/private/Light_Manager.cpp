#include "..\public\Light_Manager.h"
#include "Light.h"


IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

D3DLIGHT9 CLight_Manager::Get_LightDesc(_uint iIndex)
{
	list<CLight*>::iterator iter = m_vecLights.begin();

	for (_uint i = 0; i < iIndex; i++)
	{
		iter++;
	}

	if (m_vecLights.end() == iter)
	{
		return D3DLIGHT9();
	}

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 & LightDesc)
{
	CLight*		pLight = CLight::Create(LightDesc,pGraphic_Device);
	if (nullptr == pLight)
	{
		return E_FAIL;
	}
	m_vecLights.push_back(pLight);
	return S_OK;
}

HRESULT CLight_Manager::Render()
{
	for (auto& pLight : m_vecLights)
	{
		pLight->Render();
	}
	return S_OK;
}

HRESULT CLight_Manager::Level_Relese_Light()
{
	for (auto& pLight : m_vecLights)
	{
		Safe_Release(pLight);
	}
	m_vecLights.clear();

	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_vecLights)
	{
		Safe_Release(pLight);
	}

	m_vecLights.clear();
}
