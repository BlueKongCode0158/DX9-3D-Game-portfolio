#include "stdafx.h"
#include "..\public\CellPoint_Manager.h"
#include "GameInstacne.h"

IMPLEMENT_SINGLETON(CCellPoint_Manager)
CCellPoint_Manager::CCellPoint_Manager()
{

#ifdef _DEBUG

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	
	if (FAILED(D3DXCreateLine(pGameInstance->Get_Device(), &m_pLine)))
	{
		MSGBOX("Failed to Creating Line");
	}
	RELEASE_INSTANCE(CGameInstacne);

#endif // _DEBUG

}


CCellPoint_Manager::~CCellPoint_Manager()
{
}

void CCellPoint_Manager::Add_CellPoint(CCellPoint * pPoint)
{
	if (nullptr == pPoint)
	{
		return;
	}

	m_VecCellPoint.push_back(pPoint);
	Safe_AddRef(pPoint);
}

void CCellPoint_Manager::Insert_CellPoint(_uint iIndex, CCellPoint* pPoint)
{
	if (iIndex >= m_VecCellPoint.size())
	{
		Safe_Release(pPoint);

		return;
	}

	if (nullptr == m_VecCellPoint[iIndex])
	{
		m_VecCellPoint[iIndex] = pPoint;
		MSGBOX("비어있는 공간에 새로운 CellPoint를 넣었습니다.");
	}
	else
	{
		Safe_Release(m_VecCellPoint[iIndex]);

		m_VecCellPoint[iIndex] = pPoint;

		MSGBOX("기존 CellPoint를 삭제하고, 새로운 CellPoint를 넣었습니다.");
	}
}

void CCellPoint_Manager::Cut_vecCellPoint()
{
	m_VecCellPoint.shrink_to_fit();
}

_uint CCellPoint_Manager::Find_CellPointIndex()
{
	_uint iIndex = 0;

	for (auto& iter = m_VecCellPoint.begin(); iter != m_VecCellPoint.end(); iter++)
	{
		if (true == (*iter)->Get_isSelect())
		{
			return iIndex;
		}
		iIndex++;
	}
	
	return 0;
}

_float3 * CCellPoint_Manager::is_Point_Collision(_float3 vPosition, CCellPoint** ppCellPoint)
{
	for (auto& pPoint : m_VecCellPoint)
	{
		_float3	vCellPosition = pPoint->Get_Position();
		_float3 vDirPosition = vPosition - vCellPosition;

		_float fDistance = fabsf(D3DXVec3Length(&vDirPosition));

		if (fDistance < 2.f)
		{
			*ppCellPoint = pPoint;
			m_vMinPosition = vCellPosition;
			return &m_vMinPosition;
		}
	}
	m_vMinPosition = vPosition;
	return &m_vMinPosition;
}

HRESULT CCellPoint_Manager::Index_VecCellPoint_Select(_uint iIndex)
{
	if (iIndex >= m_VecCellPoint.size())
	{
		return E_FAIL;
	}

	for (auto& pCellPoint : m_VecCellPoint)
	{
		if (pCellPoint == nullptr)
		{
			MSGBOX("해당 공간은 비어있습니다. 새로운 CellPoint를 넣어주세요.");
			break;
		}
		pCellPoint->Set_isSelect(false);
	}

	if (nullptr != m_VecCellPoint[iIndex])
	{
		m_VecCellPoint[iIndex]->Set_isSelect(true);
	}
	return S_OK;
}

HRESULT CCellPoint_Manager::Select_VecCellPoint_Delete(_uint iIndex)
{
	if (iIndex >= m_VecCellPoint.size())
	{
		return E_FAIL;
	}

	m_VecCellPoint[iIndex]->Set_isDead(true);
	Safe_Release(m_VecCellPoint[iIndex]);

	return S_OK;
}

HRESULT CCellPoint_Manager::Save_VecCellPoint(const _tchar* pFileName)
{
	_ulong	dwByte = 0;
	HANDLE	hFile = CreateFile(pFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	_uint i = 0;

	_uint iRemain = (m_VecCellPoint.size() + 1) % 3;
	_uint iSize = m_VecCellPoint.size() - iRemain;

	for (_uint i = 0; i < iSize;)
	{
		_float3		vPoint[3];
		ZeroMemory(vPoint, sizeof(_float3) * 3);
		
		vPoint[0] = m_VecCellPoint[i++]->Get_Position();
		vPoint[1] = m_VecCellPoint[i++]->Get_Position();
		vPoint[2] = m_VecCellPoint[i++]->Get_Position();

		WriteFile(hFile, vPoint ,sizeof(_float3) * 3, &dwByte, nullptr);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CCellPoint_Manager::Load_VecCellPoint(const _tchar * pFileName)
{
	_ulong	dwByte = 0;
	HANDLE	hFile = CreateFile(pFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	_float3		vPoint[3];
	while (true)
	{

		ReadFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
		{
			break;
		}

		CCellPoint*	pCell = nullptr;
		for (_uint i = 0; i < 3; i++)
		{
			if (true == Is_in(vPoint[i], pCell))
			{
				if (nullptr != pCell)
				{
					Add_CellPoint(pCell);
				}
			}
			else
			{
				if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_Cell"), TEXT("Layer_Cell"), &vPoint[i])))
				{
					return E_FAIL;
				}
			}
		}
	}
	RELEASE_INSTANCE(CGameInstacne);
	CloseHandle(hFile);
	return S_OK;
}

_bool CCellPoint_Manager::Is_in(_float3 vPosition, CCellPoint*& ppCellPoint)
{
	for (auto pCell : m_VecCellPoint)
	{
		if (vPosition == pCell->Get_Position())
		{
			ppCellPoint = pCell;
			return true;
		}
	}
	return false;
}

HRESULT CCellPoint_Manager::CellPoint_CW_Sort()
{
	for (size_t i= 0; i < m_VecCellPoint.size(); i = i+3)
	{

		if (i + 2 < m_VecCellPoint.size())
		{
			CCellPoint* pCellPointArr[3]{};

			if (m_VecCellPoint[i]->Get_Position().z >= m_VecCellPoint[i + 1]->Get_Position().z &&
				m_VecCellPoint[i]->Get_Position().z >= m_VecCellPoint[i + 2]->Get_Position().z)
			{
				pCellPointArr[0] = m_VecCellPoint[i];

				if (m_VecCellPoint[i + 1]->Get_Position().z >= m_VecCellPoint[i + 2]->Get_Position().z)
				{
					if (m_VecCellPoint[i + 1]->Get_Position().z == m_VecCellPoint[i + 2]->Get_Position().z)
					{
						if (m_VecCellPoint[i + 1]->Get_Position().x > m_VecCellPoint[i + 2]->Get_Position().x)
						{
							pCellPointArr[1] = m_VecCellPoint[i + 1];
							pCellPointArr[2] = m_VecCellPoint[i + 2];
						}
						else
						{
							pCellPointArr[1] = m_VecCellPoint[i + 2];
							pCellPointArr[2] = m_VecCellPoint[i + 1];
						}
					}
					else
					{
						CCellPoint* pMinZPosition = m_VecCellPoint[i + 2];
						_float3		vDir = pCellPointArr[0]->Get_Position() - pMinZPosition->Get_Position();
						_float3		vNormal = _float3(vDir.z * -1.f, vDir.y, vDir.x);
						_float3		vCellPoint = m_VecCellPoint[i + 1]->Get_Position() - pMinZPosition->Get_Position();
						D3DXVec3Normalize(&vNormal, &vNormal);
						D3DXVec3Normalize(&vCellPoint, &vCellPoint);

						_float	vTheta = D3DXVec3Dot(&vNormal, &vCellPoint);

						/*상삼각인 경우*/
						if (vTheta > 0)
						{
							pCellPointArr[1] = pMinZPosition;
							pCellPointArr[2] = m_VecCellPoint[i + 1];
						}
						/*하삼각인 경우*/
						else
						{
							pCellPointArr[1] = m_VecCellPoint[i + 1];
							pCellPointArr[2] = pMinZPosition;
						}
					}
				}
				else
				{
					CCellPoint* pMinZPosition = m_VecCellPoint[i + 1];

					_float3		vDir = pCellPointArr[0]->Get_Position() - pMinZPosition->Get_Position();
					_float3		vNormal = _float3(vDir.z * -1.f, vDir.y, vDir.x);
					_float3		vCellPoint = m_VecCellPoint[i + 2]->Get_Position() - pMinZPosition->Get_Position();
					D3DXVec3Normalize(&vNormal, &vNormal);
					D3DXVec3Normalize(&vCellPoint, &vCellPoint);

					_float	vTheta = D3DXVec3Dot(&vNormal, &vCellPoint);

					/*상삼각인 경우*/
					if (vTheta > 0)
					{
						pCellPointArr[1] = pMinZPosition;
						pCellPointArr[2] = m_VecCellPoint[i + 2];
					}										 
					/*하삼각인 경우*/						
					else									 
					{										 
						pCellPointArr[1] = m_VecCellPoint[i + 2];
						pCellPointArr[2] = pMinZPosition;
					}

				}
			}
			else if (m_VecCellPoint[i + 1]->Get_Position().z >= m_VecCellPoint[i]->Get_Position().z &&
					 m_VecCellPoint[i + 1]->Get_Position().z >= m_VecCellPoint[i + 2]->Get_Position().z)
			{
				pCellPointArr[0] = m_VecCellPoint[i + 1];

				if (m_VecCellPoint[i]->Get_Position().z >= m_VecCellPoint[i + 2]->Get_Position().z)
				{
					if (m_VecCellPoint[i]->Get_Position().z == m_VecCellPoint[i + 2]->Get_Position().z)
					{
						if (m_VecCellPoint[i]->Get_Position().x > m_VecCellPoint[i + 2]->Get_Position().x)
						{
							pCellPointArr[1] = m_VecCellPoint[i];
							pCellPointArr[2] = m_VecCellPoint[i + 2];
						}
						else
						{
							pCellPointArr[1] = m_VecCellPoint[i + 2];
							pCellPointArr[2] = m_VecCellPoint[i];
						}
					}
					else
					{
						CCellPoint* pMinZPosition = m_VecCellPoint[i + 2];
						_float3		vDir = pCellPointArr[0]->Get_Position() - pMinZPosition->Get_Position();
						_float3		vNormal = _float3(vDir.z * -1.f, vDir.y, vDir.x);
						_float3		vCellPoint = m_VecCellPoint[i]->Get_Position() - pMinZPosition->Get_Position();
						D3DXVec3Normalize(&vNormal, &vNormal);
						D3DXVec3Normalize(&vCellPoint, &vCellPoint);

						_float	vTheta = D3DXVec3Dot(&vNormal, &vCellPoint);

						/*상삼각인 경우*/
						if (vTheta > 0)
						{
							pCellPointArr[1] = pMinZPosition;
							pCellPointArr[2] = m_VecCellPoint[i];
						}
						/*하삼각인 경우*/
						else
						{
							pCellPointArr[1] = m_VecCellPoint[i];
							pCellPointArr[2] = pMinZPosition;
						}
					}
				}
				else
				{
					CCellPoint* pMinZPosition = m_VecCellPoint[i];

					_float3		vDir = pCellPointArr[0]->Get_Position() - pMinZPosition->Get_Position();
					_float3		vNormal = _float3(vDir.z * -1.f, vDir.y, vDir.x);
					_float3		vCellPoint = m_VecCellPoint[i + 2]->Get_Position() - pMinZPosition->Get_Position();
					D3DXVec3Normalize(&vNormal, &vNormal);
					D3DXVec3Normalize(&vCellPoint, &vCellPoint);

					_float	vTheta = D3DXVec3Dot(&vNormal, &vCellPoint);

					/*상삼각인 경우*/
					if (vTheta > 0)
					{
						pCellPointArr[1] = pMinZPosition;
						pCellPointArr[2] = m_VecCellPoint[i + 2];
					}
					/*하삼각인 경우*/
					else
					{
						pCellPointArr[1] = m_VecCellPoint[i + 2];
						pCellPointArr[2] = pMinZPosition;
					}

				}
			}
			else if (m_VecCellPoint[i + 2]->Get_Position().z >= m_VecCellPoint[i]->Get_Position().z &&
					 m_VecCellPoint[i + 2]->Get_Position().z >= m_VecCellPoint[i + 1]->Get_Position().z)
			{
				pCellPointArr[0] = m_VecCellPoint[i + 2];

				if (m_VecCellPoint[i]->Get_Position().z >= m_VecCellPoint[i + 1]->Get_Position().z)
				{
					if (m_VecCellPoint[i]->Get_Position().z == m_VecCellPoint[i + 1]->Get_Position().z)
					{
						if (m_VecCellPoint[i]->Get_Position().x > m_VecCellPoint[i + 1]->Get_Position().x)
						{
							pCellPointArr[1] = m_VecCellPoint[i];
							pCellPointArr[2] = m_VecCellPoint[i + 1];
						}
						else
						{
							pCellPointArr[1] = m_VecCellPoint[i + 1];
							pCellPointArr[2] = m_VecCellPoint[i];
						}
					}
					else
					{
						CCellPoint* pMinZPosition = m_VecCellPoint[i + 1];
						_float3		vDir = pCellPointArr[0]->Get_Position() - pMinZPosition->Get_Position();
						_float3		vNormal = _float3(vDir.z * -1.f, vDir.y, vDir.x);
						_float3		vCellPoint = m_VecCellPoint[i]->Get_Position() - pMinZPosition->Get_Position();
						D3DXVec3Normalize(&vNormal, &vNormal);
						D3DXVec3Normalize(&vCellPoint, &vCellPoint);

						_float	vTheta = D3DXVec3Dot(&vNormal, &vCellPoint);

						/*상삼각인 경우*/
						if (vTheta > 0)
						{
							pCellPointArr[1] = pMinZPosition;
							pCellPointArr[2] = m_VecCellPoint[i];
						}
						/*하삼각인 경우*/
						else
						{
							pCellPointArr[1] = m_VecCellPoint[i];
							pCellPointArr[2] = pMinZPosition;
						}
					}
				}
				else
				{
					CCellPoint* pMinZPosition = m_VecCellPoint[i];

					_float3		vDir = pCellPointArr[0]->Get_Position() - pMinZPosition->Get_Position();
					_float3		vNormal = _float3(vDir.z * -1.f, vDir.y, vDir.x);
					_float3		vCellPoint = m_VecCellPoint[i + 1]->Get_Position() - pMinZPosition->Get_Position();
					D3DXVec3Normalize(&vNormal, &vNormal);
					D3DXVec3Normalize(&vCellPoint, &vCellPoint);

					_float	vTheta = D3DXVec3Dot(&vNormal, &vCellPoint);

					/*상삼각인 경우*/
					if (vTheta > 0)
					{
						pCellPointArr[1] = pMinZPosition;
						pCellPointArr[2] = m_VecCellPoint[i + 1];
					}
					/*하삼각인 경우*/
					else
					{
						pCellPointArr[1] = m_VecCellPoint[i + 1];
						pCellPointArr[2] = pMinZPosition;
					}
				}
			}
			m_VecCellPoint[i] = pCellPointArr[0];
			m_VecCellPoint[i + 1] = pCellPointArr[1];
			m_VecCellPoint[i + 2] = pCellPointArr[2];
		}
		else
		{
			break;
		}
	}
	return S_OK;
}

#ifdef _DEBUG

HRESULT CCellPoint_Manager::Render(const _matrix * pWorldMatrix)
{
	if (nullptr == m_pLine)
	{
		return E_FAIL;
	}


	for (_uint i = 0; i < m_VecCellPoint.size(); i = i + 3)
	{

		if (i + 3 > m_VecCellPoint.size())
		{
			return S_OK;
		}

		_float3		vPoint[4] = {
			m_VecCellPoint[i]->Get_Position(),
			m_VecCellPoint[i + 1]->Get_Position(),
			m_VecCellPoint[i + 2]->Get_Position(),
			m_VecCellPoint[i]->Get_Position()
		};

		CGameInstacne*	pPipeLine = GET_INSTANCE(CGameInstacne);

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

		if (FAILED(m_pLine->DrawTransform(vPoint, 4, &TransformMatrix, D3DXCOLOR(1.f, 0.f, 0.f, 1.f))))
		{
			return E_FAIL;
		}

		m_pLine->End();
	}
	RELEASE_INSTANCE(CGameInstacne);
}
#endif // _DEBUG

void CCellPoint_Manager::Free()
{
	for (auto& pCellPoint : m_VecCellPoint)
	{
		Safe_Release(pCellPoint);
	}
	m_VecCellPoint.clear();

	Safe_Release(m_pLine);
}
