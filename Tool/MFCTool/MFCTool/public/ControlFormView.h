#pragma once
#include "afxcmn.h"
#include "MFC_Defines.h"


class CDIgTab0;
class CDIgLog1;
class CDIgLog2;

// CControlFormView �� ���Դϴ�.

class CControlFormView : public CFormView
{
	DECLARE_DYNCREATE(CControlFormView)
public:
	enum CONTROLTAB { TERRAIN, OBJECT, EFFECT, TAB_END };
protected:
	CControlFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CControlFormView();
public:
	CDIgTab0*	pTab_MapTool;
	CDIgLog1*	pTab_Object_Tool;
	CDIgLog2*	pTab_EffectTool;
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
public:
	CTabCtrl	m_MapTab;
	CONTROLTAB	m_eControl = TAB_END;

};


