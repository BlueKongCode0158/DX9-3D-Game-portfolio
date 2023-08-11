
// MFCToolView.cpp : CMFCToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "GameInstacne.h"
#include "MainFrm.h"
#include "MFC_Defines.h"
#include "Free_Camera.h"
#include "GameObject.h"
#include "Brush_Manager.h"
#include "CursorPoint.h"
#include "CellPoint.h"
#include "Static_Prob_Object.h"
#include "Canvas.h"
#include "UI_Dummy.h"
#include "tinyxml2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWND;
// CMFCToolView

IMPLEMENT_DYNCREATE(CMFCToolView, CView)

BEGIN_MESSAGE_MAP(CMFCToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMFCToolView 생성/소멸

CMFCToolView::CMFCToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCToolView::~CMFCToolView()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pRendererCom);
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCToolView 그리기

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	//CMFCToolDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (nullptr == m_pGraphic_Device)
	{
		return;
	}
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	pGameInstance->Render_Begin();

	m_pRendererCom->Render_GameObject();

	pGameInstance->Render_End(m_hWnd);
	pGameInstance->Render();
	RELEASE_INSTANCE(CGameInstacne);
}


// CMFCToolView 인쇄

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCToolView 진단

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView 메시지 처리기


void CMFCToolView::OnInitialUpdate()
{
	/* Scroll Test를 위해 ScrollView로 변환해줌
	   해줄려고 했지만 딱히 필요없을 것 같아서 보류함.*/
	//CScrollView::OnInitialUpdate();
	CView::OnInitialUpdate();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOMOVE);
	
	g_hWND = m_hWnd;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);
	HINSTANCE hInst = AfxGetInstanceHandle();


	if (FAILED(pGameInstance->Initialize_Engine(LEVEL_END, AfxGetMainWnd()->m_hWnd, hInst)))
	{
		MessageBoxA(g_hWND, "Failed to Creating Engine", "완료", MB_OK);
		return;
	}
	if (FAILED(pGameInstance->Ready_Graphic_Device(m_hWnd, CGraphic_Device::WINMODE::MODE_WIN, WINCX, WINCY, &m_pGraphic_Device)))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	if (FAILED(pGameInstance->Add_Timers(TEXT("FPS_60"))))
	{
		MessageBoxA(g_hWND, "Failed to Creating Timer", "완료", MB_OK);
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	if (FAILED(Add_Prototype_Component()))
	{
		MessageBoxA(g_hWND, "Failed to Creating Prototype_Component ", "완료", MB_OK);
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	if (FAILED(Add_Prototype_Object()))
	{
		MessageBoxA(g_hWND, "Failed to Creating Prototype_Object ", "완료", MB_OK);
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	if (FAILED(Add_GameObject()))
	{
		MessageBoxA(g_hWND, "Failed to Creating Game_Object ", "완료", MB_OK);
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}
	if (FAILED(Add_LightDest()))
	{
		MessageBoxA(g_hWND, "Failed to Creating Light ", "완료", MB_OK);
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}
	
	CBrush_Manager::Get_Instance()->NativeConstruct(m_pGraphic_Device);

	RELEASE_INSTANCE(CGameInstacne);

	SetTimer(1, 1000 / 60, NULL);
}


BOOL CMFCToolView::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.	

	return CView::DestroyWindow();
}


void CMFCToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		Update_Tick();
		OnDraw(nullptr);
		break;
	case 2:
		break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}


void CMFCToolView::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(1);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMFCToolView::Update_Tick()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	_float fTimeDelta = pGameInstance->Compute_Time(TEXT("FPS_60"));
	
	if (pGameInstance->Input_KeyMouse_Down(CInput_Device::DIM::DIM_WHEELBUTTON))
	{
		pGameInstance->OnOffWindow();
	}
	pGameInstance->Compute_MouseCursorPosInWorld(m_hWnd);
	pGameInstance->Tick(fTimeDelta);
	pGameInstance->Late_Tick(fTimeDelta);
	pGameInstance->Frame();
	RELEASE_INSTANCE(CGameInstacne);
}

HRESULT CMFCToolView::Add_Prototype_Object()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Camera_Free"), CFree_Camera::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Point"), CCursorPoint::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Cell"), CCellPoint::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Static_Tree_Prob"), CStatic_Prob_Object::Create(m_pGraphic_Device, CStatic_Prob_Object::Crete_Object::TREE))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Static_Rock_Prob"), CStatic_Prob_Object::Create(m_pGraphic_Device, CStatic_Prob_Object::Crete_Object::ROCK))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Static_Grass_Prob"), CStatic_Prob_Object::Create(m_pGraphic_Device, CStatic_Prob_Object::Crete_Object::GRASS))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Static_Ring_Prob"), CStatic_Prob_Object::Create(m_pGraphic_Device, CStatic_Prob_Object::Crete_Object::RING))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Static_Ring_Break_Prob"), CStatic_Prob_Object::Create(m_pGraphic_Device, CStatic_Prob_Object::Crete_Object::RING_BREAK))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Static_Character_Prob"), CStatic_Prob_Object::Create(m_pGraphic_Device, CStatic_Prob_Object::Crete_Object::PLAYER_SELECT))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_Canvas"), CCanvas::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_STATIC, TEXT("Prototype_3DUI"), CUI_Dummy::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CMFCToolView::Add_Prototype_Component()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), CTransform::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), m_pRendererCom = CRenderer::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	Safe_AddRef(m_pRendererCom);

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), CCollider::Create(m_pGraphic_Device, CCollider::TYPE_AABB))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), CCollider::Create(m_pGraphic_Device, CCollider::TYPE_OBB))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE))))
	{
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Terrain_Shader"), CShader::Create(m_pGraphic_Device,TEXT("../bin/ShaderFiles/Shader_Terrain_MFC.hlsl")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Point_Shader"), CShader::Create(m_pGraphic_Device, TEXT("../bin/ShaderFiles/Shader_Point.hlsl")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Mesh_Shader"), CShader::Create(m_pGraphic_Device, TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.hlsl")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_3D"), CShader::Create(m_pGraphic_Device, TEXT("../../Client/Bin/ShaderFiles/Shader_UI_3D.hlsl")))))
	{
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Terrain_Texture"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_GENERIC, TEXT("../bin/Reference/Texture/TerrainTile%d.tga"), m_pGraphic_Device, 4))))
	{
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Player_Select_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Reference/Mesh/Static/Select/"), TEXT("PlayerSelect.X")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Tree_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Reference/Mesh/Static/Prob/"), TEXT("Prob00.X")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Rock_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Reference/Mesh/Static/Prob/"), TEXT("Prob01.X")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Grass_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Reference/Mesh/Static/Prob/"), TEXT("Prob02.X")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Ring_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Reference/Mesh/Static/Prob/"), TEXT("Prob03.X")))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Ring_Break_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Reference/Mesh/Static/Prob/"), TEXT("Prob04.X")))))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CMFCToolView::Add_GameObject()
{

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CCamera::CAMERADESC CameraDesc;
	CameraDesc.fAspect = static_cast<_float>(WINCX) / WINCY;
	CameraDesc.fAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fEye = _float3(0.f, 5.f, -3.f);
	CameraDesc.fFar = 300.f;
	CameraDesc.fNear = 0.25f;
	CameraDesc.fFov = D3DXToRadian(60.f);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_Camera_Free"), TEXT("Layer_Camera"), &CameraDesc)))
	{
		RELEASE_INSTANCE(CGameInstacne);
		MessageBoxA(g_hWND, "Failed to Clone Camera_Free", "완료", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_Point"), TEXT("Layer_Point"))))
	{
		RELEASE_INSTANCE(CGameInstacne);
		MessageBoxA(g_hWND, "Failed to Clone Cursor", "완료", MB_OK);
		return E_FAIL;
	}
	LEVEL eLevel = LEVEL_STATIC;
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_STATIC, TEXT("Prototype_Canvas"), TEXT("Layer_3DUI_Parent"), &eLevel)))
	{
		RELEASE_INSTANCE(CGameInstacne);
		MessageBoxA(g_hWND, "Failed to Clone Canvas", "완료", MB_OK);
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CMFCToolView::Add_LightDest()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	D3DLIGHT9	LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}



void CMFCToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);

	m_pCursorPoint = point;

	CString strPoint;
	strPoint.Format(L"%04d, %04d", m_pCursorPoint.x, m_pCursorPoint.y);

	dc.TextOut(0, 0, strPoint);

	CView::OnMouseMove(nFlags, point);
}

