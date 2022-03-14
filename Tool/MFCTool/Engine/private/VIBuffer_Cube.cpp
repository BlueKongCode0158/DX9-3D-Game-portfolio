#include "..\public\VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_VertexBuffer(sizeof(VTXCUBE), 8, 0, (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0)), D3DPT_TRIANGLELIST, 12)))
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

	VTXCUBE* pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, -0.5f);
	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, -0.5f);

	pVertices[4].vPosition = m_pVerticesPos[4] = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[5].vPosition = m_pVerticesPos[5] = _float3(0.5f, 0.5f, 0.5f);
	pVertices[6].vPosition = m_pVerticesPos[6] = _float3(0.5f, -0.5f, 0.5f);
	pVertices[7].vPosition = m_pVerticesPos[7] = _float3(-0.5f, -0.5f, 0.5f);

	pVertices[0].vTexUV = pVertices[0].vPosition;
	pVertices[1].vTexUV = pVertices[1].vPosition;
	pVertices[2].vTexUV = pVertices[2].vPosition;
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vTexUV = pVertices[4].vPosition;
	pVertices[5].vTexUV = pVertices[5].vPosition;
	pVertices[6].vTexUV = pVertices[6].vPosition;
	pVertices[7].vTexUV = pVertices[7].vPosition;



	m_pVB->Unlock();

	_ushort* pIndex = nullptr;

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0);

	//앞
	reinterpret_cast<_short*>(m_pIndeces)[0] = pIndex[0] = 0; reinterpret_cast<_short*>(m_pIndeces)[1] = pIndex[1] = 1;
	reinterpret_cast<_short*>(m_pIndeces)[2] = pIndex[2] = 2; reinterpret_cast<_short*>(m_pIndeces)[3] = pIndex[3] = 0;
	reinterpret_cast<_short*>(m_pIndeces)[4] = pIndex[4] = 2; reinterpret_cast<_short*>(m_pIndeces)[5] = pIndex[5] = 3;

	//뒤
	reinterpret_cast<_short*>(m_pIndeces)[6] = pIndex[6] = 5;	reinterpret_cast<_short*>(m_pIndeces)[7] = pIndex[7] = 4;
	reinterpret_cast<_short*>(m_pIndeces)[8] = pIndex[8] = 7;	reinterpret_cast<_short*>(m_pIndeces)[9] = pIndex[9] = 5;
	reinterpret_cast<_short*>(m_pIndeces)[10] = pIndex[10] = 7;	reinterpret_cast<_short*>(m_pIndeces)[11] = pIndex[11] = 6;

	//왼쪽
	reinterpret_cast<_short*>(m_pIndeces)[12] = pIndex[12] = 4; reinterpret_cast<_short*>(m_pIndeces)[13] = pIndex[13] = 0;
	reinterpret_cast<_short*>(m_pIndeces)[14] = pIndex[14] = 3; reinterpret_cast<_short*>(m_pIndeces)[15] = pIndex[15] = 4;
	reinterpret_cast<_short*>(m_pIndeces)[16] = pIndex[16] = 3; reinterpret_cast<_short*>(m_pIndeces)[17] = pIndex[17] = 7;

	//오른쪽
	reinterpret_cast<_short*>(m_pIndeces)[18] = pIndex[18] = 1; reinterpret_cast<_short*>(m_pIndeces)[19] = pIndex[19] = 5;
	reinterpret_cast<_short*>(m_pIndeces)[20] = pIndex[20] = 6; reinterpret_cast<_short*>(m_pIndeces)[21] = pIndex[21] = 1;
	reinterpret_cast<_short*>(m_pIndeces)[22] = pIndex[22] = 6; reinterpret_cast<_short*>(m_pIndeces)[23] = pIndex[23] = 2;

	//위
	reinterpret_cast<_short*>(m_pIndeces)[24] = pIndex[24] = 4; reinterpret_cast<_short*>(m_pIndeces)[25] = pIndex[25] = 5;
	reinterpret_cast<_short*>(m_pIndeces)[26] = pIndex[26] = 1; reinterpret_cast<_short*>(m_pIndeces)[27] = pIndex[27] = 4;
	reinterpret_cast<_short*>(m_pIndeces)[28] = pIndex[28] = 1; reinterpret_cast<_short*>(m_pIndeces)[29] = pIndex[29] = 0;

	//아래
	reinterpret_cast<_short*>(m_pIndeces)[30] = pIndex[30] = 3; reinterpret_cast<_short*>(m_pIndeces)[31] = pIndex[31] = 2;
	reinterpret_cast<_short*>(m_pIndeces)[32] = pIndex[32] = 6; reinterpret_cast<_short*>(m_pIndeces)[33] = pIndex[33] = 3;
	reinterpret_cast<_short*>(m_pIndeces)[34] = pIndex[34] = 6; reinterpret_cast<_short*>(m_pIndeces)[35] = pIndex[35] = 7;

	m_pIB->Unlock();
	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);
	return S_OK;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CVIBuffer_Cube_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CComponent*	 pInstance = new CVIBuffer_Cube(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Cube_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
