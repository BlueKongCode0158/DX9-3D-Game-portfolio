// ..\private\DIgLog1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "..\public\DIgLog1.h"
#include "afxdialogex.h"
#include "CellPoint_Manager.h"
#include "Mesh_Manager.h"


// CDIgLog1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDIgLog1, CDialogEx)

CDIgLog1::CDIgLog1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_TAB_DIALOG1, pParent)
{

}

CDIgLog1::~CDIgLog1()
{
}

void CDIgLog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO1, m_CreateRadioState[0]);
	DDX_Control(pDX, IDC_RADIO2, m_CreateRadioState[1]);
	DDX_Control(pDX, IDC_RADIO3, m_CreateRadioState[2]);
	DDX_Control(pDX, IDC_RADIO16, m_CreateRadioState[3]);
	DDX_Control(pDX, IDC_RADIO11, m_CreateRadioState[4]);
	DDX_Control(pDX, IDC_RADIO17, m_CreateRadioState[5]);
	DDX_Control(pDX, IDC_RADIO18, m_CreateRadioState[6]);
	DDX_Control(pDX, IDC_RADIO20, m_CreateRadioState[7]);
	DDX_Control(pDX, IDC_RADIO21, m_CreateRadioState[8]);
	DDX_Control(pDX, IDC_RADIO7, m_SelectRadio[0]);
	DDX_Control(pDX, IDC_RADIO4, m_SelectRadio[1]);
	DDX_Control(pDX, IDC_RADIO5, m_SelectRadio[2]);
	DDX_Control(pDX, IDC_RADIO6, m_TransformRadio[0]);
	DDX_Control(pDX, IDC_RADIO8, m_TransformRadio[1]);
	DDX_Control(pDX, IDC_RADIO9, m_TransformRadio[2]);
	DDX_Control(pDX, IDC_RADIO19, m_TransformRadio[3]);
	DDX_Control(pDX, IDC_LIST1, m_CellPointListBox);
	DDX_Control(pDX, IDC_LIST2, m_Dynamic_ListBox);
	DDX_Control(pDX, IDC_LIST3, m_Static_ListBox);
}

BOOL CDIgLog1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_CreateRadioState[3].SetCheck(true);
	m_SelectRadio[0].SetCheck(true);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HRESULT CDIgLog1::Update_NavigationCell_List()
{
	m_CellPointListBox.ResetContent();

	_uint iIndexSize = CCellPoint_Manager::Get_Instance()->Get_CellPoint_Size();

	for (size_t i = 0; i < iIndexSize; i++)
	{
		CString iIndexNum;
		iIndexNum.Format(L"%d", i);

		m_CellPointListBox.AddString(iIndexNum);
	}
	return S_OK;
}

HRESULT CDIgLog1::Update_Dynamic_Mesh_List()
{
	m_Dynamic_ListBox.ResetContent();

	_uint iIndexSize = CMesh_Manager::Get_Instance()->Get_DynamicMapVectorSize(m_DynamicTag);

	for (size_t i = 0; i < iIndexSize; i++)
	{
		CString iIndexNum;
		iIndexNum.Format(L"%d", i);

		m_Dynamic_ListBox.AddString(iIndexNum);
	}
	return S_OK;
}

HRESULT CDIgLog1::Update_Static_Mesh_List()
{
	m_Static_ListBox.ResetContent();

	_uint iIndexSize = CMesh_Manager::Get_Instance()->Get_StaticMapVectorSize(m_StaticTag);

	for (size_t i = 0; i < iIndexSize; i++)
	{
		CString iIndexNum;
		iIndexNum.Format(L"%d", i);

		m_Static_ListBox.AddString(iIndexNum);
	}
	return S_OK;
}

void CDIgLog1::Set_StaticLayerTag(CString tString)
{
	SetDlgItemText(IDC_StaticTag0, tString);
	m_StaticTag = tString;
}

void CDIgLog1::Set_DynamicLayerTag(CString tString)
{
	SetDlgItemText(IDC_DynamicTag0, tString);
	m_DynamicTag = tString;
}

void CDIgLog1::Set_StaticObjectTag(CString tString)
{
	SetDlgItemText(IDC_StaticTag1, tString);
	m_Static_CreateObjectTag = tString;
}

void CDIgLog1::Set_DynamicObjectTag(CString tString)
{
	SetDlgItemText(IDC_DynamicTag1, tString);
	m_Dynamic_CreateObjectTag = tString;
}

void CDIgLog1::OnBnClicked_SaveCell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH,szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\Cell");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		CCellPoint_Manager::Get_Instance()->Save_VecCellPoint(wstrFilePath);

	}
}


void CDIgLog1::OnBnClicked_LoadCell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(TRUE, L"dat", L"*.dat");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\Cell");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		CCellPoint_Manager::Get_Instance()->Load_VecCellPoint(wstrFilePath);

		Update_NavigationCell_List();
	}

}


void CDIgLog1::OnBnClicked_Dynamic_Mesh_List()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Prototype_Dynamic_Mesh.GetSafeHwnd() == nullptr)
	{
		m_Prototype_Dynamic_Mesh.Create(IDD_DYNAMIC_MESH_LIST);
	}
	m_Prototype_Dynamic_Mesh.ShowWindow(SW_SHOW);
}

void CDIgLog1::OnBnClicked_Static_Mesh_List()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Prototype_Static_Mesh.GetSafeHwnd() == nullptr)
	{
		m_Prototype_Static_Mesh.Create(IDD_STATIC_MESH_LIST);
	}
	m_Prototype_Static_Mesh.ShowWindow(SW_SHOW);
}
void CDIgLog1::OnBnClicked_Dynamic_Object()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[0].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Static_Object()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[1].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Cell_Create()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[2].SetCheck(true);

	this;

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_None_Default()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[3].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Dynamic_Assert_Create()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[4].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Static_Assert_Create()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[5].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Cell_Assert_Create()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_CreateRadioState[6].SetCheck(true);

	UpdateData(FALSE);
}

void CDIgLog1::OnBnClicked_Terrain_Select()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_SelectRadio[0].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Static_Select()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_SelectRadio[7].SetCheck(true);

	UpdateData(FALSE);
}


void CDIgLog1::OnBnClicked_Dynamic_Select()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_SelectRadio[8].SetCheck(true);

	UpdateData(FALSE);
}

void CDIgLog1::OnLbnSelchange_NevigationList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_CellPointListBox.GetCurSel();
	if (LB_ERR == iIndex)
	{
		return;
	}
	CString	wstrFindName = L"";
	m_CellPointListBox.GetText(iIndex, wstrFindName);

	_uint iNum = _ttoi(wstrFindName);
	CCellPoint_Manager::Get_Instance()->Index_VecCellPoint_Select(iNum);

	UpdateData(FALSE);
}

void CDIgLog1::OnBnClicked_Transform_None()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDIgLog1::OnBnClicked_Transform_Position()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDIgLog1::OnBnClicked_Transform_Scale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDIgLog1::OnBnClicked_Transform_Rotation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDIgLog1::OnBnClicked_CW_Sort()
{
	UpdateData(TRUE);

	CCellPoint_Manager::Get_Instance()->CellPoint_CW_Sort();

	UpdateData(FALSE);
}

void CDIgLog1::OnBnClicked_DeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_CellPointListBox.GetCurSel();
	if (LB_ERR == iIndex)
	{
		return;
	}
	CString	wstrFindName = L"";
	m_CellPointListBox.GetText(iIndex, wstrFindName);

	_uint iNum = _ttoi(wstrFindName);
	CCellPoint_Manager::Get_Instance()->Select_VecCellPoint_Delete(iNum);
	CCellPoint_Manager::Get_Instance()->Cut_vecCellPoint();

	UpdateData(FALSE);
}
void CDIgLog1::OnLbnSelchange_DynamicList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	UpdateData(FALSE);
}

void CDIgLog1::OnLbnSelchange_StaticList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	this;

	UpdateData(FALSE);

}


BEGIN_MESSAGE_MAP(CDIgLog1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON6, &CDIgLog1::OnBnClicked_SaveCell)
	ON_BN_CLICKED(IDC_BUTTON9, &CDIgLog1::OnBnClicked_LoadCell)
	ON_BN_CLICKED(IDC_BUTTON3, &CDIgLog1::OnBnClicked_Dynamic_Mesh_List)
	ON_BN_CLICKED(IDC_BUTTON4, &CDIgLog1::OnBnClicked_Static_Mesh_List)
	ON_BN_CLICKED(IDC_RADIO1, &CDIgLog1::OnBnClicked_Dynamic_Object)
	ON_BN_CLICKED(IDC_RADIO2, &CDIgLog1::OnBnClicked_Static_Object)
	ON_BN_CLICKED(IDC_RADIO3, &CDIgLog1::OnBnClicked_Cell_Create)
	ON_BN_CLICKED(IDC_RADIO16, &CDIgLog1::OnBnClicked_None_Default)
	ON_BN_CLICKED(IDC_RADIO11, &CDIgLog1::OnBnClicked_Dynamic_Assert_Create)
	ON_BN_CLICKED(IDC_RADIO17, &CDIgLog1::OnBnClicked_Static_Assert_Create)
	ON_BN_CLICKED(IDC_RADIO18, &CDIgLog1::OnBnClicked_Cell_Assert_Create)
	ON_BN_CLICKED(IDC_RADIO7, &CDIgLog1::OnBnClicked_Terrain_Select)
	ON_BN_CLICKED(IDC_RADIO4, &CDIgLog1::OnBnClicked_Static_Select)
	ON_BN_CLICKED(IDC_RADIO5, &CDIgLog1::OnBnClicked_Dynamic_Select)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDIgLog1::OnLbnSelchange_NevigationList)
	ON_BN_CLICKED(IDC_RADIO6, &CDIgLog1::OnBnClicked_Transform_None)
	ON_BN_CLICKED(IDC_RADIO8, &CDIgLog1::OnBnClicked_Transform_Position)
	ON_BN_CLICKED(IDC_RADIO9, &CDIgLog1::OnBnClicked_Transform_Scale)
	ON_BN_CLICKED(IDC_RADIO19, &CDIgLog1::OnBnClicked_Transform_Rotation)
	ON_BN_CLICKED(IDC_BUTTON10, &CDIgLog1::OnBnClicked_CW_Sort)
	ON_BN_CLICKED(IDC_BUTTON11, &CDIgLog1::OnBnClicked_DeleteButton)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDIgLog1::OnLbnSelchange_DynamicList)
	ON_LBN_SELCHANGE(IDC_LIST3, &CDIgLog1::OnLbnSelchange_StaticList)
END_MESSAGE_MAP()


// CDIgLog1 메시지 처리기입니다.
