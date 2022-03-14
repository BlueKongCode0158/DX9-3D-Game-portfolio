#pragma once
#include "afxwin.h"


// CStatic_Mesh_List ��ȭ �����Դϴ�.

class CStatic_Mesh_List : public CDialog
{
	DECLARE_DYNAMIC(CStatic_Mesh_List)

public:
	CStatic_Mesh_List(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStatic_Mesh_List();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATIC_MESH_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString		Get_LayerTag()
	{
		return m_pLayerTag;
	}
	CString		Get_PrototypeTag()
	{
		return m_pPrototypeTag;
	}
public:
	CListBox m_ListBox_Layer_Tag;
	CListBox m_List_Box_Prototype_Tag;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchange_Static_List();
	afx_msg void OnLbnSelchangeList_Prototype();
private:
	CString		m_pPrototypeTag;
	CString		m_pLayerTag;
public:
	afx_msg void OnBnClickedOk();
};
