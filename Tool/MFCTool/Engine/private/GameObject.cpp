#include "..\public\GameObject.h"
#include "GameInstacne.h"



CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphicDevice(pGraphic_Device)
{
	Safe_AddRef(m_pGraphicDevice);
}

CGameObject::CGameObject(const CGameObject & rhs)
	:m_pGraphicDevice(rhs.m_pGraphicDevice)
{
	Safe_AddRef(m_pGraphicDevice);
}

_float CGameObject::Get_CamDistance() const
{
	return m_fCamDistance;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
	{
		return E_FAIL;
	}

	CGameInstacne*		pInstance = CGameInstacne::Get_Instance();
	if (nullptr == pInstance)
	{
		return E_FAIL;
	}
	Safe_AddRef(pInstance);

	CComponent*	pComponent = pInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	m_Componenets.emplace(pComponentTag, pComponent);

	Safe_AddRef(pComponent);
	*ppOut = pComponent;
	Safe_Release(pInstance);
	
	return S_OK;
}

CComponent * CGameObject::Find_ObjectComponent(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CGameObject::Late_Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::Compute_CameraDistance(CTransform * pTransform)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	_float3	vWorldPosition = pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	_matrix	ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	_matrix CamWorldMatrix = *D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
	_float3	vCamPosition = *reinterpret_cast<_float3*>(&CamWorldMatrix.m[3][0]);
	
	m_fCamDistance = D3DXVec3Length(&(vWorldPosition - vCamPosition));

	RELEASE_INSTANCE(CGameInstacne);
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto iter = find_if(m_Componenets.begin(), m_Componenets.end(), CTagFinder(pComponentTag));

	if (m_Componenets.end() == iter)
		return nullptr;
	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Componenets)
		Safe_Release(Pair.second);
	m_Componenets.clear();

	Safe_Release(m_pGraphicDevice);
}
