// UITabLog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UITabLog.h"
#include "afxdialogex.h"
#include "UICreate_Manager.h"


// CUITabLog ��ȭ �����Դϴ�.

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
}


BEGIN_MESSAGE_MAP(CUITabLog, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUITabLog::OnLbnSelchangeList_UIList)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITabLog::OnBnClickedButton_Create)
	ON_BN_CLICKED(IDC_BUTTON12, &CUITabLog::OnBnClickedSaveButton)
END_MESSAGE_MAP()


// CUITabLog �޽��� ó�����Դϴ�.


void CUITabLog::OnLbnSelchangeList_UIList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUITabLog::OnBnClickedButton_Create()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// UI ����


}


void CUITabLog::OnBnClickedSaveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
