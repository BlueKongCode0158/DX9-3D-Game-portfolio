#include "..\public\VIBuffer_Gizmo.h"


CVIBuffer_Gizmo::CVIBuffer_Gizmo(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Gizmo::CVIBuffer_Gizmo(const CVIBuffer_Gizmo & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_pVertexDeclaration(rhs.m_pVertexDeclaration)
	, m_iNumInstance(rhs.m_iNumInstance)
{
	Safe_AddRef(m_pVBInstance);
	Safe_AddRef(m_pVertexDeclaration);
}

HRESULT CVIBuffer_Gizmo::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_VertexBuffer(sizeof(VTXGIZMO), 4, 0, (D3DFVF_XYZ | D3DFVF_DIFFUSE), D3DPT_LINELIST, 0)))
	{
		return E_FAIL;
	}
	m_pVerticesPos = new _float3[m_iNumVertices];

	if (FAILED(__super::NativeConstruct_IndexBuffer(2, 2, 0)))
	{
		return E_FAIL;
	}


	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return E_FAIL;
	}

	VTXGIZMO* pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(0.f, 0.f, 0.f);
	pVertices[0].vColor = _float4(1.f, 1.f, 1.f, 1.f);
	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(1.f, 0.f, 0.f);
	pVertices[1].vColor = _float4(1.f, 0.f, 0.f, 1.f);
	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.f, 1.f, 0.f);
	pVertices[2].vColor = _float4(0.f, 1.f, 0.f, 1.f);
	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(0.f, 0.f, 1.f);
	pVertices[3].vColor = _float4(0.f, 0.f, 1.f, 1.f);
	m_pVB->Unlock();

	const int iIndexSize = 6;
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iOneIndexSize * iIndexSize, m_dwIBUsage, m_iOneIndexSize == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		return E_FAIL;
	}

	if (2 == m_iOneIndexSize)
	{
		m_pIndeces = new _ushort[m_iNumPrimitive * 3];
	}
	else
	{
		m_pIndeces = new _uint[m_iNumPrimitive * 3];
	}

	_ushort*	pIndex = nullptr;
	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0);

	reinterpret_cast<_ushort*>(m_pIndeces)[0] = pIndex[0] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[1] = pIndex[1] = 1;

	reinterpret_cast<_ushort*>(m_pIndeces)[2] = pIndex[2] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[3] = pIndex[3] = 2;

	reinterpret_cast<_ushort*>(m_pIndeces)[4] = pIndex[4] = 0;
	reinterpret_cast<_ushort*>(m_pIndeces)[5] = pIndex[5] = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Gizmo::NativeConstruct(void * pArg)
{
	return S_OK;
}


CVIBuffer_Gizmo * CVIBuffer_Gizmo::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance)
{
	return nullptr;
}

CComponent * CVIBuffer_Gizmo::Clone(void * pArg)
{
	return nullptr;
}

void CVIBuffer_Gizmo::Free()
{
}
