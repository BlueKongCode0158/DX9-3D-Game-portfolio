#include "stdafx.h"
#include "..\public\UICreate_Manager.h"
#include "ControlFormView.h"
#include "MainFrm.h"
#include "GameInstacne.h"
#include "MFCToolView.h"
#include "UI_Dummy.h"
#include "tinyxml2.h"
#include "UITabLog.h"


#ifdef DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SINGLETON(CUICreate_Manager)

CUICreate_Manager::CUICreate_Manager()
{
}

CUICreate_Manager::~CUICreate_Manager()
{
}

HRESULT CUICreate_Manager::Add_UI(const _tchar* pLayerTag, CUI_Dummy * pUI)
{
	auto iter = find_if(map_UIs.begin(), map_UIs.end(), CTagFinder(pLayerTag));
	if (iter != map_UIs.end())
	{
		pUI->Set_Dead(true);
		CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
		pInstance->Delete_GameObject_End(LEVEL_STATIC, pLayerTag);
		RELEASE_INSTANCE(CGameInstacne);
		return S_OK;
	}
	
	map_UIs.emplace(pLayerTag, pUI);
	map_Radians.emplace(pLayerTag, 0.f);
	Safe_AddRef(pUI);

	return S_OK;
}

HRESULT CUICreate_Manager::Delete_UI(const _tchar * pLayerTag)
{
	auto iter = find_if(map_UIs.begin(), map_UIs.end(), CTagFinder(pLayerTag));
	auto mapiter_Radian = find_if(map_Radians.begin(), map_Radians.end(), CTagFinder(pLayerTag));

	if (iter == map_UIs.end())
	{
		MSGBOX("해당 Layer는 존재하지 않습니다.");
		return S_OK;
	}

	Safe_Release(iter->second);
	iter->second->Set_Dead(true);

	map_UIs.erase(iter);
	map_Radians.erase(mapiter_Radian);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	pGameInstance->Delete_GameObject(LEVEL_STATIC, pLayerTag, 0);
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

void CUICreate_Manager::Set_Position(const _tchar * pLayerTag, _float fX, _float fY)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = dynamic_cast<CTransform*>(pInstance->Find_Component(LEVEL_STATIC, pLayerTag, TEXT("Com_Transform")));

	if (nullptr == pTransform)
	{
		MSGBOX("해당 Component는 존재하지 않습니다.");
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}
	_float3 tPosition = pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	tPosition.x = fX;
	tPosition.y = fY;
	pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, tPosition);
	RELEASE_INSTANCE(CGameInstacne);
}

void CUICreate_Manager::Set_Rotation(const _tchar * pLayerTag, _float fY)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = dynamic_cast<CTransform*>(pInstance->Find_Component(LEVEL_STATIC, pLayerTag, TEXT("Com_Transform")));

	if (nullptr == pTransform)
	{
		MSGBOX("해당 Component는 존재하지 않습니다.");
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	_float fTimeDelte = pInstance->Get_Time(TEXT("FPS_60"));
	_float fRadian = D3DXToRadian(fY);
	pTransform->SetUp_RotatinAxis(_float3(0.f, 1.f, 0.f), fRadian);

	auto iter = find_if(map_Radians.begin(), map_Radians.end(), CTagFinder(pLayerTag));
	if (iter != map_Radians.end())
	{
		iter->second = fY;
	}
	else
	{
		map_Radians.emplace(pLayerTag, fY);
	}

	RELEASE_INSTANCE(CGameInstacne);
}

void CUICreate_Manager::Set_Scale(const _tchar * pLayerTag, _float fScaleX, _float fScaleY)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = dynamic_cast<CTransform*>(pInstance->Find_Component(LEVEL_STATIC, pLayerTag, TEXT("Com_Transform")));

	if (nullptr == pTransform)
	{
		MSGBOX("해당 Component는 존재하지 않습니다.");
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}
	pTransform->Set_MatrixScale(fScaleX, fScaleY, 1.f);
	RELEASE_INSTANCE(CGameInstacne);
}

void CUICreate_Manager::Get_Information(const _tchar* pLayerTag, UIINFO& rInfo)
{
	auto map_iter		= find_if(map_UIs.begin(), map_UIs.end(), CTagFinder(pLayerTag));
	auto mapiter_Radian = find_if(map_Radians.begin(), map_Radians.end(), CTagFinder(pLayerTag));

	if (map_UIs.end() == map_iter )
	{
		MSGBOX("해당 UI는 존재하지 않습니다.");
		return;
	}
	map_iter->second->Get_Information(rInfo);

	if (map_Radians.end() == mapiter_Radian)
	{
		MSGBOX("해당 Radian은 존재하지 않습니다.");
		return;
	}
	rInfo.fRadian = mapiter_Radian->second;
	return;
}

void CUICreate_Manager::Set_Select(const _tchar * pLayerTag)
{
	if (nullptr == m_pastTag && nullptr == m_preTag)
	{
		_tcscpy(m_pastTag, pLayerTag);
		_tcscpy(m_preTag, pLayerTag);
		Object_Select(m_preTag, true);
		return;
	}
	_tcscpy(m_preTag, pLayerTag);
	if (0 == _tcscmp(m_preTag, m_pastTag))
	{
		return;
	}
	Object_Select(m_pastTag, false);
	Object_Select(m_preTag, true);
	_tcscpy(m_pastTag, m_preTag);
}

HRESULT CUICreate_Manager::Load_UI( _tchar * pFileName)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	for (auto Pair : map_UIs)
	{
		pGameInstance->Delete_GameObjects(LEVEL_STATIC, Pair.first);
		Safe_Release(Pair.second);
	}
	map_UIs.clear();
	RELEASE_INSTANCE(CGameInstacne);

	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));
	if (CControlFormView::UI != pView->m_MapTab.GetCurSel())
	{
		return 0;
	}
	tinyxml2::XMLDocument Doc;

	_tchar pFile[MAX_PATH];
	wcscpy(pFile, pFileName);
	char cTemp[MAX_PATH];

	WcharToChar(pFile, cTemp);
	Doc.LoadFile(cTemp);

	tinyxml2::XMLNode* pRootNode = Doc.FirstChild();
	tinyxml2::XMLElement* pElement = pRootNode->FirstChildElement();

	for (pElement; pElement; pElement = pElement->NextSiblingElement())
	{
		_matrix tMatrix;
		D3DXMatrixIdentity(&tMatrix);

		for (int iRow = 0; iRow < 4; iRow++)
		{
			for (int iCol= 0; iCol < 4; iCol++)
			{
				string tString;
				tString += 'm';
				tString += (iRow + '0');
				tString += (iCol + '0');
				const tinyxml2::XMLAttribute* pAttribute = pElement->FindAttribute(tString.c_str());
				if (nullptr != pAttribute)
				{
					tMatrix.m[iRow][iCol] = pAttribute->FloatValue();
				}
			}
		}
		CControlFormView* pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));

		CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
		string pName = pElement->Name();
		CString LayerName;
		LayerName = pName.c_str();
		wstring pWname;
		pWname.assign(pName.begin(), pName.end());

		if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_3DUI"),pWname.c_str(), &LayerName)))
		{
			RELEASE_INSTANCE(CGameInstacne);
			continue;
		}
		CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, pWname.c_str(), TEXT("Com_Transform")));
		pTransform->Set_WorldMatrix(tMatrix);
		
		pView->pTab_UITool->Update_LayerName(LayerName);
		RELEASE_INSTANCE(CGameInstacne);
	}

	return S_OK;
}

HRESULT CUICreate_Manager::Save_UI(_tchar * pFileName)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* pRootNode = doc.NewElement("UI_Interface");
	doc.InsertFirstChild(pRootNode);
	for (auto& Pair : map_UIs)
	{
		/* _tchar -> char 로 변환하는 과정 */
		char cName[MAX_PATH];
		memset(static_cast<void*>(cName), 0, sizeof(cName));		
		WcharToChar(Pair.first, cName);

		tinyxml2::XMLElement* pUIElement = doc.NewElement(cName);
		pRootNode->InsertEndChild(pUIElement);
		const CTransform* pTransform = Pair.second->Get_Transform();
		_matrix tMatrix = *pTransform->Get_WorldMatrix();
		
		for (int iRow = 0; iRow < 4; iRow++)
		{
			for (int iCol = 0; iCol < 4; iCol++)
			{
				string tString;
				tString += 'm';
				tString += (iRow + '0');
				tString += (iCol + '0');
				pUIElement->SetAttribute(tString.c_str(), tMatrix.m[iRow][iCol]);
			}
		}
	}
	 
	_tchar pFile[MAX_PATH];
	wcscpy(pFile, pFileName);
	char cTemp[MAX_PATH];
	int iLen = MAX_PATH;

	WideCharToMultiByte(CP_ACP, 0, pFile, iLen, cTemp, iLen, NULL, NULL);
	doc.SaveFile(cTemp);

	return S_OK;
}

void CUICreate_Manager::Object_Select(const _tchar * pLayerTag, _bool isSelect)
{
	auto iter = find_if(map_UIs.begin(), map_UIs.end(), CTagFinder(pLayerTag));
	if (iter == map_UIs.end())
	{
		MSGBOX("해당 Layer는 존재하지않습니다.");
		return;
	}
	iter->second->Set_Select(isSelect);
}

void CUICreate_Manager::Free()
{
	for (auto Pair : map_UIs)
	{
		Safe_Release(Pair.second);
	}
	map_UIs.clear();
	map_Radians.clear();
}


