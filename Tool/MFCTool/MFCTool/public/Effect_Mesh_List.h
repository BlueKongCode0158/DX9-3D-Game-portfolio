#pragma once


// CEffect_Mesh_List ��ȭ �����Դϴ�.

class CEffect_Mesh_List : public CDialog
{
	DECLARE_DYNAMIC(CEffect_Mesh_List)

public:
	CEffect_Mesh_List(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffect_Mesh_List();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECT_MESH_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
