#include "..\public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	:CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_VertexBuffer(sizeof(VTXTEX), 6, 0, (D3DFVF_XYZ | D3DFVF_TEX1), D3DPT_TRIANGLELIST, 2)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct_IndexBuffer(2, 3, 0)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition	 = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV		 = _float2(0.f, 0.f);
	pVertices[1].vPosition	 = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV		 = _float2(1.f, 0.f);
	pVertices[2].vPosition	 = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV		 = _float2(1.f, 1.f);
	pVertices[3].vPosition	 = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV		 = _float2(0.f, 1.f);

	m_pVB->Unlock();

	_ushort*		pIndex = nullptr;
	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0);

	reinterpret_cast<_ushort*>(m_pIndeces)[0] = pIndex[0] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[1] = pIndex[1] = 1;
	reinterpret_cast<_ushort*>(m_pIndeces)[2] = pIndex[2] = 2;

	reinterpret_cast<_ushort*>(m_pIndeces)[3] = pIndex[3] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[4] = pIndex[4] = 2;
	reinterpret_cast<_ushort*>(m_pIndeces)[5] = pIndex[5] = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Rect::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}
