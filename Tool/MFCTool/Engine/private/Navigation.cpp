#include "..\public\Navigation.h"
#include "Cell.h"




CNavigation::CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_vecCells(rhs.m_vecCells)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
	for (auto& pCell : m_vecCells)
	{
		Safe_AddRef(pCell);
	}
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar* pDataFiles)
{
	_ulong	dwByte = 0;
	HANDLE	hFile = CreateFile(pDataFiles, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	_float3		vPoint[CCell::POINT_END];

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(_float3)*CCell::POINT_END, &dwByte, nullptr);
		if (0 == dwByte)
		{
			break;
		}

		CCell*		pCell = CCell::Create(m_pGraphic_Device, vPoint[CCell::POINT::POINT_A], vPoint[CCell::POINT::POINT_B], vPoint[CCell::POINT_C], m_vecCells.size());
		if (nullptr == pCell)
		{
			return E_FAIL;
		}
		m_vecCells.push_back(pCell);
	}

	if (FAILED(SetUp_Neighbor()))
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&m_WorldMatrix);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}
	if (nullptr != pArg)
	{
		m_NeviDesc = *(reinterpret_cast<NAVIDESC*>(pArg));
	}
	return S_OK;
}

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_vecCells)
	{
		for (auto& pDestCell : m_vecCells)
		{
			if (pSourCell == pDestCell)
			{
				continue;
			}
			
			if (true == pDestCell->Compare_Points(pSourCell->Get_Points(CCell::POINT::POINT_A), pSourCell->Get_Points(CCell::POINT::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE::LINE_AB, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Points(CCell::POINT::POINT_B), pSourCell->Get_Points(CCell::POINT::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE::LINE_BC, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Points(CCell::POINT::POINT_C), pSourCell->Get_Points(CCell::POINT::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE::LINE_CA, pDestCell);
			}
		}
	}
	return S_OK;
}

_bool CNavigation::Move_OnNavigation(_float3 vPosition, _float3** vNormal)
{
	CCell*	pNeighbor = nullptr;

	if (true == m_vecCells[m_NeviDesc.iCurrentCellIndex]->isIn(vPosition,&pNeighbor,vNormal))
	{
		return true;
	}
	else
	{
		if (nullptr == pNeighbor)
		{

		}
		else
		{
			 for (_uint i = 0 ; i<1000; i++)
			 {
				if (nullptr == pNeighbor)
				{
					return false;
				}
				if (true == pNeighbor->isIn(vPosition, &pNeighbor,vNormal))
				{
					m_NeviDesc.iCurrentCellIndex = pNeighbor->Get_Index();
					return true;
				}
			}
		}
	}

	return false;
}

HRESULT CNavigation::Update_WorldMatrix(const _matrix * pWorldMatrix)
{
	if (nullptr != pWorldMatrix)
	{
		m_WorldMatrix = *pWorldMatrix;
	}
	return S_OK;
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	for (_uint i = 0; i < m_vecCells.size(); i++)
	{
		if (nullptr != m_vecCells[i])
		{
			D3DXCOLOR		Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
			if (i == m_NeviDesc.iCurrentCellIndex)
			{
				Color = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
			}
			m_vecCells[i]->Render(&m_WorldMatrix,Color);
		}
	}
	return S_OK;
}
#endif // DEBUG

CNavigation * CNavigation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pDataFiles)
{
	CNavigation*	pInstance = new CNavigation(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pDataFiles)))
	{
		MSGBOX("Failed to Creating CNavigation_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_vecCells)
	{
		Safe_Release(pCell);
	}
	m_vecCells.clear();
}
