// DIgTab0.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "DIgTab0.h"
#include "afxdialogex.h"
#include "GameInstacne.h"
#include "SaveLoadInfo.h"
#include "MFC_Terrain.h"
#include "Brush_Manager.h"


// CDIgTab0 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDIgTab0, CDialogEx)

CDIgTab0::CDIgTab0(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_TAB_DIALOG0, pParent)
	, m_iTerrainX(0)
	, m_iTerrainZ(0)
	, m_VIBuffer_Name(_T(""))
	, m_LayerName(_T(""))
	, m_TerrainName(_T(""))
{

}

CDIgTab0::~CDIgTab0()
{
	for (auto& rPair : m_mapTerrainList_Value)
		Safe_Delete(rPair.second);
	m_mapTerrainList_Value.clear();
	Safe_Release(m_pSaveInfoCom);
	Safe_Release(m_pGraphic_Device);
}

void CDIgTab0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTerrainX);
	DDX_Text(pDX, IDC_EDIT2, m_iTerrainZ);
	DDX_Text(pDX, IDC_EDIT4, m_VIBuffer_Name);
	DDX_Text(pDX, IDC_EDIT5, m_TerrainName);
	DDX_Text(pDX, IDC_EDIT6, m_LayerName);
	DDX_Control(pDX, IDC_LIST2, m_Terrain_Value_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_TerrainPicture);
	DDX_Control(pDX, IDC_SLIDER2, m_BrushSizeSlider);
	DDX_Control(pDX, IDC_EDIT3, m_BrushSizeEdit);
	DDX_Control(pDX, IDC_TERRAIN_LIST, m_Terrain_ObjectList);
	DDX_Control(pDX, IDC_SLIDER3, m_BrushPowerSlider);
	DDX_Control(pDX, IDC_EDIT7, m_editBrushPower);
	DDX_Control(pDX, IDC_RADIO4, m_BrushTypeRadio[0]);
	DDX_Control(pDX, IDC_RADIO5, m_BrushTypeRadio[1]);
	DDX_Control(pDX, IDC_RADIO6, m_BrushFunctionRadio[0]);
	DDX_Control(pDX, IDC_RADIO7, m_BrushFunctionRadio[1]);
	DDX_Control(pDX, IDC_RADIO10, m_BrushFunctionRadio[2]);
	DDX_Control(pDX, IDC_RADIO8, m_BrushFunctionRadio[3]);
	DDX_Control(pDX, IDC_RADIO9, m_BrushFunctionRadio[4]);
	DDX_Control(pDX, IDC_RADIO1, m_BrushFilterColor[0]);
	DDX_Control(pDX, IDC_RADIO3, m_BrushFilterColor[1]);
	DDX_Control(pDX, IDC_RADIO2, m_BrushFilterColor[2]);
	DDX_Control(pDX, IDC_RADIO11, m_BrushFilterColor[3]);
	DDX_Control(pDX, IDC_RADIO12, m_TerrainRenderState[0]);
	DDX_Control(pDX, IDC_RADIO13, m_TerrainRenderState[1]);
	DDX_Control(pDX, IDC_RADIO14, m_TerrainRenderState[2]);
	DDX_Control(pDX, IDC_RADIO15, m_TerrainRenderState[3]);
}

void CDIgTab0::OnBnClicked_TerrainButton()
{
	UpdateData(TRUE);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	TERRAININFO*	pTerrainInfo = new TERRAININFO;
	pTerrainInfo->Terrain_Prototype_Name = m_TerrainName;
	pTerrainInfo->StringName = m_VIBuffer_Name;
	pTerrainInfo->Layer_Name = m_LayerName;
	pTerrainInfo->iTerrainX = m_iTerrainX;
	pTerrainInfo->iTerrainZ = m_iTerrainZ;

	auto& iter = m_mapTerrainList_Value.find(m_VIBuffer_Name.operator LPCWSTR());

	if (iter == m_mapTerrainList_Value.end())
	{
		MessageBoxA(m_hWnd, "저장 성공", "OK", MB_OK);

		//const _tchar*	pVIBuffer_PrototypeName = T2W(m_VIBuffer_Name.GetBuffer(0));
		//const _tchar*	pPrototype_TerrainName = T2W(m_TerrainName.GetBuffer(0));
		if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, m_VIBuffer_Name.operator LPCWSTR() , CVIBuffer_Terrain::Create(m_pGraphic_Device, m_iTerrainX, m_iTerrainZ))))
		{
			MessageBoxA(m_hWnd, "Terrain Buffer가 추가되지 않았습니다. ", "OK", MB_OK);
		}

		if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, m_TerrainName.operator LPCWSTR() ,CMFC_Terrain::Create(m_pGraphic_Device, m_iTerrainX, m_iTerrainZ ,m_VIBuffer_Name.operator LPCWSTR()))))
		{
			MessageBoxA(m_hWnd, "오브젝트가 추가되지 않았습니다. ", "OK", MB_OK);
		}
		m_mapTerrainList_Value.emplace(pTerrainInfo->StringName, pTerrainInfo);
		m_Terrain_Value_ListBox.AddString(pTerrainInfo->StringName);
	}
	else
	{
		Safe_Delete(pTerrainInfo);
		MessageBoxA(m_hWnd, "중복된 map이 존재합니다.", "OK", MB_OK);
	}
	
	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_CreateButton()
{
	UpdateData(TRUE);

	/* 이렇게 하면 동일한 사이즈의 Terrain을 두개 이상 생성하는게 안되는데? */

	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);

	//const _tchar*	pPrototype_TerrainName = T2W(m_TerrainName.GetBuffer(0));
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, m_TerrainName.operator LPCWSTR(), m_LayerName.operator LPCWSTR())))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	m_Terrain_ObjectList.AddString(m_LayerName);
	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_Delete_PrototypeTerrainList()
{
	UpdateData(TRUE);

	int iIndex = m_Terrain_Value_ListBox.GetCurSel();
	if (LB_ERR == iIndex)
	{
		return;
	}
	CString wstrFindName = L"";
	m_Terrain_Value_ListBox.GetText(iIndex, wstrFindName);
	
	CGameInstacne* pGameInstacne = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstacne->Delete_Prototype(LEVEL_STATIC, wstrFindName.operator LPCWSTR())))
	{
		MessageBoxA(m_hWnd, "해당 Prototype은 존재하지 않습니다.", "OK", MB_OK);
	}

	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}

void CDIgTab0::OnLbnSelchange_TerrainCreateInfo_List()
{
	UpdateData(TRUE);

	int iIndex = m_Terrain_Value_ListBox.GetCurSel();
	if (LB_ERR == iIndex)
	{
		return;
	}
	CString	wstrFindName = L"";
	m_Terrain_Value_ListBox.GetText(iIndex, wstrFindName);

	auto& iter = m_mapTerrainList_Value.find(wstrFindName.operator LPCWSTR());
	if (iter == m_mapTerrainList_Value.end())
	{
		return;
	}

	m_TerrainName	= iter->second->Terrain_Prototype_Name;
	m_VIBuffer_Name	= iter->second->StringName;
	m_LayerName		= iter->second->Layer_Name;
	m_iTerrainX		= iter->second->iTerrainX;
	m_iTerrainZ		= iter->second->iTerrainZ;

	UpdateData(FALSE);
}

void CDIgTab0::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IDC_SLIDER2 == pScrollBar->GetDlgCtrlID())
	{
		int iPos = m_BrushSizeSlider.GetPos();

		CString sPos;
		sPos.Format(L"%d", iPos);
		m_BrushSizeEdit.SetWindowTextW(sPos);

		CBrush_Manager::Get_Instance()->Set_BrushSize(iPos);
	}
	if (IDC_SLIDER3 == pScrollBar->GetDlgCtrlID())
	{
		int iPos = m_BrushPowerSlider.GetPos();

		CString sPos;
		sPos.Format(L"%d", iPos);
		m_editBrushPower.SetWindowTextW(sPos);

		CBrush_Manager::Get_Instance()->Set_BrushPower(iPos);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDIgTab0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_BrushSizeSlider.SetRange(0, 50);
	m_BrushSizeSlider.SetPos(1);
	m_BrushSizeSlider.SetLineSize(1);
	m_BrushSizeSlider.SetPageSize(1);
	CBrush_Manager::Get_Instance()->Set_BrushSize(1.f);


	m_BrushPowerSlider.SetRange(0, 50);
	m_BrushPowerSlider.SetPos(1);
	m_BrushPowerSlider.SetLineSize(1);
	m_BrushPowerSlider.SetPageSize(1);
	CBrush_Manager::Get_Instance()->Set_BrushPower(1.f);


	int iPos = m_BrushSizeSlider.GetPos();
	CString sPos;
	sPos.Format(L"%d", iPos);
	m_BrushSizeEdit.SetWindowTextW(sPos);

	iPos = m_BrushPowerSlider.GetPos();
	sPos.Format(L"%d", iPos);
	m_editBrushPower.SetWindowTextW(sPos);

	m_pSaveInfoCom = CSaveLoadInfo::Create(m_hWnd);

	m_BrushTypeRadio[0].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE);

	m_BrushFunctionRadio[0].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushFuctionType(CBrush_Manager::BRUSHFUNCTION::TERRAIN_UP);

	m_TerrainRenderState[1].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_RenderState(CBrush_Manager::TERRAINRENDERSTATE::TERRAIN_DEFAULT);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pGraphic_Device = pGameInstance->Get_Device();
	Safe_AddRef(m_pGraphic_Device);

	RELEASE_INSTANCE(CGameInstacne);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDIgTab0::OnBnClickedCloneDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_Terrain_ObjectList.GetCurSel();
	if (LB_ERR == iIndex)
	{
		return;
	}
	CString	wstrFindName = L"";
	m_Terrain_ObjectList.GetText(iIndex, wstrFindName);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


	RELEASE_INSTANCE(CGameInstacne);

	UpdateData(FALSE);

}

void CDIgTab0::OnLbnSelchange_TerrainCloneList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString wstrFindName = L"";
	int iIndex = m_Terrain_ObjectList.GetCurSel();
	m_Terrain_ObjectList.GetText(iIndex, wstrFindName);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	//CGameObject * CGameInstacne::Find_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndexNum)
	CMFC_Terrain* pTerrain = dynamic_cast<CMFC_Terrain*>(pGameInstance->Find_GameObject(LEVEL_STATIC, m_LayerName.operator LPCWSTR(), 0));
	
	if (nullptr == pTerrain)
	{
		RELEASE_INSTANCE(CGameInstacne);
		UpdateData(FALSE);
		return;
	}

	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_CircleRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushTypeRadio[0].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE);

	UpdateData(FALSE);
}


void CDIgTab0::OnBnClicked_RectRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushTypeRadio[1].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_RECT);

	UpdateData(FALSE);

}


void CDIgTab0::OnBnClicked_BrushFunction_Up()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFunctionRadio[0].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushFuctionType(CBrush_Manager::BRUSHFUNCTION::TERRAIN_UP);

	UpdateData(FALSE);

}


void CDIgTab0::OnBnClicked_BrushFunction_Down()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFunctionRadio[1].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushFuctionType(CBrush_Manager::BRUSHFUNCTION::TERRAIN_DOWN);

	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_BrushFunction_Splatting()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFunctionRadio[2].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushFuctionType(CBrush_Manager::BRUSHFUNCTION::TERRAIN_SPLATTING);

	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_VertexRadio_Smooth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFunctionRadio[3].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushFuctionType(CBrush_Manager::BRUSHFUNCTION::TERRAIN_SMOOTH);

	UpdateData(FALSE);

}


void CDIgTab0::OnBnClicked_VertexRadio_Flat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFunctionRadio[4].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_BrushFuctionType(CBrush_Manager::BRUSHFUNCTION::TERRAIN_FLAT);

	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_RedFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFilterColor[0].SetCheck(true);
	CBrush_Manager::Get_Instance()->is_BrushColor(CBrush_Manager::BRUSHCOLOR::RED);

	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_GreenFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFilterColor[1].SetCheck(true);
	CBrush_Manager::Get_Instance()->is_BrushColor(CBrush_Manager::BRUSHCOLOR::GREEN);

	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_BlueFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFilterColor[2].SetCheck(true);
	CBrush_Manager::Get_Instance()->is_BrushColor(CBrush_Manager::BRUSHCOLOR::BLUE);

	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_AlphaFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_BrushFilterColor[3].SetCheck(true);
	CBrush_Manager::Get_Instance()->is_BrushColor(CBrush_Manager::BRUSHCOLOR::ALPHA);

	UpdateData(FALSE);
}
void CDIgTab0::OnBnClicked_PositionChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_RotationChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_ScaleChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


	RELEASE_INSTANCE(CGameInstacne);
	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_FilterSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"bmp", L"*.bmp");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		//D3DXSaveTextureToFile(TEXT("../bin/Filter.bmp"), D3DXIFF_BMP, m_pFilterTexture, nullptr);

		CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
		CMFC_Terrain* pTerrain = dynamic_cast<CMFC_Terrain*>(pGameInstance->Find_GameObject(LEVEL_STATIC, m_LayerName, 0));

		if (nullptr == pTerrain)
		{
			RELEASE_INSTANCE(CGameInstacne);
			return;
		}
		D3DXSaveTextureToFile(wstrFilePath, D3DXIFF_BMP, pTerrain->Get_FilterTexture(), nullptr);
		
		RELEASE_INSTANCE(CGameInstacne);
	}

	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_RenderState_Wire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_TerrainRenderState[0].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_RenderState(CBrush_Manager::TERRAINRENDERSTATE::TERRAIN_WIRE);


	UpdateData(FALSE);

}


void CDIgTab0::OnBnClicked_RenderState_Normal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_TerrainRenderState[1].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_RenderState(CBrush_Manager::TERRAINRENDERSTATE::TERRAIN_DEFAULT);


	UpdateData(FALSE);

}


void CDIgTab0::OnBnClicked_RenderState_Rect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_TerrainRenderState[2].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_RenderState(CBrush_Manager::TERRAINRENDERSTATE::TERRAIN_RECT);


	UpdateData(FALSE);

}


void CDIgTab0::OnBnClicked_RenderState_Circle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_TerrainRenderState[3].SetCheck(true);
	CBrush_Manager::Get_Instance()->Set_RenderState(CBrush_Manager::TERRAINRENDERSTATE::TERRAIN_CIRCLE);


	UpdateData(FALSE);

}

void CDIgTab0::OnBnClicked_SaveTerrain_Height()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE , L"bmp", L"*.bmp");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();

		CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
		CMFC_Terrain* pTerrain = dynamic_cast<CMFC_Terrain*>(pGameInstance->Find_GameObject(LEVEL_STATIC, m_LayerName, 0));

		if (nullptr == pTerrain)
		{
			RELEASE_INSTANCE(CGameInstacne);
			return;
		}
		D3DXSaveTextureToFile(wstrFilePath, D3DXIFF_BMP, pTerrain->Get_HightMapTexture(), nullptr);

		RELEASE_INSTANCE(CGameInstacne);
	}
	UpdateData(FALSE);
}


void CDIgTab0::OnBnClicked_LoadTerrainHeightMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"bmp", L"*.bmp");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();

		CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
		//CMFC_Terrain* pTerrain = dynamic_cast<CMFC_Terrain*>(pGameInstance->Find_GameObject(LEVEL_STATIC, m_LayerName, 0));
		CVIBuffer_Terrain* pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(pGameInstance->Find_Component(LEVEL_STATIC, m_LayerName, TEXT("Com_Terrain"), 0));
		CMFC_Terrain* pTerrain = dynamic_cast<CMFC_Terrain*>(pGameInstance->Find_GameObject(LEVEL_STATIC, m_LayerName, 0));

		if (nullptr == pTerrainBuffer)
		{
			RELEASE_INSTANCE(CGameInstacne);
			return;
		}
		pTerrainBuffer->Update_TerrainFile_Height(wstrFilePath, pTerrain->Get_HightMapTexture());

		RELEASE_INSTANCE(CGameInstacne);
	}
	UpdateData(FALSE);
}

void CDIgTab0::OnBnClicked_FilterLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"bmp", L"*.bmp");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Save\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();

		CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
		CMFC_Terrain* pTerrain = dynamic_cast<CMFC_Terrain*>(pGameInstance->Find_GameObject(LEVEL_STATIC, m_LayerName, 0));

		if (nullptr == pTerrain)
		{
			RELEASE_INSTANCE(CGameInstacne);
			return;
		}
		pTerrain->Filter_Texture_Load(wstrFilePath);

		RELEASE_INSTANCE(CGameInstacne);
	}

	UpdateData(FALSE);
}


void CDIgTab0::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	Update_TerrainFilterPicture();
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDIgTab0::Update_TerrainFilterPicture()
{
	UpdateData(TRUE);

	CRect	tRect;
	m_TerrainPicture.GetWindowRect(tRect);
	CDC*	dc;
	dc = m_TerrainPicture.GetDC();

	CImage	Image;
	Image.Load(TEXT("../bin/Filter.bmp"));
	Image.StretchBlt(dc->m_hDC, 0, 0, tRect.Width(), tRect.Height(), SRCCOPY);
	ReleaseDC(dc);

	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDIgTab0, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON3, &CDIgTab0::OnBnClicked_TerrainButton)
ON_BN_CLICKED(IDC_BUTTON2, &CDIgTab0::OnBnClicked_CreateButton)
ON_BN_CLICKED(IDC_BUTTON7, &CDIgTab0::OnBnClicked_Delete_PrototypeTerrainList)
ON_LBN_SELCHANGE(IDC_LIST2, &CDIgTab0::OnLbnSelchange_TerrainCreateInfo_List)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_CLONE_DELETE, &CDIgTab0::OnBnClickedCloneDelete)
ON_LBN_SELCHANGE(IDC_TERRAIN_LIST, &CDIgTab0::OnLbnSelchange_TerrainCloneList)
ON_BN_CLICKED(IDC_RADIO4, &CDIgTab0::OnBnClicked_CircleRadio)
ON_BN_CLICKED(IDC_RADIO5, &CDIgTab0::OnBnClicked_RectRadio)
ON_BN_CLICKED(IDC_RADIO6, &CDIgTab0::OnBnClicked_BrushFunction_Up)
ON_BN_CLICKED(IDC_RADIO7, &CDIgTab0::OnBnClicked_BrushFunction_Down)
ON_BN_CLICKED(IDC_RADIO10, &CDIgTab0::OnBnClicked_BrushFunction_Splatting)
ON_BN_CLICKED(IDC_RADIO8, &CDIgTab0::OnBnClicked_VertexRadio_Smooth)
ON_BN_CLICKED(IDC_RADIO9, &CDIgTab0::OnBnClicked_VertexRadio_Flat)
ON_BN_CLICKED(IDC_RADIO1, &CDIgTab0::OnBnClicked_RedFilter)
ON_BN_CLICKED(IDC_RADIO3, &CDIgTab0::OnBnClicked_GreenFilter)
ON_BN_CLICKED(IDC_RADIO2, &CDIgTab0::OnBnClicked_BlueFilter)
ON_BN_CLICKED(IDC_RADIO11, &CDIgTab0::OnBnClicked_AlphaFilter)
ON_BN_CLICKED(IDC_BUTTON10, &CDIgTab0::OnBnClicked_ScaleChange)
ON_BN_CLICKED(IDC_BUTTON8, &CDIgTab0::OnBnClicked_RotationChange)
ON_BN_CLICKED(IDC_BUTTON5, &CDIgTab0::OnBnClicked_PositionChange)
ON_BN_CLICKED(IDC_BUTTON1, &CDIgTab0::OnBnClicked_FilterSave)
ON_BN_CLICKED(IDC_RADIO12, &CDIgTab0::OnBnClicked_RenderState_Wire)
ON_BN_CLICKED(IDC_RADIO13, &CDIgTab0::OnBnClicked_RenderState_Normal)
ON_BN_CLICKED(IDC_RADIO14, &CDIgTab0::OnBnClicked_RenderState_Rect)
ON_BN_CLICKED(IDC_RADIO15, &CDIgTab0::OnBnClicked_RenderState_Circle)
ON_BN_CLICKED(IDC_BUTTON11, &CDIgTab0::OnBnClicked_SaveTerrain_Height)
ON_BN_CLICKED(IDC_BUTTON9, &CDIgTab0::OnBnClicked_LoadTerrainHeightMap)
ON_BN_CLICKED(IDC_BUTTON12, &CDIgTab0::OnBnClicked_FilterLoad)
//ON_WM_KEYUP()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()
// CDIgTab0 메시지 처리기입니다.
