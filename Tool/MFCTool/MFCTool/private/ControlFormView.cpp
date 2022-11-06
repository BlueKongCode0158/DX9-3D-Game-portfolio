// ..\private\ControlFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ControlFormView.h"

#include "DIgTab0.h"
#include "DIgLog1.h"
#include "DIgLog2.h"
#include "UITabLog.h"


// CControlFormView

IMPLEMENT_DYNCREATE(CControlFormView, CFormView)

CControlFormView::CControlFormView()
	: CFormView(IDD_CONTROLFORMVIEW)
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
	//혹시 몰라서 바꿔줬음.
}


void CControlFormView::OnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

BEGIN_MESSAGE_MAP(CControlFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CControlFormView::OnSelchangeTabMain)
END_MESSAGE_MAP()


// CControlFormView 진단입니다.

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


// CControlFormView 메시지 처리기입니다.
