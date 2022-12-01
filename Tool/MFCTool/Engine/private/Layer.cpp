#include "..\public\Layer.h"
#include "Component.h"



CLayer::CLayer()
{
}

CGameObject * CLayer::Get_GameObjectPointer(_uint iIndex)
{
	if (m_Objects.size() <= iIndex)
	{
		return nullptr;
	}
	auto iter = m_Objects.begin();
	for (size_t i = 0; i < iIndex; i++)
	{
		iter++;
	}
	return (*iter);
}

CComponent * CLayer::Find_Component(const _tchar * ComponentTag, _uint iIndexNum)
{
	if (m_Objects.size() <= iIndexNum)
	{
		return nullptr;
	}

	auto iter = m_Objects.begin();
	for (size_t i = 0; i < iIndexNum; i++)
	{
		iter++;
	}

	return (*iter)->Find_ObjectComponent(ComponentTag);
}

HRESULT CLayer::Delete_GameObject(_uint iIndex)
{
	if (m_Objects.size() <= iIndex)
	{
		return E_FAIL;
	}
	auto iter = m_Objects.begin();
	for (size_t i = 0; i < iIndex; i++)
	{
		iter++;
	}
	Safe_Release(*iter);
	m_Objects.erase(iter);
	return S_OK;
}

HRESULT CLayer::Delete_GameObjects()
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end(); )
	{
		if ((*iter)->IsDead() == true)
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else
		{

		}
	}
	return S_OK;
}


HRESULT CLayer::NativeConstruct()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
	{
		return E_FAIL;
	}

	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	for (list<CGameObject*>::iterator& iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		_int iEvent = (*iter)->Tick(TimeDelta);

		if (CGameObject::STATE::OBJ_DEAD == iEvent)
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return _int();
}

_int CLayer::Late_Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		pGameObject->Late_Tick(TimeDelta);
	}
	return _int();
}

_bool CLayer::IsEmpty()
{
	return m_Objects.empty();
}

CLayer * CLayer::Create()
{
	CLayer* pInstance = new CLayer();
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
	{
		Safe_Release(pGameObject);
	}
	m_Objects.clear();
}
