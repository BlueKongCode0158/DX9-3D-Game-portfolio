// ..\private\Dynamic_Mesh_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "..\public\Dynamic_Mesh_List.h"
#include "afxdialogex.h"


// CDynamic_Mesh_List 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDynamic_Mesh_List, CDialog)

CDynamic_Mesh_List::CDynamic_Mesh_List(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DYNAMIC_MESH_LIST, pParent)
{

}

CDynamic_Mesh_List::~CDynamic_Mesh_List()
{
}

void CDynamic_Mesh_List::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_List_Box_Layer_Tag);
	DDX_Control(pDX, IDC_LIST5, m_List_Box_Prototype_Tag);
}


BEGIN_MESSAGE_MAP(CDynamic_Mesh_List, CDialog)
END_MESSAGE_MAP()


// CDynamic_Mesh_List 메시지 처리기입니다.
