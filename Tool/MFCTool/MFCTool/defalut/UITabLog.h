#pragma once
#include "MFC_Defines.h"
#include "afxwin.h"

// CUITabLog 대화 상자입니다.
BEGIN(MFC_TOOL)
class CUICreate_Manager;
END

class CUITabLog : public CDialogEx
{
	DECLARE_DYNAMIC(CUITabLog)
public:
	CUITabLog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUITabLog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITABLOG };
#endif
private:
	CUICreate_Manager* m_pManager = nullptr;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList_UIList();
	afx_msg void OnBnClickedButton_Create();
	afx_msg void OnBnClickedButton_Save ();
public:
	void Update_LayerName(CString LayerName);
private:
	CString m_LayerName;
	float m_fX;
	float m_fY;
	float m_fRotationY;
	float m_fScaleX;
	float m_fScaleY;
	CListBox m_UIListBox;
public:
	afx_msg void OnBnClicked_EditButton();
	afx_msg void OnBnClicked_Delete();
	afx_msg void OnBnClicked_Load();
};
