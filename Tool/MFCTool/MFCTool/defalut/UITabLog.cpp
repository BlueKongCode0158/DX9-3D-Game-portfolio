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
END_MESSAGE_MAP()


// CUITabLog �޽��� ó�����Դϴ�.
