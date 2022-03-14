#pragma once
#include "afxwin.h"
#include "Engine_Defines.h"
#include "MFC_Defines.h"
#include "afxcmn.h"

// CDIgTab0 대화 상자입니다.
BEGIN(MFC_TOOL)
class CSaveLoadInfo;
class CMFC_Terrain;
END


class CDIgTab0 : public CDialogEx
{
	DECLARE_DYNAMIC(CDIgTab0)

public:
	CDIgTab0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDIgTab0();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_TAB_DIALOG0 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CSaveLoadInfo*				m_pSaveInfoCom = nullptr;
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
private:
	map<wstring, TERRAININFO*>	m_mapTerrainList_Value;
public:
	int			m_iTerrainX;
	int			m_iTerrainZ;
	CString		m_TerrainName;
	CString		m_VIBuffer_Name;
	CString		m_LayerName;
private:
	/* Terrain */
	CListBox	m_Terrain_Value_ListBox;
	CStatic		m_TerrainPicture;
	CListBox	m_Terrain_ObjectList;
private:
	/* Slider Brush */
	CSliderCtrl m_BrushSizeSlider;
	CEdit		m_BrushSizeEdit;
	CSliderCtrl m_BrushPowerSlider;
	CEdit		m_editBrushPower;
private:
	/* Radio Button */
	CButton		m_BrushTypeRadio[2];
	CButton		m_BrushFunctionRadio[5];
	CButton		m_TerrainRenderState[4];
	CButton		m_BrushFilterColor[4];
private:
	CMFC_Terrain* m_pTerrain = nullptr;
public:
	/* Prototype_Terrain*/
	afx_msg void OnBnClicked_TerrainButton();
	afx_msg void OnBnClicked_CreateButton();
	afx_msg void OnBnClicked_Delete_PrototypeTerrainList();
public:
	afx_msg void OnLbnSelchange_TerrainCreateInfo_List();
	afx_msg void OnLbnSelchange_TerrainCloneList();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCloneDelete();
private:
	afx_msg void OnBnClicked_CircleRadio();
	afx_msg void OnBnClicked_RectRadio();
	afx_msg void OnBnClicked_BrushFunction_Up();
	afx_msg void OnBnClicked_BrushFunction_Down();
	afx_msg void OnBnClicked_BrushFunction_Splatting();
	afx_msg void OnBnClicked_VertexRadio_Smooth();
	afx_msg void OnBnClicked_VertexRadio_Flat();
	afx_msg void OnBnClicked_RedFilter();
	afx_msg void OnBnClicked_GreenFilter();
	afx_msg void OnBnClicked_BlueFilter();
	afx_msg void OnBnClicked_AlphaFilter();
private:
	afx_msg void OnBnClicked_ScaleChange();
	afx_msg void OnBnClicked_RotationChange();
	afx_msg void OnBnClicked_PositionChange();
public:
	afx_msg void OnBnClicked_FilterSave();
	afx_msg void OnBnClicked_RenderState_Wire();
	afx_msg void OnBnClicked_RenderState_Normal();
	afx_msg void OnBnClicked_RenderState_Rect();
	afx_msg void OnBnClicked_RenderState_Circle();
	afx_msg void OnBnClicked_SaveTerrain_Height();
	afx_msg void OnBnClicked_LoadTerrainHeightMap();
	afx_msg void OnBnClicked_FilterLoad();
};
