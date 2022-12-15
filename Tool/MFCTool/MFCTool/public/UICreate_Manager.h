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
	HRESULT Add_UI(const _tchar* pLayerTag,CUI_Dummy* pUI);
	HRESULT	Delete_UI(const _tchar* pLayer);
public:
	void	Set_Position(const _tchar* pLayerTag, _float fX, _float fY);
	void	Set_Rotation(const _tchar* pLayerTag, _float fY);
	void	Set_Scale(const _tchar* pLayerTag, _float fScaleX, _float fScaleY);
public:
	HRESULT Load_UI(_tchar* pFile);
	HRESULT Save_UI(_tchar* pFileName);
private:

private:
	map<const _tchar*, CUI_Dummy*> map_UIs;
public:
	virtual void Free() override;
};
END

