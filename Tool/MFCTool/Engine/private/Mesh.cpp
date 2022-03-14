#include "..\public\Mesh.h"

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CMesh::CMesh(const CMesh & rhs)
	:CComponent(rhs)
{

}

HRESULT CMesh::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMesh::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CMesh::Free()
{
	__super::Free();
}
