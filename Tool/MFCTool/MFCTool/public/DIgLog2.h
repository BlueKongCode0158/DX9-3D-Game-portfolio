#pragma once
// CDIgLog2 대화 상자입니다.

class CDIgLog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDIgLog2)

public:
	CDIgLog2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDIgLog2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_TAB_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
