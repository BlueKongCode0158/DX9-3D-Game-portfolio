#include "..\public\Decal.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer_Cube.h"
#include "Shader.h"

CDecal::CDecal(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CDecal::CDecal(const CDecal & rhs)
	:CGameObject(rhs)
{
}

HRESULT CDecal::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CDecal::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{

	}
	return S_OK;
}

HRESULT CDecal::Add_Component(_uint iLevel)
{
	return S_OK;
}

_bool CDecal::Change_Target(CGameObject * pTarget)
{
	if (nullptr == pTarget)
	{
		return false;
	}

	if (nullptr != m_pTarget)
	{
		Safe_Release(m_pTarget);
	}

	m_pTarget = pTarget;
	Safe_AddRef(pTarget);

	return true;
}

CDecal * CDecal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDecal*	pInstance = new CDecal(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CDecal");
	}
	return pInstance;
}

CGameObject * CDecal::Clone(void * pArg)
{
	CDecal*	pInstance = new CDecal(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CDecal_Clone");
	}
	return pInstance;
}

void CDecal::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
