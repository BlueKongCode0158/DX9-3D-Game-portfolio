#include "stdafx.h"
#include "..\public\Level_Loading.h"
#include "GameInstacne.h"
#include "Level_GamePlay_Stage0.h"
#include "Level_Select.h"
#include "LoadingBar.h"
#include "Loader.h"
#include "UI.h"
#include "Mouse_Cursor.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (eNextLevel == m_eNextLevel)
	{
		return E_FAIL;
	}
	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pGraphic_Deviec, eNextLevel);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UserInterface(TEXT("Layer_UI"))))
	{
		return E_FAIL;
	}


	if (nullptr == m_pLoader)
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CLevel_Loading::Tick(_double TimeDelta)
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);

	CLoadingBar*	pLoadingBar = dynamic_cast<CLoadingBar*>(pInstance->Find_GameObject(LEVEL_LOADING, TEXT("Layer_UI"), 0));
	pLoadingBar->Set_LoadingPer(m_pLoader->Get_LoadingPer());

	RELEASE_INSTANCE(CGameInstacne);
	if (true == m_pLoader->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			CGameInstacne*	pGameInstance	= GET_INSTANCE(CGameInstacne);
			CLevel*			pLevel			= nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_SELECT:
				pLevel = CLevel_Select::Create(m_pGraphic_Deviec);
				break;
			case LEVEL_GAMEPLAY0:
				pLevel = CLevel_GamePlay_Stage0::Create(m_pGraphic_Deviec);
				break;
			default:
				break;
			}
			pGameInstance->Clear(LEVEL_LOADING);
			pGameInstance->SetUp_Level(pLevel);
			
			RELEASE_INSTANCE(CGameInstacne);
		}
	}
	return _int();
}

HRESULT CLevel_Loading::Render()
{
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UserInterface(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CUI::UIDESC	tUIDesc;
	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fSizeX = 320;
	tUIDesc.m_fSizeY = 64;
	tUIDesc.m_fX = iWinCX - 200;
	tUIDesc.m_fY = iWinCY - 128;


	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_LOADING, TEXT("Prototype_Loading_Bar"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	POINT	pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	tUIDesc.m_fSizeX = 64;
	tUIDesc.m_fSizeY = 64;
	tUIDesc.m_fX = pt.x;
	tUIDesc.m_fY = pt.y;

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_LOADING, TEXT("Prototype_Cursor"), pLayerTag, &tUIDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CUI::UIDESC	tUIDesc;
	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fSizeX = iWinCX;
	tUIDesc.m_fSizeY = iWinCY;
	tUIDesc.m_fX = iWinCX >> 1;
	tUIDesc.m_fY = iWinCY >> 1;

	if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_LOADING, TEXT("Prototype_BackGround"), pLayerTag,&tUIDesc)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}


CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(HRESULT(pInstance->NativeConstruct(eNextLevel))))
	{
		MSGBOX("Failed to Creating CLevel_Loading_Object");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
