#include "..\public\VIBuffer_Point.h"
#include "Attribute.h"

CVIBuffer_Point::CVIBuffer_Point(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Point::CVIBuffer_Point(const CVIBuffer_Point & rhs)
	:CVIBuffer(rhs)
	,m_pVBInstance(rhs.m_pVBInstance)
	,m_pVertexDeclaration(rhs.m_pVertexDeclaration)
	,m_iNumInstance(rhs.m_iNumInstance)
{
	Safe_AddRef(m_pVBInstance);
	Safe_AddRef(m_pVertexDeclaration);
}

HRESULT CVIBuffer_Point::NativeConstruct_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::NativeConstruct_VertexBuffer(sizeof(VTXPOINT), iNumInstance, 0, D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE4(1)  , D3DPT_POINTLIST, 0)))
	{
		return E_FAIL;
	}

	m_pVerticesPos = new _float3[m_iNumVertices];

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride*m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_DEFAULT, &m_pVB, nullptr)))
	{
		return E_FAIL;
	}

	VTXPOINT*	pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pVertices[i].vPosition = m_pVerticesPos[i] = _float3(0.f, 0.f, 0.f);
		pVertices[i].fSize = 1.f;
		pVertices[i].vTexUV = _float2(0.f, 0.f);

		// { a, r, g, b }
		pVertices[i].vColor = _float4(1.f, 0.f, 0.f, 1.f);
	}

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXMATRIX)* iNumInstance, D3DUSAGE_DYNAMIC, D3DFVF_TEX4 | D3DFVF_TEXCOORDSIZE4(0) | D3DFVF_TEXCOORDSIZE4(1) | D3DFVF_TEXCOORDSIZE4(2) | D3DFVF_TEXCOORDSIZE4(3), D3DPOOL_DEFAULT, &m_pVBInstance, nullptr)))
	{
		return E_FAIL;
	}

	VTXMATRIX*		pMatrix = nullptr;

	m_pVBInstance->Lock(0, 0, reinterpret_cast<void**>(&pMatrix), 0);

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pMatrix[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pMatrix[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pMatrix[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pMatrix[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	}

	m_pVBInstance->Unlock();

	D3DVERTEXELEMENT9		Decl[] = {
		{ 0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0 },
		{ 0,12,D3DDECLTYPE_FLOAT1,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_PSIZE,0 },
		{ 0,16,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
		{ 0,24,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1 },
		{ 1,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2 },
		{ 1,16,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3 },
		{ 1,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4 },
		{ 1,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,5 },
		D3DDECL_END()
	};

	if (FAILED(m_pGraphic_Device->CreateVertexDeclaration(Decl, &m_pVertexDeclaration)))
	{
		return E_FAIL;
	}

	m_iNumInstance = iNumInstance;

	return S_OK;
}

HRESULT CVIBuffer_Point::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Point::Render_VIBuffer()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	m_pGraphic_Device->SetStreamSourceFreq(0, 1);
	m_pGraphic_Device->SetStreamSource(1, m_pVBInstance, 0, sizeof(VTXMATRIX));
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);

	m_pGraphic_Device->SetVertexDeclaration(m_pVertexDeclaration);
	m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumInstance);

	m_pGraphic_Device->SetStreamSourceFreq(0, 1);
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);
	return S_OK;
}

HRESULT CVIBuffer_Point::Render_VIBuffer(_uint iNumInstance)
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetStreamSourceFreq(0, 1);

	m_pGraphic_Device->SetStreamSource(1, m_pVBInstance, 0, sizeof(VTXMATRIX));
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);

	m_pGraphic_Device->SetVertexDeclaration(m_pVertexDeclaration);
	m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, iNumInstance);

	m_pGraphic_Device->SetStreamSourceFreq(0, 1);
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);
	return S_OK;
}

_int CVIBuffer_Point::Update(list<CAttribute*>& rList)
{

	VTXPOINT*	pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	int iIndex = 0;
	for (auto iter = rList.begin(); iter != rList.end(); iter++, iIndex++)
	{
		const PDESC* pDesc = (*iter)->GetInfo();
		pVertices[iIndex].vColor = _float4(pDesc->m_vColor, 1.f);
		pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(0.f, 0.f, 0.f);
		pVertices[iIndex].fSize = 1.f;
		pVertices[iIndex].vTexUV = _float2(0.f, 0.f);
	}

	m_pVB->Unlock();
	VTXMATRIX*		pMatrix = nullptr;

	m_pVBInstance->Lock(0, 0, reinterpret_cast<void**>(&pMatrix), 0);
	iIndex = 0;
	for (auto iter = rList.begin(); iter != rList.end(); iter++, iIndex++)
	{
		const PDESC* pDesc	= (*iter)->GetInfo();
		pMatrix[iIndex].vRight		= _float4(1.f, 0.f, 0.f, 0.f);
		pMatrix[iIndex].vUp			= _float4(0.f, 1.f, 0.f, 0.f);
		pMatrix[iIndex].vLook		= _float4(0.f, 0.f, 1.f, 0.f);
		pMatrix[iIndex].vPosition	= _float4(pDesc->m_vPosition , 1.f);
	}
	m_pVBInstance->Unlock();

	return _int();
}


CVIBuffer_Point * CVIBuffer_Point::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CVIBuffer_Point_Prototype");
	}
	return pInstance;
}

CComponent * CVIBuffer_Point::Clone(void * pArg)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CVIBuffer_Point_Clone");
	}
	return pInstance;
}

void CVIBuffer_Point::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);
	Safe_Release(m_pVertexDeclaration);
}
