#include "..\public\Cell.h"
#include "PipeLine.h"

CCell::CCell(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * LINE_END);
}

HRESULT CCell::NativeConstruct(_float3 vPointA, _float3 vPointB, _float3 vPointC, _uint iIndex)
{
	m_iIndex = iIndex;

	m_vPoint[POINT_A] = vPointA;
	m_vPoint[POINT_B] = vPointB;
	m_vPoint[POINT_C] = vPointC;

	m_vLine[LINE_AB] = m_vPoint[POINT_B] - m_vPoint[POINT_A];
	m_vLine[LINE_BC] = m_vPoint[POINT_C] - m_vPoint[POINT_B];
	m_vLine[LINE_CA] = m_vPoint[POINT_A] - m_vPoint[POINT_C];

	/* 여기 나중에 수정해줄 것.*/
	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	//_float3 vNormalAB, vNormalBC, vNormalCA;
	//_float3 vPointAB_UP, vPointBC_UP, vPointCA_UP;

	//D3DXVec3Cross(&vPointAB_UP, &m_vLine[LINE_CA], &m_vLine[LINE_AB]);
	//D3DXVec3Cross(&vPointBC_UP, &m_vLine[LINE_AB], &m_vLine[LINE_BC]);
	//D3DXVec3Cross(&vPointCA_UP, &m_vLine[LINE_BC], &m_vLine[LINE_CA]);

	//D3DXVec3Cross(&vNormalAB, &m_vLine[LINE_AB], &vPointAB_UP);
	//D3DXVec3Cross(&vNormalBC, &m_vLine[LINE_BC], &vPointBC_UP);
	//D3DXVec3Cross(&vNormalCA, &m_vLine[LINE_CA], &vPointCA_UP);

	//m_vNormal[LINE_AB] = vNormalAB;
	//m_vNormal[LINE_BC] = vNormalBC;
	//m_vNormal[LINE_CA] = vNormalCA;

	for (_uint i = 0; i < LINE_END; ++i)
	{
		D3DXVec3Normalize(&m_vNormal[i], &m_vNormal[i]);
	}
	
#ifdef _DEBUG
	if (FAILED(Ready_Debug()))
	{
		return E_FAIL;
	}
#endif // DEBUG


	return S_OK;
}

_bool CCell::Compare_Points(_float3 vSourPoint, _float3 vDestPoint)
{
	if (m_vPoint[POINT_A] == vSourPoint)
	{
		if (m_vPoint[POINT_B] == vDestPoint)
		{
			return true;
		}
		if (m_vPoint[POINT_C] == vDestPoint)
		{
			return true;
		}
	}

	if (m_vPoint[POINT_B] == vSourPoint)
	{
		if (m_vPoint[POINT_A] == vDestPoint)
		{
			return true;
		}
		if (m_vPoint[POINT_C] == vDestPoint)
		{
			return true;
		}
	}

	if (m_vPoint[POINT_C] == vSourPoint)
	{
		if (m_vPoint[POINT_B] == vDestPoint)
		{
			return true;
		}
		if (m_vPoint[POINT_A] == vDestPoint)
		{
			return true;
		}
	}
	return false;
}

_bool CCell::isIn(_float3 vPoint, CCell ** ppNeighbor, _float3** vNormal)
{
	for (_uint i = 0; i < LINE_END; i++)
	{
		_float3 vDir;
		D3DXVec3Normalize(&vDir, &(vPoint - m_vPoint[i]));

		if (0 < D3DXVec3Dot(&m_vNormal[i], &vDir))
		{
			if (nullptr != m_pNeighbor[i])
			{
				*ppNeighbor = m_pNeighbor[i];
			}
			*vNormal = &m_vNormal[i];
			return false;
		}
	}
	return true;
}

#ifdef _DEBUG

HRESULT CCell::Ready_Debug()
{
	if (FAILED(D3DXCreateLine(m_pGraphic_Device, &m_pLine)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCell::Render(const _matrix * pWorldMatrix, D3DXCOLOR Color /*= D3DXCOLOR(1.f, 0.f, 0.f, 1.f)*/)
{
	if (nullptr == m_pLine)
	{
		return E_FAIL;
	}

	_float3		vPoint[4] = {
		m_vPoint[POINT_A],
		m_vPoint[POINT_B],
		m_vPoint[POINT_C],
		m_vPoint[POINT_A]
	};

	CPipeLine*	pPipeLine = GET_INSTANCE(CPipeLine);
	_matrix TransformMatrix;
	D3DXMatrixIdentity(&TransformMatrix);

	for (_uint i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], pWorldMatrix);
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &pPipeLine->Get_Transform(D3DTS_VIEW));
		if (0 >= vPoint[i].z)
		{
			vPoint[i].z = 0.25;
		}
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &pPipeLine->Get_Transform(D3DTS_PROJECTION));
	}


	D3DXMatrixIdentity(&TransformMatrix);

	m_pLine->SetWidth(3.f);
	m_pLine->Begin();

	if (FAILED(m_pLine->DrawTransform(vPoint, 4, &TransformMatrix, Color)))
	{
		return E_FAIL;
	}

	m_pLine->End();

	RELEASE_INSTANCE(CPipeLine);
	return S_OK;
}
#endif // DEBUG

CCell * CCell::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float3 vPointA, _float3 vPointB, _float3 vPointC, _uint iIndex)
{
	CCell* pInstance = new CCell(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(vPointA, vPointB, vPointC, iIndex)))
	{
		MSGBOX("Failed to Creating CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pGraphic_Device);

#ifdef _DEBUG
	Safe_Release(m_pLine);
#endif // DEBUG

}
