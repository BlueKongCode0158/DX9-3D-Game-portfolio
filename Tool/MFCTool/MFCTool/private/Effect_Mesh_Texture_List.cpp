// ../private/Effect_Mesh_Texture_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "../public/Effect_Mesh_Texture_List.h"
#include "afxdialogex.h"


// CEffect_Mesh_Texture_List 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffect_Mesh_Texture_List, CDialog)

CEffect_Mesh_Texture_List::CEffect_Mesh_Texture_List(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EFFECT_MESH_TEXTURE_LIST, pParent)
{

}

CEffect_Mesh_Texture_List::~CEffect_Mesh_Texture_List()
{
}

void CEffect_Mesh_Texture_List::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffect_Mesh_Texture_List, CDialog)
END_MESSAGE_MAP()


// CEffect_Mesh_Texture_List 메시지 처리기입니다.
