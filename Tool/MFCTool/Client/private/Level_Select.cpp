#include "stdafx.h"
#include "Client_Defines.h"
#include "..\public\Level_Select.h"
#include "GameInstacne.h"
#include "Select_Button.h"
#include "Level_Loading.h"
#include "SkyBox.h"
#include "Camera.h"
#include "UI.h"

CLevel_Select::CLevel_Select(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Select::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Light_Desc()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI_Button(TEXT("Layer_UI_Button"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Player(TEXT("Layer_Player"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CLevel_Select::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CSelect_Button* pSelectButton = dynamic_cast<CSelect_Button*>(pGameInstance->Find_GameObject(LEVEL_SELECT, TEXT("Layer_UI_Button"), 0));

	if (pSelectButton->Get_Select() == true)
	{
		if (SUCCEEDED(pGameInstance->SetUp_Level(CLevel_Loading::Create(m_pGraphic_Deviec, LEVEL_GAMEPLAY0))))
		{
			pGameInstance->Clear(LEVEL_SELECT);
			pGameInstance->Level_Relese_Light();
			goto succeeded;
		}
	}

	RELEASE_INSTANCE(CGameInstacne);
	return -1;
succeeded:
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CLevel_Select::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLevel_Select::Ready_Light_Desc()
{
	CGameInstacne *pGameInstance = GET_INSTANCE(CGameInstacne);

	D3DLIGHT9	LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 0.5f, 0.5f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Deviec, LightDesc)))
	{
		return E_FAIL;
	}

	LightDesc.Type = D3DLIGHT_POINT;
	LightDesc.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Position = _float3(-12.6f, 2.0f, -8.7f);
	LightDesc.Range = 4.f;

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Deviec, LightDesc)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_Select_Object"), pLayerTag)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_UI_Button(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


	CUI::UIDESC	tUIDesc;
	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fX = iWinCX * 0.5f;
	tUIDesc.m_fY = iWinCY - 70.f;
	tUIDesc.m_fSizeX = 256.f;
	tUIDesc.m_fSizeY = 128.f;

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_Select_Botton"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CSkyBox::SKYDESC SkyDesc;
	SkyDesc.iCurrentIndex = LEVEL_SELECT;
	SkyDesc.iSkyBoxNum = 1;
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_SkyBox"), pLayerTag,&SkyDesc)))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CCamera::CAMERADESC CameraDesc;

	CameraDesc.fAspect = static_cast<_float>(iWinCX) / iWinCY;
	CameraDesc.fAt = _float3(-10.f, 2.8f, -7.7f);
	CameraDesc.fEye = _float3(-17.f, 2.7f, -6.3f);
	CameraDesc.fFar = 300.f;
	CameraDesc.fNear = 0.25f;
	CameraDesc.fFov = D3DXToRadian(80.f);


	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_Camera_Free"), pLayerTag, &CameraDesc)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	POINT	pt{};

	CUI::UIDESC	tUIDesc;
	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	tUIDesc.m_fSizeX = 64;
	tUIDesc.m_fSizeY = 64;
	tUIDesc.m_fX = pt.x;
	tUIDesc.m_fY = pt.y;

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_Cursor"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_BackGound_UI"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Select::Ready_Player(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_SELECT, TEXT("Prototype_Dummy_Player"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

CLevel_Select * CLevel_Select::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Select* pInstance = new CLevel_Select(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CLevel_Select");
	}
	return pInstance;
}

void CLevel_Select::Free()
{
	__super::Free();
}
