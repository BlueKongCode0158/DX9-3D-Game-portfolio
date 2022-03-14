#include "stdafx.h"
#include "..\public\MeshList.h"
#include "GameObject.h"

CMeshList::CMeshList()
{
}

HRESULT CMeshList::Add_Mesh(CGameObject * pGameObject)
{
	m_vecMeshList.push_back(pGameObject);
	Safe_AddRef(pGameObject);
	return S_OK;
}

HRESULT CMeshList::Assert_Mesh(_uint iIndex, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
	{
		return E_FAIL;
	}

	Safe_Release(m_vecMeshList[iIndex]);
	m_vecMeshList[iIndex] = pGameObject;

	return S_OK;
}


CMeshList * CMeshList::Create()
{
	CMeshList* pInstance = new CMeshList();
	if (nullptr == pInstance)
	{
		MSGBOX("Failed to Creating CMeshList_Prototype");
	}
	return pInstance;
}

void CMeshList::Free()
{
	for (auto& pGameObject : m_vecMeshList)
	{
		Safe_Release(pGameObject);
	}
	m_vecMeshList.clear();
}
