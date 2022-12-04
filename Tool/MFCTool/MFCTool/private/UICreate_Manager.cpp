#include "stdafx.h"
#include "..\public\UICreate_Manager.h"
#include "tinyxml2.h"

IMPLEMENT_SINGLETON(CUICreate_Manager)

CUICreate_Manager::CUICreate_Manager()
{
}

CUICreate_Manager::~CUICreate_Manager()
{
}

HRESULT CUICreate_Manager::Add_UI( _tchar * pLayer, const CUI_Dummy * pUI)
{
	return S_OK;
}

HRESULT CUICreate_Manager::Delete_UI( _tchar * pLayer)
{
	return S_OK;
}

HRESULT CUICreate_Manager::Import_UI( _tchar * pFile)
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
	pUIElement->SetAttribute("Attribute", "Data");
	pUIElement->SetAttribute("eType", 1);
	pUIElement->SetAttribute("eID", 1);
	pUIElement->SetAttribute("Text", "Prototype");
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

}


