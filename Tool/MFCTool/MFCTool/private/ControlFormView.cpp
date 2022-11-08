// ..\private\ControlFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ControlFormView.h"
#include "MFCToolView.h"
#include "MainFrm.h"

#include "DIgTab0.h"
#include "DIgLog1.h"
#include "DIgLog2.h"
#include "UITabLog.h"


// CControlFormView

IMPLEMENT_DYNCREATE(CControlFormView, CFormView)

CControlFormView::CControlFormView()
	: CFormView(IDD_CONTROLFORMVIEW)
	, m_isLockView(_T("UNLOCK"))
{

}

CControlFormView::~CControlFormView()
{
	delete pTab_MapTool;
	delete pTab_Object_Tool;
	delete pTab_EffectTool;
	delete pTab_UITool;
}

void CControlFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_CONTROLFORMVIEW, m_MapTab);
	//DDX_Control(pDX, IDC_TAB_MAIN, m_MapTab);
	//Ȥ�� ���� �ٲ�����.
	DDX_Control(pDX, IDC_LOCKTEXT, m_TextLockView);
	DDX_Text(pDX, IDC_LOCKTEXT, m_isLockView);
}


void CControlFormView::OnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iSelect = m_MapTab.GetCurSel();

	switch (iSelect)
	{
	case TERRAIN:
		pTab_MapTool->ShowWindow(SW_SHOW);
		pTab_Object_Tool->ShowWindow(SW_HIDE);
		pTab_EffectTool->ShowWindow(SW_HIDE);
		pTab_UITool->ShowWindow(SW_HIDE);

		m_eControl = CControlFormView::CControlFormView::CONTROLTAB::TERRAIN;
		break;
	case OBJECT:
		pTab_MapTool->ShowWindow(SW_HIDE);
		pTab_Object_Tool->ShowWindow(SW_SHOW);
		pTab_EffectTool->ShowWindow(SW_HIDE);
		pTab_UITool->ShowWindow(SW_HIDE);
		m_eControl = CControlFormView::CControlFormView::CONTROLTAB::OBJECT;
		break;
	case EFFECT:
		pTab_MapTool->ShowWindow(SW_HIDE);
		pTab_Object_Tool->ShowWindow(SW_HIDE);
		pTab_EffectTool->ShowWindow(SW_SHOW);
		pTab_UITool->ShowWindow(SW_HIDE);
		m_eControl = CControlFormView::CControlFormView::CONTROLTAB::EFFECT;
		break;
	case UI:
		pTab_MapTool->ShowWindow(SW_HIDE);
		pTab_Object_Tool->ShowWindow(SW_HIDE);
		pTab_EffectTool->ShowWindow(SW_HIDE);
		pTab_UITool->ShowWindow(SW_SHOW);
		m_eControl = CControlFormView::CControlFormView::CONTROLTAB::UI;
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CControlFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_MapTab.InsertItem(0, L"Terrain");
	m_MapTab.InsertItem(1, L"Game_Object");
	m_MapTab.InsertItem(2, L"Effect");
	m_MapTab.InsertItem(3, L"UI");

	m_MapTab.SetCurSel(0);

	CRect rect;
	m_MapTab.GetWindowRect(&rect);

	pTab_MapTool		= new CDIgTab0;
	pTab_MapTool->Create(IDD_TAB_DIALOG0, &m_MapTab);
	pTab_MapTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTab_MapTool->ShowWindow(SW_SHOW);
	m_eControl = CControlFormView::CONTROLTAB::TERRAIN;
	
	pTab_Object_Tool = new CDIgLog1;
	pTab_Object_Tool->Create(IDD_TAB_DIALOG1, &m_MapTab);
	pTab_Object_Tool->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTab_Object_Tool->ShowWindow(SW_HIDE);

	pTab_EffectTool		= new CDIgLog2;
	pTab_EffectTool->Create(IDD_TAB_DIALOG2, &m_MapTab);
	pTab_EffectTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTab_EffectTool->ShowWindow(SW_HIDE);

	pTab_UITool = new CUITabLog;
	pTab_UITool->Create(IDD_UITABLOG, &m_MapTab);
	pTab_UITool->MoveWindow(0, 25, rect.Width(), rect.Height());
	pTab_UITool->ShowWindow(SW_HIDE);
}

void CControlFormView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CMFCToolView* pView = (CMFCToolView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 1));
	if (m_isLock == false)
	{
		//pView->LockWindowUpdate();
		pView->KillTimer(1);
		m_isLock = true;
		m_isLockView = _T("LOCK");
	}
	else
	{
		pView->SetTimer(1, 1000 / 60, NULL);
		//pView->UnlockWindowUpdate();
		m_isLock = false;
		m_isLockView = _T("UNLOCK");
	}
	m_TextLockView.SetWindowTextW(m_isLockView);
}

BEGIN_MESSAGE_MAP(CControlFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CControlFormView::OnSelchangeTabMain)
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// CControlFormView �����Դϴ�.

#ifdef _DEBUG
void CControlFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlFormView �޽��� ó�����Դϴ�.
