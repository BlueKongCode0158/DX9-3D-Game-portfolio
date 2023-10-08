
// MainFrm.cpp : CMainFrame 클래스의 구현
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
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.hMenu = NULL;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = L"MAP_TOOL";
	cs.cx = 1480;
	cs.cy = 900;
	return TRUE;
}

// CMainFrame 진단

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


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_tMainSplitter.CreateStatic(this, 1, 2);

	m_tMainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMFCToolView), CSize(WINCX, WINCY), pContext);
	m_tMainSplitter.CreateView(0, 0, RUNTIME_CLASS(CControlFormView), CSize(450, WINCY), pContext);

	return TRUE/*CFrameWnd::OnCreateClient(lpcs, pContext)*/;
}
