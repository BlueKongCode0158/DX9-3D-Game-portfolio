
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MFCTool.h"

#include "MainFrm.h"
#include "MFCToolView.h"
#include "ControlFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GameInstacne.h"
#include "Brush_Manager.h"
#include "UICreate_Manager.h"
#include "CellPoint_Manager.h"
#include "Mesh_Manager.h"

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
	CMesh_Manager::Destroy_Instance();
	CCellPoint_Manager::Destroy_Instance();
	CBrush_Manager::Destroy_Instance();
	CUICreate_Manager::Destroy_Instance();
	CGameInstacne::Release_Engine();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.hMenu = NULL;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = L"MAP_TOOL";
	cs.cx = 1480;
	cs.cy = 900;
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_tMainSplitter.CreateStatic(this, 1, 2);

	m_tMainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMFCToolView), CSize(WINCX, WINCY), pContext);
	m_tMainSplitter.CreateView(0, 0, RUNTIME_CLASS(CControlFormView), CSize(450, WINCY), pContext);

	return TRUE/*CFrameWnd::OnCreateClient(lpcs, pContext)*/;
}
