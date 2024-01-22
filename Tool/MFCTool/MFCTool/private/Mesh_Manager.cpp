#include "stdafx.h"
#include "..\public\Mesh_Manager.h"
#include "GameInstacne.h"


IMPLEMENT_SINGLETON(CMesh_Manager)
CMesh_Manager::CMesh_Manager()
{
}


CMesh_Manager::~CMesh_Manager()
{
}

HRESULT CMesh_Manager::Add_StaticMeshList(const _tchar * pLayerTag, CGameObject * pMesh)
{
	if (nullptr == pLayerTag ||
		nullptr == pMesh)
	{
		return E_FAIL;
	}

	CMeshList*	pMeshList = Find_StaticMeshList(pLayerTag);

	if (nullptr == pMeshList)
	{
		CMeshList* pList = CMeshList::Create();

		pList->Add_Mesh(pMesh);
		m_mapStaticMeshList.emplace(pLayerTag, pList);
	}
	else
	{
		pMeshList->Add_Mesh(pMesh);
	}

	return S_OK;
}

HRESULT CMesh_Manager::Assert_StaticMeshList(const _tchar * pLayerTag, CGameObject * pMesh)
{
	if (nullptr == pLayerTag ||
		nullptr == pMesh)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMesh_Manager::Add_DynamicMeshList(const _tchar * pLayerTag, CGameObject * pMesh)
{
	if (nullptr == pLayerTag ||
		nullptr == pMesh)
	{
		return E_FAIL;
	}

	CMeshList*	pMeshList = Find_DynamicMeshList(pLayerTag);

	if (nullptr == pMeshList)
	{
		CMeshList* pList = CMeshList::Create();

		pList->Add_Mesh(pMesh);
		m_mapDynamicMeshList.emplace(pLayerTag, pList);
	}
	else
	{
		pMeshList->Add_Mesh(pMesh);
	}

	return S_OK;
}

HRESULT CMesh_Manager::Assert_DynamicMeshList(const _tchar * pLayerTag, CGameObject * pMesh)
{
	return S_OK;
}

_uint CMesh_Manager::Get_StaticMapVectorSize(const _tchar * pLayerTag)
{
	return Find_StaticMeshList_Size(pLayerTag);
}

_uint CMesh_Manager::Get_DynamicMapVectorSize(const _tchar * pLayerTag)
{
	return Find_DynamicMeshList_Size(pLayerTag);
}

CMeshList * CMesh_Manager::Find_StaticMeshList(const _tchar * pLayerTag)
{
	auto pMeshlist = find_if(m_mapStaticMeshList.begin(), m_mapStaticMeshList.end(), CTagFinder(pLayerTag));

	if (pMeshlist == m_mapStaticMeshList.end())
	{
		return nullptr;
	}
	return pMeshlist->second;
}

_uint CMesh_Manager::Find_StaticMeshList_Size(const _tchar * pLayerTag)
{
	if (nullptr == pLayerTag)
	{
		return 0;
	}

	CMeshList*	pMeshList = Find_StaticMeshList(pLayerTag);

	if (nullptr == pMeshList)
	{
		return 0;
	}
	else
	{
		return pMeshList->Get_vecMeshList_Size();
	}
	return _uint();
}

CMeshList * CMesh_Manager::Find_DynamicMeshList(const _tchar * pLayerTag)
{
	auto pMeshlist = find_if(m_mapDynamicMeshList.begin(), m_mapDynamicMeshList.end(), CTagFinder(pLayerTag));

	if (pMeshlist == m_mapDynamicMeshList.end())
	{
		return nullptr;
	}
	return pMeshlist->second;
}

_uint CMesh_Manager::Find_DynamicMeshList_Size(const _tchar * pLayerTag)
{
	if (nullptr == pLayerTag)
	{
		return 0;
	}

	CMeshList*	pMeshList = Find_DynamicMeshList(pLayerTag);

	if (nullptr == pMeshList)
	{
		return 0;
	}
	else
	{
		return pMeshList->Get_vecMeshList_Size();
	}
}

HRESULT CMesh_Manager::Save_StaticMeshList(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CMesh_Manager::Save_DynamicMeshList(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

void CMesh_Manager::Free()
{
	for (auto& pPair : m_mapStaticMeshList)
	{
		Safe_Release(pPair.second);
	}
	m_mapStaticMeshList.clear();
	
	for (auto& pPair : m_mapDynamicMeshList)
	{
		Safe_Release(pPair.second);
	}
	m_mapDynamicMeshList.clear();
}
