#include "Shape.h"
#include "Graphic_Device.h"
#include "Static_Mesh.h"

CShape::CShape()
{
}

HRESULT CShape::NativeContruct(void* pArg)
{
	if (nullptr == pArg)
	{

	}
	else
	{
		
	}
	return S_OK;
}

HRESULT CShape::SetUp_Mesh(const _tchar * pFilePath, const _tchar * pFileName)
{
	if (nullptr == pFilePath)
	{
		return E_FAIL;
	}

	m_pMesh = CStatic_Mesh::Create(m_pGraphicDevice, pFilePath, pFileName);
	return S_OK;
}

_float3 CShape::Get_RandomVector()
{
	return _float3();
}

CShape * CShape::Create(void * pArg)
{
	CShape*	pInstance = new CShape();
	if (pInstance->NativeContruct(pArg))
	{
		MSGBOX("Failed Creating Shape Class");
		return nullptr;
	}
	return pInstance;
}

void CShape::Free()
{
	Safe_Release(m_pMesh);
}