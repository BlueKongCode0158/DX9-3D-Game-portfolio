#include "..\public\VIBuffer_Rect_Viewport.h"

CVIBuffer_Rect_Viewport::CVIBuffer_Rect_Viewport(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Rect_Viewport::CVIBuffer_Rect_Viewport(const CVIBuffer_Rect_Viewport & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect_Viewport::NativeConstruct_Prototype(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	if (FAILED(NativeConstruct_VertexBuffer(sizeof(VTXTEX_VP), 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPT_TRIANGLELIST, 2)))
	{
		return E_FAIL;
	}
	if (FAILED(NativeConstruct_IndexBuffer(2, 3, 0)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	VTXTEX_VP*		pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition	= _float4(fLeftX, fTopY, 0.f, 1.f);
	pVertices[0].vTexUV		= _float2(0.f, 0.f);
	pVertices[1].vPosition	= _float4(fLeftX + fSizeX, fTopY, 0.f, 1.f);
	pVertices[1].vTexUV		= _float2(1.f, 0.f);
	pVertices[2].vPosition	= _float4(fLeftX + fSizeX, fTopY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV		= _float2(1.f, 1.f);
	pVertices[3].vPosition	= _float4(fLeftX, fTopY + fSizeY, 0.f, 1.f);
	pVertices[3].vTexUV		= _float2(0.f, 1.f);

	m_pVB->Unlock();

	_ushort*		pIndices = 0;

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	reinterpret_cast<_ushort*>(m_pIndeces)[0] = pIndices[0] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[1] = pIndices[1] = 1;
	reinterpret_cast<_ushort*>(m_pIndeces)[2] = pIndices[2] = 2;

	reinterpret_cast<_ushort*>(m_pIndeces)[3] = pIndices[3] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[4] = pIndices[4] = 2;
	reinterpret_cast<_ushort*>(m_pIndeces)[5] = pIndices[5] = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Rect_Viewport::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect_Viewport * CVIBuffer_Rect_Viewport::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	CVIBuffer_Rect_Viewport* pInstance = new CVIBuffer_Rect_Viewport(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(fLeftX, fTopY, fSizeX, fSizeY)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CVIBuffer_Rect_Viewport_Prototype");
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect_Viewport::Clone(void * pArg)
{
	CVIBuffer_Rect_Viewport* pInstance = new CVIBuffer_Rect_Viewport(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CVIBuffer_Rect_Viewport_Clone");
	}
	return pInstance;
}

void CVIBuffer_Rect_Viewport::Free()
{
	__super::Free();
}
