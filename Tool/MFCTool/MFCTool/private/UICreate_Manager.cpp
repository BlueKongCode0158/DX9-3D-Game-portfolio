#include "stdafx.h"
#include "..\public\UICreate_Manager.h"
#include "GameInstacne.h"
#include "UI_Dummy.h"
#include "tinyxml2.h"

IMPLEMENT_SINGLETON(CUICreate_Manager)

CUICreate_Manager::CUICreate_Manager()
{
}

CUICreate_Manager::~CUICreate_Manager()
{
}

HRESULT CUICreate_Manager::Add_UI(const _tchar* pLayerTag, CUI_Dummy * pUI)
{
	auto iter = map_UIs.find(pLayerTag);

	if (iter != map_UIs.end())
	{
		pUI->Set_Dead(true);
		CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
		pInstance->Delete_GameObject_End(LEVEL_STATIC, pLayerTag);
		RELEASE_INSTANCE(CGameInstacne);
		return S_OK;
	}
	
	map_UIs.emplace(pLayerTag, pUI);
	Safe_AddRef(pUI);

	return S_OK;
}

HRESULT CUICreate_Manager::Delete_UI(const _tchar * pLayer)
{
	auto iter = map_UIs.find(pLayer);

	if (iter == map_UIs.end())
	{
		MSGBOX("해당 Layer는 존재하지 않습니다.");
		return S_OK;
	}

	Safe_Release(iter->second);
	iter->second->Set_Dead(true);

	map_UIs.erase(pLayer);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
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
		return;
	}

	_float fTimeDelte = pInstance->Get_Time(TEXT("FPS_60"));
	_float fRadian = D3DXToRadian(fY);
	pTransform->SetUp_RotatinAxis(_float3(0.f, 1.f, 0.f), fRadian);

	RELEASE_INSTANCE(CGameInstacne);
}

void CUICreate_Manager::Set_Scale(const _tchar * pLayerTag, _float fScaleX, _float fScaleY)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = dynamic_cast<CTransform*>(pInstance->Find_Component(LEVEL_STATIC, pLayerTag, TEXT("Com_Transform")));

	if (nullptr == pTransform)
	{
		MSGBOX("해당 Component는 존재하지 않습니다.");
		return;
	}
	pTransform->Set_MatrixScale(fScaleX, fScaleY, 1.f);
	RELEASE_INSTANCE(CGameInstacne);
}

HRESULT CUICreate_Manager::Load_UI( _tchar * pFile)
{
	return S_OK;
}

HRESULT CUICreate_Manager::Save_UI(_tchar * pFileName)
{
	tinyxml2::XMLDocument doc;

	/*
	수정 전 사항
	tinyxml2::XMLDeclaration* pDecl = doc.NewDeclaration();
	doc.LinkEndChild(pDecl);

	tinyxml2::XMLElement* pRootNode = doc.NewElement("UI_Node");
	doc.LinkEndChild(pRootNode);

	tinyxml2::XMLElement* pUIElement = doc.NewElement("UI_Element_1");
	pRootNode->LinkEndChild(pUIElement);
	pUIElement->SetAttribute("Attribute", "Data");
	pUIElement->SetAttribute("eType", 1);
	pUIElement->SetAttribute("eID", 1);
	pUIElement->SetAttribute("Text", "Prototype");
	pUIElement->SetAttribute("double", 1.454877);

	*/
	tinyxml2::XMLNode* pRootNode = doc.NewElement("UI_Node");
	doc.InsertFirstChild(pRootNode);

	tinyxml2::XMLElement* pUIElement = doc.NewElement("UI_Element_1");
	pRootNode->InsertEndChild(pUIElement);
	pUIElement->SetAttribute("eType", 1);
	pUIElement->SetAttribute("eID", 1);
	pUIElement->SetAttribute("Prototype_Tag", "Prototype");
	pUIElement->SetAttribute("double", 1.454877);

	_tchar pFile[MAX_PATH];
	wcscpy(pFile, pFileName);
	char cTemp[MAX_PATH];
	int iLen = MAX_PATH;

	WideCharToMultiByte(CP_ACP, 0, pFile, iLen, cTemp, iLen, NULL, NULL);
	doc.SaveFile(cTemp);

	return S_OK;
}

void CUICreate_Manager::Free()
{
	for (auto Pair : map_UIs)
	{
		Safe_Release(Pair.second);
	}
	map_UIs.clear();
}


