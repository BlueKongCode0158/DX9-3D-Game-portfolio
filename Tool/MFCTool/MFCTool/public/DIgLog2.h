#pragma once


// CDIgLog2 ��ȭ �����Դϴ�.

class CDIgLog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDIgLog2)

public:
	CDIgLog2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDIgLog2();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_TAB_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
