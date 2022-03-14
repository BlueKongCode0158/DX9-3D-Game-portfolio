// ..\private\DIgLog2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "..\public\DIgLog2.h"
#include "afxdialogex.h"


// CDIgLog2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDIgLog2, CDialogEx)

CDIgLog2::CDIgLog2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_TAB_DIALOG2, pParent)
{

}

CDIgLog2::~CDIgLog2()
{
}

void CDIgLog2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDIgLog2, CDialogEx)
END_MESSAGE_MAP()


// CDIgLog2 메시지 처리기입니다.
