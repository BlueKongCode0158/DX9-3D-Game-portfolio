// UITabLog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UITabLog.h"
#include "afxdialogex.h"


// CUITabLog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUITabLog, CDialogEx)

CUITabLog::CUITabLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UITABLOG, pParent)
{

}

CUITabLog::~CUITabLog()
{
}

void CUITabLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUITabLog, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUITabLog::OnLbnSelchangeList_UIList)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITabLog::OnBnClickedButton_Create)
END_MESSAGE_MAP()


// CUITabLog �޽��� ó�����Դϴ�.


void CUITabLog::OnLbnSelchangeList_UIList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUITabLog::OnBnClickedButton_Create()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
