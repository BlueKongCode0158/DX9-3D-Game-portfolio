#include "..\public\GameObject_Manager.h"
#include "GameObject.h"
#include "Layer.h"


IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
{
}

void CGameObject_Manager::Clear(_uint iLevel)
{
	if (m_iNumLevel <= iLevel)
	{
		return;
	}
	for (auto& Pair : m_pLayers[iLevel])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevel].clear();
}

HRESULT CGameObject_Manager::Reserve_Container_ProtoType(_uint iTypeNum)
{
	if (nullptr != m_pLayers)
	{
		MSGBOX("Failed to Reserve Container ProtoType");
		return E_FAIL;
	}
	m_pPrototypes = new PROTOTYPE[iTypeNum];
	
	m_iNumLevel = iTypeNum;

	return S_OK;
}

HRESULT CGameObject_Manager::Reserve_Container(_uint iNumLevel)
{
	if (nullptr != m_pLayers)
	{
		MSGBOX("Failed to Reserve Container Clone");
		return E_FAIL;
	}
	m_pLayers = new LAYER[iNumLevel];

	m_iNumLevel = iNumLevel;
	return S_OK;
}

HRESULT CGameObject_Manager::Add_Prototype(_uint iTypeNum, const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject  ||
		iTypeNum >= m_iNumLevel ||
		nullptr != Find_Prototype(iTypeNum, pPrototypeTag))
	{
		if (nullptr != pGameObject)
		{
			Safe_Release(pGameObject);
		}
		return E_FAIL;
	}

	m_pPrototypes[iTypeNum].emplace(pPrototypeTag,pGameObject);
	return S_OK;
}

HRESULT CGameObject_Manager::Add_GameObject_Clone(_uint iNumLevel, const _tchar * pPrototypeTag, const _tchar* pLayerTag, void * pArg /* = nullptr*/)
{
	if (m_iNumLevel <= iNumLevel)
	{
		MSGBOX("Add GameObject Clone - Container vector space over");
		return E_FAIL;
	}

	CGameObject* pPrototype = Find_Prototype(iNumLevel, pPrototypeTag);
	if (nullptr == pPrototype)
	{
		//MSGBOX("Add GameObject Clone - pPrototype Pointer nullptr");
		return E_FAIL;
	}

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
	{
		//MSGBOX("Add GameObject Clone - pGameObject Pointer nullptr");
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(iNumLevel, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
		{
			//MSGBOX("Add GameObject Clone - pLayer Pointer nullptr");
			return E_FAIL;
		}
		pLayer->Add_GameObject(pGameObject);
		m_pLayers[iNumLevel].emplace(pLayerTag, pLayer);
	}
	else
	{
		pLayer->Add_GameObject(pGameObject);
	}

	return S_OK;
}

CGameObject * CGameObject_Manager::Find_CloneObject(_uint iTypeNum, const _tchar * pLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iTypeNum, pLayerTag);

	if (nullptr == pLayer)
	{
		return nullptr;
	}
	CGameObject* pObject = pLayer->Get_GameObjectPointer(iIndex);
	return pObject;
}

HRESULT CGameObject_Manager::Delete_Prototype(_uint iTypeNum, const _tchar * pPrototypeTag)
{
	if (iTypeNum >= m_iNumLevel)
	{
		return E_FAIL;
	}
	

	CGameObject* pPrototype = Find_Prototype(iTypeNum, pPrototypeTag);
	if (nullptr == pPrototype)
	{
		//MSGBOX("Add GameObject Clone - pPrototype Pointer nullptr");
		return E_FAIL;
	}

	Safe_Release(pPrototype);
	m_pPrototypes[iTypeNum].erase(pPrototypeTag);
	
	return S_OK;
	
}

HRESULT CGameObject_Manager::Delete_GameObject(_uint iTypeNum, const _tchar * pLayerTag, _int iIndex)
{
	if (iTypeNum >= m_iNumLevel)
	{
		return E_FAIL;
	}

	CGameObject* pObj = Find_GameObject(iTypeNum, pLayerTag, iIndex);
	if (nullptr == pObj)
	{
		MSGBOX("is Empty Pointer");
		return E_FAIL;
	}
	Safe_Release(pObj);
	return S_OK;
}

_uint CGameObject_Manager::Get_GameObject_ListSize(_uint iLevelIndex, const _tchar * pLayerTag)
{
	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		return 0;
	}

	return pLayer->Get_ListSize();
}

_int CGameObject_Manager::Tick(_double TimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(TimeDelta);
		}
	}
	return _int();
}

_int CGameObject_Manager::Late_Tick(_double TimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Late_Tick(TimeDelta);
		}
	}
	return _int();
}

CGameObject * CGameObject_Manager::Find_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _int iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		return nullptr;
	}
	return pLayer->Get_GameObjectPointer(iIndex);
}

CComponent * CGameObject_Manager::Find_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndexNum)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		return nullptr;
	}

	return pLayer->Find_Component(pComponentTag,iIndexNum);
}

CGameObject * CGameObject_Manager::Find_Prototype(_uint iTypeNum, const _tchar * pPrototypeTag)
{

	auto iter = find_if(m_pPrototypes[iTypeNum].begin(), m_pPrototypes[iTypeNum].end() , CTagFinder(pPrototypeTag));

	if (m_pPrototypes[iTypeNum].end() == iter)
	{
		//MSGBOX("Failed to Find GameObject_ProtoType");
		return nullptr;
	}
	return iter->second;
}

CLayer * CGameObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (m_pLayers[iLevelIndex].end() == iter)
	{
		//MSGBOX("Faild to Find GameObject_Layer");
		return nullptr;
	}
	return iter->second;
}

void CGameObject_Manager::Free()
{
	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			Safe_Release(Pair.second);
		}
		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pPrototypes);
	/* 이거 해주는 거 맞다고 생각하는데 한번 더 고민해보자 10.19*/
}
