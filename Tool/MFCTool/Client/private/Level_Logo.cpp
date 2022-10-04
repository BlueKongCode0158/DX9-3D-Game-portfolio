#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstacne.h"
#include "Level_Logo.h"
#include "BackGround.h"
#include "Monster.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "Start_Button.h"
#include "Exit_Button.h"
#include "SkyBox.h"
#include "UI.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Logo::NativeConstruct()
{
	__super::NativeConstruct();

	if (FAILED(Ready_Light_Desc()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_UI(TEXT("Layer_UI"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Camera(TEXT("Layer_Camera"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		return E_FAIL;
	}
	CSound_Manager::Get_Instance()->PlayBGM(L"Main.mp3");

	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CStart_Button* pSelectButton = dynamic_cast<CStart_Button*>(pGameInstance->Find_GameObject(LEVEL_LOGO, TEXT("Layer_UI"), 0));
	CExit_Button*  pExitButton = dynamic_cast<CExit_Button*>(pGameInstance->Find_GameObject(LEVEL_LOGO, TEXT("Layer_UI"), 1));

	if (true == pSelectButton->Get_Select())
	{
		if (SUCCEEDED(pGameInstance->SetUp_Level(CLevel_Loading::Create(m_pGraphic_Deviec, LEVEL_SELECT))))
		{
			pGameInstance->Clear(LEVEL_LOGO);
			pGameInstance->Level_Relese_Light();
			goto succeeded;
		}
	}
	if (true == pExitButton->Get_Select())
	{
		DestroyWindow(g_hWnd);
	}

	RELEASE_INSTANCE(CGameInstacne);
	return -1;
succeeded:
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CLevel_Logo::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Light_Desc()
{
	CGameInstacne *pGameInstance = GET_INSTANCE(CGameInstacne);

	D3DLIGHT9	LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Deviec, LightDesc)))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	
	CUI::UIDESC	tUIDesc;
	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fSizeX = 1024;
	tUIDesc.m_fSizeY = 448;
	tUIDesc.m_fX = iWinCX >> 1;
	tUIDesc.m_fY = 300;

	if (FAILED(pInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_Logo_Title"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_Logo_Terrain"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);

	CSkyBox::SKYDESC SkyDesc;
	SkyDesc.iCurrentIndex = LEVEL_LOGO;
	SkyDesc.iSkyBoxNum = 0;
	
	if (FAILED(pInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_SkyBox"), pLayerTag, &SkyDesc)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Camera(const _tchar * pLayerTag)
{
	
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CCamera::CAMERADESC CameraDesc;

	CameraDesc.fAspect = static_cast<_float>(iWinCX) / iWinCY;
	CameraDesc.fEye = _float3(50.5f, 1.3f, 9.f);
	CameraDesc.fAt = _float3(48.4f, 3.6f, 57.2f);
	CameraDesc.fFar = 300.f;
	CameraDesc.fNear = 0.25f;
	CameraDesc.fFov = D3DXToRadian(80.f);


	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_Camera_Free"), pLayerTag, &CameraDesc)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
	
}

HRESULT CLevel_Logo::Ready_UI(const _tchar * pLayerTag)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);

	CUI::UIDESC	tUIDesc;

	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fSizeX = 256;
	tUIDesc.m_fSizeY = 64;
	tUIDesc.m_fX = 200;
	tUIDesc.m_fY = iWinCY - 500;

	if (FAILED(pInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_Start_Button"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	//Prototype_Exit_Button
	tUIDesc.m_fX = 200;
	tUIDesc.m_fY = iWinCY - 430;
	if (FAILED(pInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_Exit_Button"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}
	tUIDesc.m_fSizeX = 64;
	tUIDesc.m_fSizeY = 64;
	
	POINT	pt{};
	
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	tUIDesc.m_fX = pt.x;
	tUIDesc.m_fY = pt.y;

	if (FAILED(pInstance->Add_GameObject_Clone(LEVEL_LOGO, TEXT("Prototype_Cursor"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
