#include "..\public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	:CComponent(rhs)
	,m_pVB(rhs.m_pVB)
	,m_pIB(rhs.m_pIB)
	,m_iStride(rhs.m_iStride)
	,m_iNumVertices(rhs.m_iNumVertices)
	,m_dwVBUsage(rhs.m_dwVBUsage)
	,m_dwFVF(rhs.m_dwFVF)
	,m_ePrimitiveType(rhs.m_ePrimitiveType)
	,m_iNumPrimitive(rhs.m_iNumPrimitive)
	,m_iOneIndexSize(rhs.m_iOneIndexSize)
	,m_iNumVerticesPrimitive(rhs.m_iNumVerticesPrimitive)
	,m_dwIBUsage(rhs.m_dwIBUsage)
	,m_pIndeces(rhs.m_pIndeces)
	,m_pVerticesPos(rhs.m_pVerticesPos)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::NativeConstruct_VertexBuffer(_uint iStride, _uint iNumVertices, _ulong dwVBUsage, _ulong dwFVF, D3DPRIMITIVETYPE ePrimitiveType, _uint iNumPrimitive)
{
	m_iStride			= iStride;
	m_iNumVertices		= iNumVertices;
	m_dwVBUsage			= dwVBUsage;
	m_dwFVF				= dwFVF;
	m_ePrimitiveType	= ePrimitiveType;
	m_iNumPrimitive		= iNumPrimitive;

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct_IndexBuffer(_uint iNumIndexSize, _uint iNumVerticesPrimitive, _ulong dwIBUsage)
{
	m_iOneIndexSize = iNumIndexSize;
	m_iNumVerticesPrimitive = iNumVerticesPrimitive;
	m_dwIBUsage = dwIBUsage;

	return S_OK;
}

void CVIBuffer::Get_PolygonVertexPosition(_uint uiPolygonIndex, _float3 * pPoints) const
{
	_uint iIndex = uiPolygonIndex * 3;
	if (2 == m_iOneIndexSize)
	{
		pPoints[0] = m_pVerticesPos[reinterpret_cast<_ushort*>(m_pIndeces)[iIndex]];
		pPoints[1] = m_pVerticesPos[reinterpret_cast<_ushort*>(m_pIndeces)[iIndex + 1]];
		pPoints[2] = m_pVerticesPos[reinterpret_cast<_ushort*>(m_pIndeces)[iIndex + 2]];
	}
	else
	{
		pPoints[0] = m_pVerticesPos[reinterpret_cast<_uint*>(m_pIndeces)[iIndex]];
		pPoints[1] = m_pVerticesPos[reinterpret_cast<_uint*>(m_pIndeces)[iIndex + 1]];
		pPoints[2] = m_pVerticesPos[reinterpret_cast<_uint*>(m_pIndeces)[iIndex + 2]];
	}
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride*m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iOneIndexSize*m_iNumPrimitive*m_iNumVerticesPrimitive, m_dwIBUsage, m_iOneIndexSize == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		return E_FAIL;
	}

	m_pVerticesPos = new _float3[m_iNumVertices];

	if (2 == m_iOneIndexSize)
	{
		m_pIndeces = new _ushort[m_iNumPrimitive * 3];
	}
	else
	{
		m_pIndeces = new _uint[m_iNumPrimitive * 3];
	}
	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}
HRESULT CVIBuffer::Render_VIBuffer()
{
	if (FAILED(m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->SetFVF(m_dwFVF)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->SetIndices(m_pIB)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isClone)
	{
		Safe_Delete_Array(m_pIndeces);
		Safe_Delete_Array(m_pVerticesPos);
	}


	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
