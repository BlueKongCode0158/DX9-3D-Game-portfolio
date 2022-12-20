// UITabLog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UI_Dummy.h"
#include "UITabLog.h"
#include "afxdialogex.h"
#include "GameInstacne.h"
#include "UICreate_Manager.h"


// CUITabLog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUITabLog, CDialogEx)
CUITabLog::CUITabLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_UITABLOG, pParent)
	, m_LayerName(_T(""))
	, m_fX(0)
	, m_fY(0)
	, m_fRotationY(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
{
	m_pManager = CUICreate_Manager::Get_Instance();
}

CUITabLog::~CUITabLog()
{
}

void CUITabLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_LayerName);
	DDX_Text(pDX, IDC_EDIT1, m_fX);
	DDX_Text(pDX, IDC_EDIT2, m_fY);
	DDX_Text(pDX, IDC_EDIT17, m_fRotationY);
	DDX_Text(pDX, IDC_EDIT18, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT19, m_fScaleY);
	DDX_Control(pDX, IDC_LIST1, m_UIListBox);
}


BEGIN_MESSAGE_MAP(CUITabLog, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUITabLog::OnLbnSelchangeList_UIList)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITabLog::OnBnClickedButton_Create)
	ON_BN_CLICKED(IDC_BUTTON12, &CUITabLog::OnBnClickedButton_Save)
	ON_BN_CLICKED(IDC_BUTTON2, &CUITabLog::OnBnClicked_EditButton)
	ON_BN_CLICKED(IDC_BUTTON7, &CUITabLog::OnBnClicked_Delete)
	ON_BN_CLICKED(IDC_BUTTON17, &CUITabLog::OnBnClicked_Load)
END_MESSAGE_MAP()


// CUITabLog 메시지 처리기입니다.


void CUITabLog::OnLbnSelchangeList_UIList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iIndex = m_UIListBox.GetCurSel();

	if (iIndex != LB_ERR)
	{
		m_UIListBox.GetText(iIndex, m_LayerName);
		m_pManager->Set_Select(m_LayerName.operator LPCWSTR());
		
		UIINFO tTempInfo;
		m_pManager->Get_Information(m_LayerName, tTempInfo);

		m_fX		= tTempInfo.vPosition.x;
		m_fY		= tTempInfo.vPosition.y;
		m_fScaleX	= tTempInfo.vScale.x;
		m_fScaleY	= tTempInfo.vScale.y;
		m_fRotationY = tTempInfo.fRadian;
	}
	UpdateData(FALSE);
}


void CUITabLog::OnBnClickedButton_Create()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// UI 생성
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	CUI_Dummy* pGameObject = dynamic_cast<CUI_Dummy*>(pInstance->Find_GameObject(LEVEL_STATIC, m_LayerName.operator LPCWSTR()));

	if (nullptr == pGameObject)
	{
		if (FAILED(pInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_3DUI"), m_LayerName.operator LPCWSTR(), &m_LayerName)))
		{
			RELEASE_INSTANCE(CGameInstacne);
			return;
		}
		m_UIListBox.AddString(m_LayerName);
	}
	else
	{
		MessageBoxA(m_hWnd, "해당 Layer는 존재합니다.", "OK", MB_OK);
		RELEASE_INSTANCE(CGameInstacne);
		UpdateData(FALSE);
		return;
	}
	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}


void CUITabLog::OnBnClickedButton_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"xml", L"*.xml");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\XML");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstPathName = Dlg.GetPathName();
		_tchar* pFilePath;
		pFilePath = wstPathName.GetBuffer(wstPathName.GetLength());
		wstPathName.ReleaseBuffer();
		m_pManager->Save_UI(pFilePath);
	}
}


void CUITabLog::OnBnClicked_EditButton()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_pManager)
	{
		return;
	}
	m_pManager->Set_Position(m_LayerName.operator LPCWSTR(), m_fX, m_fY);
	m_pManager->Set_Rotation(m_LayerName.operator LPCWSTR(), m_fRotationY);
	m_pManager->Set_Scale(m_LayerName.operator LPCWSTR(), m_fScaleX, m_fScaleY);
	UpdateData(FALSE);
}


void CUITabLog::OnBnClicked_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (nullptr == m_pManager)
	{
		return;
	}
	m_pManager->Delete_UI(m_LayerName.operator LPCWSTR());
	int iIndex = m_UIListBox.GetCurSel();
	m_UIListBox.DeleteString(iIndex);
	UpdateData(FALSE);
}


void CUITabLog::OnBnClicked_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
