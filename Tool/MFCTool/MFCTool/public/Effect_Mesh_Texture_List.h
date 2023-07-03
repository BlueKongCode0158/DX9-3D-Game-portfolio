#pragma once


// CEffect_Mesh_Texture_List 대화 상자입니다.

class CEffect_Mesh_Texture_List : public CDialog
{
	DECLARE_DYNAMIC(CEffect_Mesh_Texture_List)

public:
	CEffect_Mesh_Texture_List(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffect_Mesh_Texture_List();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECT_MESH_TEXTURE_LIST };
#endif
private:
	CImage	m_PreviewImg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
