#include "..\public\Particle_Mesh.h"
#include "Static_Mesh.h"
#include "Transform.h"
#include "Shader.h"
CParticle_Mesh::CParticle_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticle(pGraphic_Device)
{

}

CParticle_Mesh::CParticle_Mesh(const CParticle_Mesh & rhs)
	:CParticle(rhs)
{

}

HRESULT CParticle_Mesh::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Mesh::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CParticle_Mesh::Render()
{
	return S_OK;
}

_int CParticle_Mesh::Tick(_float fTimeDelta)
{
	return _int();
}

CComponent * CParticle_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle_Mesh* pInstance = new CParticle_Mesh(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CParticle_Mesh::Clone(void * pArg)
{
	CParticle* pInstance = new CParticle_Mesh(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CParticle_Mesh::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);
}
