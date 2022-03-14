#include "stdafx.h"
#include "..\public\Level_GamePlay_Stage0.h"
#include "GameInstacne.h"
#include "Monster.h"
#include "Player.h"
#include "Camera_Free.h"
#include "Pod.h"
#include "UI.h"


CLevel_GamePlay_Stage0::CLevel_GamePlay_Stage0(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay_Stage0::NativeConstruct()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(Ready_Light_Desc()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Pod(TEXT("Layer_Pod"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Light(TEXT("Layer_Light"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		return E_FAIL;
	}

	CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::BGM);

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

_int CLevel_GamePlay_Stage0::Tick(_double DeltaTime)
{
	__super::Tick(DeltaTime);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	
	if (pGameInstance->Input_KeyBoard_Down(DIK_C))
	{
		if (false == g_isCheat)
		{
			g_isCheat = true;
		}
		else
		{
			g_isCheat = false;
		}
	}

	if (false == m_isStart)
	{
		CPod* pGameObject = dynamic_cast<CPod*>(pGameInstance->Find_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Pod"), 0));
		if (nullptr == pGameObject)
		{
			return 0;
		}
		m_isStart = pGameObject->isOpen();
	}
	else if( true == m_isStart && false == m_isCreate)
	{
		CSound_Manager::Get_Instance()->PlayBGM(L"Field.mp3");

		if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		{
			return 0;
		}
		m_isCreate = true;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CLevel_GamePlay_Stage0::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLevel_GamePlay_Stage0::Ready_Light_Desc()
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

HRESULT CLevel_GamePlay_Stage0::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CUI::UIDESC	tUIDesc;
	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fSizeX = 64;
	tUIDesc.m_fSizeY = 64;
	tUIDesc.m_fX = iWinCX * 0.5f;
	tUIDesc.m_fY = iWinCY * 0.5f;

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_ShotUI"), pLayerTag,&tUIDesc)))
	{
		return E_FAIL;
	}


	tUIDesc.m_fSizeX = 400.f;
	tUIDesc.m_fSizeY = 32.;
	tUIDesc.m_fX = 300.f;
	tUIDesc.m_fY = iWinCY - 100.f;

	//Prototype_Player_Hp
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Player_Hp"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_GamePlay_Stage0::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	//if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Wisp"), pLayerTag)))
	//{
	//	return E_FAIL;
	//}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Golem"), pLayerTag)))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Beetle"), pLayerTag)))
	{
		return E_FAIL;
	}
	//if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Titan"), pLayerTag)))
	//{
	//	return E_FAIL;
	//}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Vagrant"), pLayerTag)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;

}

HRESULT CLevel_GamePlay_Stage0::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Player"), pLayerTag)))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Left"), pLayerTag)))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Right"), pLayerTag)))
	{
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}


HRESULT CLevel_GamePlay_Stage0::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CCamera::CAMERADESC CameraDesc;

	CameraDesc.fAspect = static_cast<_float>(iWinCX) / iWinCY;
	CameraDesc.fAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fEye = _float3(0.f, 5.f, -3.f);
	CameraDesc.fFar = 300.f;
	CameraDesc.fNear = 0.25f;
	CameraDesc.fFov = D3DXToRadian(80.f);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Camera_Character"), pLayerTag, &CameraDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_GamePlay_Stage0::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Terrain"), pLayerTag)))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_SkyBox"), pLayerTag)))
	{
		goto failed;
	}


	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_00"), pLayerTag, &_float3(38.f, 2.f, 38.f))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_01"), pLayerTag, &_float3(38.f, 2.f, 40.f))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_02"), pLayerTag, &_float3(38.f, 2.f, 42.f))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_03"), pLayerTag, &_float3(38.f, 2.f, 44.f))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_04"), pLayerTag, &_float3(38.f, 2.f, 46.f))))
	{
		goto failed;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;

failed:
	RELEASE_INSTANCE(CGameInstacne);
	return E_FAIL;
}

HRESULT CLevel_GamePlay_Stage0::Ready_Layer_Pod(const _tchar* pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Pod"), pLayerTag)))
	{
		goto failed;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
failed:
	RELEASE_INSTANCE(CGameInstacne);
	return E_FAIL;
}

HRESULT CLevel_GamePlay_Stage0::Ready_Light(const _tchar * pLayerTag)
{

	if (nullptr == m_pGraphic_Deviec)
		return E_FAIL;

	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);

	/* 방향성 광원을 셋팅한다. */
	D3DLIGHT9			LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Deviec, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CLevel_GamePlay_Stage0 * CLevel_GamePlay_Stage0::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay_Stage0* pInstance = new CLevel_GamePlay_Stage0(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay_Stage0");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_GamePlay_Stage0::Free()
{
	__super::Free();
}
