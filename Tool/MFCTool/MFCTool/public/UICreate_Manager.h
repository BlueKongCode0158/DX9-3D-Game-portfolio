#pragma once
#include "Base.h"
#include "MFC_Defines.h"

BEGIN(MFC_TOOL)
// ½Ì±ÛÅæ °ü¸®
class CUI_Dummy;
class CUICreate_Manager final: public CBase
{
	DECLARE_SINGLETON(CUICreate_Manager)
private:
	CUICreate_Manager();
	virtual ~CUICreate_Manager();
public:
	HRESULT Add_UI(_tchar* pLayerTag,CUI_Dummy* pUI);
	HRESULT	Delete_UI(_tchar* pLayer);
public:
	HRESULT Load_UI(_tchar* pFile);
	HRESULT Save_UI(_tchar* pFileName);
private:

private:
	map<_tchar*, CUI_Dummy*> map_UIs;
public:
	virtual void Free() override;
};
END

