#pragma once
#include "afxcmn.h"
#include "MFC_Defines.h"


class CDIgTab0;
class CDIgLog1;
class CDIgLog2;
class CUITabLog;

// CControlFormView 폼 뷰입니다.

class CControlFormView : public CFormView
{
	DECLARE_DYNCREATE(CControlFormView)
public:
	enum CONTROLTAB { TERRAIN, OBJECT, EFFECT, UI ,TAB_END };
protected:
	CControlFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CControlFormView();
public:
	CDIgTab0*	pTab_MapTool;
	CDIgLog1*	pTab_Object_Tool;
	CDIgLog2*	pTab_EffectTool;
	CUITabLog*	pTab_UITool;
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
public:
	CTabCtrl	m_MapTab;
	CONTROLTAB	m_eControl = TAB_END;

};


