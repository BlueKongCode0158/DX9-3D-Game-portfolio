// UITabLog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UITabLog.h"
#include "afxdialogex.h"


// CUITabLog 대화 상자입니다.

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


// CUITabLog 메시지 처리기입니다.


void CUITabLog::OnLbnSelchangeList_UIList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUITabLog::OnBnClickedButton_Create()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
