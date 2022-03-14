#pragma once
#include "afxwin.h"


// CDynamic_Mesh_List 대화 상자입니다.

class CDynamic_Mesh_List : public CDialog
{
	DECLARE_DYNAMIC(CDynamic_Mesh_List)

public:
	CDynamic_Mesh_List(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDynamic_Mesh_List();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DYNAMIC_MESH_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List_Box_Layer_Tag;
	CListBox m_List_Box_Prototype_Tag;
};
