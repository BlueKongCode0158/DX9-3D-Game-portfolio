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
	void	Get_Information(const _tchar* pLayerTag, UIINFO& rInfo);
public:
	void	Set_Select(const _tchar* pLayerTag);
public:
	HRESULT Load_UI(_tchar* pFileName);
	HRESULT Save_UI(_tchar* pFileName);
private:
	void	Object_Select(const _tchar* pLayerTag, _bool isSelect);
private:
	_tchar	m_pastTag[128]{};
	_tchar	m_preTag[128]{};
private:
	map<const _tchar*, CUI_Dummy*>	map_UIs;
	map<const _tchar*, _float>		map_Radians;
public:
	virtual void Free() override;
};
END

