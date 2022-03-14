#pragma once
#include "MFC_Defines.h"
#include "Static_Mesh_List.h"
#include "Dynamic_Mesh_List.h"
#include "afxwin.h"


// CDIgLog1 대화 상자입니다.

class CDIgLog1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDIgLog1)

public:
	CDIgLog1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDIgLog1();
public:
	CStatic_Mesh_List	m_Prototype_Static_Mesh;
	CDynamic_Mesh_List	m_Prototype_Dynamic_Mesh;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_TAB_DIALOG1 };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton	m_CreateRadioState[9];
	CButton m_SelectRadio[3];
	CButton m_TransformRadio[4];
public:
	CListBox m_CellPointListBox;
	CListBox m_Dynamic_ListBox;
	CListBox m_Static_ListBox;
public:
	CString		 m_StaticTag;
	CString		 m_DynamicTag;
	CString		 m_Static_CreateObjectTag;
	CString		 m_Dynamic_CreateObjectTag;
public:
	virtual BOOL OnInitDialog();
public:
	HRESULT	Update_NavigationCell_List();
	HRESULT	Update_Dynamic_Mesh_List();
	HRESULT Update_Static_Mesh_List();
public:
	void	Set_StaticLayerTag(CString tString);
	void	Set_DynamicLayerTag(CString tString);
	void	Set_StaticObjectTag(CString tString);
	void	Set_DynamicObjectTag(CString tString);
private:
	afx_msg void OnBnClicked_SaveCell();
	afx_msg void OnBnClicked_LoadCell();
	afx_msg void OnBnClicked_Dynamic_Mesh_List();
	afx_msg void OnBnClicked_Static_Mesh_List();
private:
	afx_msg void OnBnClicked_Dynamic_Object();
	afx_msg void OnBnClicked_Static_Object();
	afx_msg void OnBnClicked_Cell_Create();
	afx_msg void OnBnClicked_None_Default();
	afx_msg void OnBnClicked_Dynamic_Assert_Create();
	afx_msg void OnBnClicked_Static_Assert_Create();
	afx_msg void OnBnClicked_Cell_Assert_Create();
	afx_msg void OnBnClicked_Terrain_Select();
	afx_msg void OnBnClicked_Static_Select();
	afx_msg void OnBnClicked_Dynamic_Select();
private:
	afx_msg void OnLbnSelchange_NevigationList();
public:
	afx_msg void OnBnClicked_Transform_None();
	afx_msg void OnBnClicked_Transform_Position();
	afx_msg void OnBnClicked_Transform_Scale();
	afx_msg void OnBnClicked_Transform_Rotation();
	afx_msg void OnBnClicked_CW_Sort();
	afx_msg void OnBnClicked_DeleteButton();
	afx_msg void OnLbnSelchange_DynamicList();
	afx_msg void OnLbnSelchange_StaticList();
};
