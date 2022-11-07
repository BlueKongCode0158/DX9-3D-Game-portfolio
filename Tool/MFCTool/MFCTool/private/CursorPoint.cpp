#include "stdafx.h"
#include "..\public\CursorPoint.h"
#include "GameInstacne.h"
#include "MainFrm.h"
#include "ControlFormView.h"
#include "DIgTab0.h"
#include "DIgLog1.h"
#include "DIgLog2.h"
#include "CellPoint_Manager.h"

CCursorPoint::CCursorPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CCursorPoint::CCursorPoint(const CCursorPoint & rhs)
	:CGameObject(rhs)
	,m_pMesh(rhs.m_pMesh)
{
	Safe_AddRef(m_pMesh);
}

HRESULT CCursorPoint::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateBox(m_pGraphicDevice, 1.f, 1.f, 1.f, &m_pMesh, nullptr)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCursorPoint::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	m_pTransformCom->Set_MatrixScale(0.5f, 0.5f, 0.5f);

	return S_OK;
}

_int CCursorPoint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CCursorPoint::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
	{
		return _int();
	}
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));

	CDIgTab0* pTab = pView->pTab_MapTool;
	CString LayerName = pTab->m_LayerName;

	CVIBuffer_Terrain* pTerrain = reinterpret_cast<CVIBuffer_Terrain*>(pGameInstance->Find_Component(LEVEL_STATIC, LayerName, TEXT("Com_Terrain"), 0));
	CTransform*	 pTransform = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, LayerName, TEXT("Com_Transform"), 0));

	if (nullptr == pTerrain ||
		nullptr == pTransform)
	{
		RELEASE_INSTANCE(CGameInstacne);
		return _int();
	}

	if (pGameInstance->Input_KeyBoard_Down(DIK_0))
	{
		RadioButton_Remove();
		pView->pTab_Object_Tool->m_CreateRadioState[0].SetCheck(true);
	}
	else if (pGameInstance->Input_KeyBoard_Down(DIK_1))
	{
		RadioButton_Remove();
		pView->pTab_Object_Tool->m_CreateRadioState[1].SetCheck(true);
	}
	else if (pGameInstance->Input_KeyBoard_Down(DIK_2))
	{
		RadioButton_Remove();
		pView->pTab_Object_Tool->m_CreateRadioState[2].SetCheck(true);
	}
	else if (pGameInstance->Input_KeyBoard_Down(DIK_3))
	{
		RadioButton_Remove();
		pView->pTab_Object_Tool->m_CreateRadioState[3].SetCheck(true);
	}

	_float3* pPosition = pGameInstance->Compute_PickingPoint(pTerrain, *pTransform->Get_WorldMatrix());
	if (nullptr != pPosition)
	{
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, *pPosition);
	}
	
	if (1 == pView->m_MapTab.GetCurSel())
	{
		if (true == pView->pTab_Object_Tool->m_CreateRadioState[0].GetCheck())
		{
			Create_Dynamic_Object();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[1].GetCheck())
		{
			Create_Static_Object();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[2].GetCheck())
		{
			Create_Cell();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[3].GetCheck())
		{

		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[4].GetCheck())
		{
			Create_Dynamic_Assert_Object();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[5].GetCheck())
		{
			Create_Static_Assert_Object();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[6].GetCheck())
		{
			Create_Cell_Assert();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[7].GetCheck())
		{
			Create_Monster_Create();
		}
		else if (true == pView->pTab_Object_Tool->m_CreateRadioState[8].GetCheck())
		{
			Create_Player_Create();
		}
	}
	else
	{

	}


	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CCursorPoint::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	m_pShaderCom->Begin_Shader(0);
	m_pMesh->DrawSubset(0);
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CCursorPoint::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Point_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCursorPoint::SetUp_ConstantTable()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED( m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix)) ||
				m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix))   ||
				m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix))))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CCursorPoint::Create_Dynamic_Object()
{
	return S_OK;
}

HRESULT CCursorPoint::Create_Static_Object()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));
	CDIgTab0* pTab = pView->pTab_MapTool;
	CDIgLog1* pObject = pView->pTab_Object_Tool;

	if (pGameInstance->Input_KeyMouse_Up(CInput_Device::DIM::DIM_LBUTTON))
	{
		if (true == pObject->m_SelectRadio[0].GetCheck())
		{
			CString LayerName = pTab->m_LayerName;
			CString	MeshLayerName = pObject->m_StaticTag;
			CString MeshPrototypeName = pObject->m_Static_CreateObjectTag;

			CVIBuffer_Terrain* pTerrain = reinterpret_cast<CVIBuffer_Terrain*>(pGameInstance->Find_Component(LEVEL_STATIC, LayerName, TEXT("Com_Terrain"), 0));
			CTransform*	 pTransform = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, LayerName, TEXT("Com_Transform"), 0));

			if (nullptr != pTerrain && nullptr != pTransform)
			{
				_float3* pPosition = pGameInstance->Compute_PickingPoint(pTerrain, *pTransform->Get_WorldMatrix());

				if (nullptr != pPosition)
				{
					if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, MeshPrototypeName, MeshLayerName, pPosition)))
					{
						return E_FAIL;
					}
					pObject->Update_Static_Mesh_List();
				}
			}
		}
		else if (true == pObject->m_SelectRadio[1].GetCheck())
		{
			CString StaticName = pObject->m_StaticTag;

			LPD3DXMESH	 pMesh = reinterpret_cast<LPD3DXMESH>(pGameInstance->Find_Component(LEVEL_STATIC, StaticName, TEXT("Com_Mesh")));
			CTransform*	pTransform_Mesh = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, StaticName, TEXT("Com_Transform")));

			if (nullptr != pMesh && nullptr != pTransform_Mesh)
			{
				_float3* pPosition = pGameInstance->Compute_PickingPoint(pMesh, *pTransform_Mesh->Get_WorldMatrix());
			}
		}
		else if (true == pObject->m_SelectRadio[2].GetCheck())
		{
			CString DynamicName = pObject->m_DynamicTag;


			LPD3DXMESH	 pMesh = reinterpret_cast<LPD3DXMESH>(pGameInstance->Find_Component(LEVEL_STATIC, DynamicName, TEXT("Com_Mesh")));
			CTransform*	pTransform_Mesh = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, DynamicName, TEXT("Com_Transform")));

			if (nullptr != pMesh && nullptr != pTransform_Mesh)
			{
				_float3* pPosition = pGameInstance->Compute_PickingPoint(pMesh, *pTransform_Mesh->Get_WorldMatrix());
			}

		}
	}


	return S_OK;
}

HRESULT CCursorPoint::Create_Cell()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));
	CDIgTab0* pTab = pView->pTab_MapTool;
	CDIgLog1* pObject = pView->pTab_Object_Tool;


	if (pGameInstance->Input_KeyMouse_Up(CInput_Device::DIM::DIM_LBUTTON))
	{

		if (true == pObject->m_SelectRadio[0].GetCheck())
		{
			CString LayerName = pTab->m_LayerName;

			CVIBuffer_Terrain* pTerrain = reinterpret_cast<CVIBuffer_Terrain*>(pGameInstance->Find_Component(LEVEL_STATIC, LayerName, TEXT("Com_Terrain"), 0));
			CTransform*	 pTransform = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, LayerName, TEXT("Com_Transform"), 0));

			if (nullptr != pTerrain && nullptr != pTransform)
			{
				_float3* pPosition = pGameInstance->Compute_PickingPoint(pTerrain, *pTransform->Get_WorldMatrix());

				if (nullptr != pPosition)
				{
					_float3	 vPosition = *pPosition;
					CCellPoint*	pCellPoint = nullptr;

					_float3*	pCellPosition = CCellPoint_Manager::Get_Instance()->is_Point_Collision(*pPosition,&pCellPoint);
					if (*pCellPosition == vPosition)
					{
						if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_Cell"), TEXT("Layer_Cell"), pCellPosition)))
						{
							return E_FAIL;
						}
					}
					else if(nullptr != pCellPoint)
					{
						CCellPoint_Manager::Get_Instance()->Add_CellPoint(pCellPoint);
					}
					pObject->Update_NavigationCell_List();
				}
			}
		}
		else if (true == pObject->m_SelectRadio[1].GetCheck())
		{
			CString StaticName = pObject->m_StaticTag;

			LPD3DXMESH	 pMesh = reinterpret_cast<LPD3DXMESH>(pGameInstance->Find_Component(LEVEL_STATIC, StaticName, TEXT("Com_Mesh")));
			CTransform*	pTransform_Mesh = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, StaticName, TEXT("Com_Transform")));

			if (nullptr != pMesh && nullptr != pTransform_Mesh)
			{
				_float3* pPosition = pGameInstance->Compute_PickingPoint(pMesh,*pTransform_Mesh->Get_WorldMatrix());
			}
		}
		else if (true == pObject->m_SelectRadio[2].GetCheck())
		{
			CString DynamicName = pObject->m_DynamicTag;


			LPD3DXMESH	 pMesh = reinterpret_cast<LPD3DXMESH>(pGameInstance->Find_Component(LEVEL_STATIC, DynamicName, TEXT("Com_Mesh")));
			CTransform*	pTransform_Mesh = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, DynamicName, TEXT("Com_Transform")));

			if (nullptr != pMesh && nullptr != pTransform_Mesh)
			{
				_float3* pPosition = pGameInstance->Compute_PickingPoint(pMesh, *pTransform_Mesh->Get_WorldMatrix());
			}

		}
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CCursorPoint::Create_Dynamic_Assert_Object()
{
	return S_OK;
}

HRESULT CCursorPoint::Create_Static_Assert_Object()
{
	return S_OK;
}

HRESULT CCursorPoint::Create_Cell_Assert()
{
	return S_OK;
}

HRESULT CCursorPoint::Create_Monster_Create()
{
	return S_OK;
}

HRESULT CCursorPoint::Create_Player_Create()
{
	return S_OK;
}

void CCursorPoint::RadioButton_Remove()
{
	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));

	for (_uint i = 0; i < 7; i++)
	{
		pView->pTab_Object_Tool->m_CreateRadioState[i].SetCheck(false);
	}

}

CCursorPoint * CCursorPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCursorPoint* pInstance = new CCursorPoint(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CursorPoint_Prototype");
	}
	return pInstance;
}

CGameObject * CCursorPoint::Clone(void * pArg)
{
	CCursorPoint* pInstance = new CCursorPoint(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CursorPoint_Prototype");
	}
	return pInstance;
}

void CCursorPoint::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMesh);
}
