#include "..\public\ParticleSystem_Manager.h"
#include "Graphic_Device.h"
#include "Particle_System.h"


IMPLEMENT_SINGLETON(CParticleSystem_Manager)
CParticleSystem_Manager::CParticleSystem_Manager()
{
}


CParticleSystem_Manager::~CParticleSystem_Manager()
{
}

HRESULT CParticleSystem_Manager::Add_ParticleSystem(const _tchar * pLayerTag)
{
	auto pParticleObject = Find_PrototypeParticle(pLayerTag);
	if (nullptr == pParticleObject)
	{
		CGraphic_Device* pGraphicDevice = GET_INSTANCE(CGraphic_Device);
		CParticle_System* pInstance = CParticle_System::Create(pGraphicDevice->Get_Device());
		RELEASE_INSTANCE(CGraphic_Device);
		m_mapPrototypeParticles.emplace(pLayerTag, pInstance);
		Add_Layer(pLayerTag,nullptr);
	}
	else
	{
		Add_Layer(pLayerTag, nullptr);
	}

	return S_OK;
}

HRESULT CParticleSystem_Manager::Add_ParticleSystem(const _tchar * pLayerTag, void * pArg)
{
	auto pParticleObject = Find_PrototypeParticle(pLayerTag);
	if (nullptr == pParticleObject)
	{
		CGraphic_Device* pGraphicDevice = GET_INSTANCE(CGraphic_Device);
		CParticle_System* pInstance = CParticle_System::Create(pGraphicDevice->Get_Device());
		RELEASE_INSTANCE(CGraphic_Device);
		m_mapParticles.emplace(pLayerTag, pInstance);
		Add_Layer(pLayerTag, pArg);
	}
	else
	{
		Add_Layer(pLayerTag, pArg);
	}
	return S_OK;
}


HRESULT CParticleSystem_Manager::DeleteAll_ParticleSystem(const _tchar * pLayerTag)
{
	auto iter = Find_PrototypeParticle(pLayerTag);
	if (nullptr == iter)
	{
		return E_FAIL;
	}
	Safe_Release(*iter);
	m_mapPrototypeParticles.erase(pLayerTag);

	auto pLayer = Find_Particles(pLayerTag);
	if (nullptr == iter)
	{
		return E_FAIL;
	}
	
	for (auto pointer = pLayer->begin(); pointer != pLayer->end(); pointer++)
	{
		Safe_Release(*pointer);
	}
	pLayer->clear();
	m_mapParticles.erase(pLayerTag);

	return S_OK;
}

HRESULT CParticleSystem_Manager::Delete_ParticleSystem(const _tchar * pLayerTag, _int iIndex)
{
	auto pLayer = Find_Particles(pLayerTag);
	if (nullptr == pLayer)
	{
		return E_FAIL;
	}
	
	int iNum = 0;
	auto iter = pLayer->begin();
	for (iter ; iter != pLayer->end(); iter++, iNum++)
	{
		if (iNum == iIndex)
		{
			break;
		}
	}
	Safe_Release(*iter);

	if (pLayer->size() == 0)
	{
		pLayer->clear();
		m_mapParticles.erase(pLayerTag);
	}
	return S_OK;
}

CParticle_System * CParticleSystem_Manager::Find_PrototypeParticle(const _tchar * pLayerTag)
{
	auto iter = find_if(m_mapPrototypeParticles.begin(), m_mapPrototypeParticles.end(), CTagFinder(pLayerTag));
	if (m_mapPrototypeParticles.end() == iter)
	{
		return nullptr;
	}
	return (*iter).second;
}

list<CParticle_System*>* CParticleSystem_Manager::Find_Particles(const _tchar * pLayerTag)
{
	auto iter = find_if(m_mapParticles.begin(), m_mapParticles.end(), CTagFinder(pLayerTag));
	if (m_mapParticles.end() == iter)
	{
		return nullptr;
	}
	return &(*iter).second;
}

void CParticleSystem_Manager::Update_Index()
{
	for (auto Pair : m_mapParticles)
	{
		auto iter = Pair.second.begin();
		_int iStartIndex = 1;

		for (iter; iter != Pair.second.end(); iter++)
		{
			(*iter)->Set_Index(iStartIndex);
		}
	}
}

HRESULT CParticleSystem_Manager::Add_Layer(const _tchar * pLayerTag,void* pArg)
{
	list<CParticle_System*>* pLayer = Find_Particles(pLayerTag);
	CParticle_System*	pParticle = Find_PrototypeParticle(pLayerTag);
	CParticle_System*	pClone = pParticle->Clone(pArg);

	if (nullptr == pClone)
	{
		return E_FAIL;
	}

	if (nullptr == pLayer)
	{
		vector<CParticle_System*> pInstance;
		pInstance.push_back(pClone);
		m_mapParticles.emplace(pLayerTag, pInstance);
	}
	else
	{
		pLayer->push_back(pClone);
	}
	return S_OK;
}

void CParticleSystem_Manager::Free()
{
	for (auto Pair : m_mapParticles)
	{
		for (auto iter = Pair.second.begin(); iter != Pair.second.end(); iter++)
		{
			Safe_Release(*iter);
		}
		Pair.second.clear();
	}
	m_mapParticles.clear();

	for (auto Pair : m_mapPrototypeParticles)
	{
		Safe_Release(Pair.second);
	}
	m_mapPrototypeParticles.clear();
}
