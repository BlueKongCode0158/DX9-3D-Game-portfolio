#include "..\public\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)
CComponent_Manager::CComponent_Manager()
{

}

void CComponent_Manager::Clear(_uint iLevel)
{
	if (iLevel >= m_iNumLevel)
	{
		MSGBOX(" iLevel >= m_iNumLevel, Component Manager");
		return;
	}
	for (auto& Pair : m_pProtoType[iLevel])
	{
		Safe_Release(Pair.second);
	}
	m_pProtoType[iLevel].clear();
}

HRESULT CComponent_Manager::Reserve_Container(_uint iNumLevel)
{
	if (nullptr != m_pProtoType)
	{
		MSGBOX("Failed to Reserve Container_Component Manager");
		return E_FAIL;
	}
	m_pProtoType = new PROTOTYPES[iNumLevel];

	m_iNumLevel = iNumLevel;
	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == pPrototype ||
		m_iNumLevel <= iLevelIndex ||
		nullptr != Find_Component(iLevelIndex, pPrototypeTag))
	{
		if (nullptr != pPrototype)
		{
			Safe_Release(pPrototype);
		}
		return E_FAIL;
	}

	m_pProtoType[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (m_iNumLevel <= iLevelIndex)
	{
		MSGBOX("Failed to Clone Component_ Index over");
		return nullptr;
	}

	CComponent*		pPrototype = Find_Component(iLevelIndex, pPrototypeTag);

	if (nullptr == pPrototype)
	{
		MSGBOX("Failed to Clone Component_ Pointer nullptr");
		return nullptr;
	}

	CComponent*		pComponent = pPrototype->Clone(pArg);

	return pComponent;
}

CComponent * CComponent_Manager::Find_Component(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_pProtoType[iLevelIndex].begin(), m_pProtoType[iLevelIndex].end(), CTagFinder(pPrototypeTag));

	if (m_pProtoType[iLevelIndex].end() == iter)
	{
		//MSGBOX("Failed to Find_Component iter == End()");
		return nullptr;
	}
	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Pair : m_pProtoType[i])
		{
			Safe_Release(Pair.second);
		}
		m_pProtoType[i].clear();
	}

	Safe_Delete_Array(m_pProtoType);
}
