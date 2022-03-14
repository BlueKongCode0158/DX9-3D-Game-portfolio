// Static_Mesh_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MainFrm.h"
#include "Static_Mesh_List.h"
#include "ControlFormView.h"
#include "afxdialogex.h"
#include "DIgLog1.h"


// CStatic_Mesh_List 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStatic_Mesh_List, CDialog)

CStatic_Mesh_List::CStatic_Mesh_List(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_STATIC_MESH_LIST, pParent)
{

}

CStatic_Mesh_List::~CStatic_Mesh_List()
{
}

void CStatic_Mesh_List::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LayerText1, m_pLayerTag);
	DDX_Text(pDX, IDC_PrototypeText1, m_pPrototypeTag);
	DDX_Control(pDX, IDC_LIST4, m_ListBox_Layer_Tag);
	DDX_Control(pDX, IDC_LIST5, m_List_Box_Prototype_Tag);
}


BOOL CStatic_Mesh_List::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	/* 우선 주석 !*/
	//m_ListBox_Layer_Tag.AddString(TEXT("Layer_BackGround"));

	m_ListBox_Layer_Tag.AddString(TEXT("Layer_Rock"));
	m_ListBox_Layer_Tag.AddString(TEXT("Layer_Tree"));
	m_ListBox_Layer_Tag.AddString(TEXT("Layer_Ring"));
	m_ListBox_Layer_Tag.AddString(TEXT("Layer_Grass"));
	m_ListBox_Layer_Tag.AddString(TEXT("Layer_Ring_Broken"));
	m_ListBox_Layer_Tag.AddString(TEXT("Layer_Player_Select"));

	m_List_Box_Prototype_Tag.AddString(TEXT("Prototype_Static_Tree_Prob"));
	m_List_Box_Prototype_Tag.AddString(TEXT("Prototype_Static_Rock_Prob"));
	m_List_Box_Prototype_Tag.AddString(TEXT("Prototype_Static_Grass_Prob"));
	m_List_Box_Prototype_Tag.AddString(TEXT("Prototype_Static_Ring_Prob"));
	m_List_Box_Prototype_Tag.AddString(TEXT("Prototype_Static_Ring_Break_Prob"));
	m_List_Box_Prototype_Tag.AddString(TEXT("Prototype_Static_Character_Prob"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CStatic_Mesh_List::OnLbnSelchange_Static_List()
{
	UpdateData(TRUE);

	int iIndex = m_ListBox_Layer_Tag.GetCurSel();
	m_ListBox_Layer_Tag.GetText(iIndex, m_pLayerTag);

	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));

	CDIgLog1* pTab = pView->pTab_Object_Tool;
	pTab->Set_StaticLayerTag(m_pLayerTag);

	UpdateData(FALSE);
}
void CStatic_Mesh_List::OnLbnSelchangeList_Prototype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_List_Box_Prototype_Tag.GetCurSel();
	m_List_Box_Prototype_Tag.GetText(iIndex, m_pPrototypeTag);

	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));
	CDIgLog1* pTab = pView->pTab_Object_Tool;
	pTab->Set_StaticObjectTag(m_pPrototypeTag);

	UpdateData(FALSE);

}

void CStatic_Mesh_List::OnBnClickedOk()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDialog::OnOK();

}

BEGIN_MESSAGE_MAP(CStatic_Mesh_List, CDialog)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LIST4, &CStatic_Mesh_List::OnLbnSelchange_Static_List)
	ON_LBN_SELCHANGE(IDC_LIST5, &CStatic_Mesh_List::OnLbnSelchangeList_Prototype)
	ON_BN_CLICKED(IDOK, &CStatic_Mesh_List::OnBnClickedOk)
END_MESSAGE_MAP()


// CStatic_Mesh_List 메시지 처리기입니다.
