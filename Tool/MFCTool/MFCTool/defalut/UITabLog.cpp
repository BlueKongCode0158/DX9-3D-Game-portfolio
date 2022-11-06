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
END_MESSAGE_MAP()


// CUITabLog 메시지 처리기입니다.
