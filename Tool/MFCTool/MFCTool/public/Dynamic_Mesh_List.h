#pragma once
#include "afxwin.h"


// CDynamic_Mesh_List ��ȭ �����Դϴ�.

class CDynamic_Mesh_List : public CDialog
{
	DECLARE_DYNAMIC(CDynamic_Mesh_List)

public:
	CDynamic_Mesh_List(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDynamic_Mesh_List();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DYNAMIC_MESH_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List_Box_Layer_Tag;
	CListBox m_List_Box_Prototype_Tag;
};
