#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstacne.h"
#include "Player.h"
#include "Camera_Free.h"
#include "Character_Camera.h"
#include "Terrain.h"

#include "SkyBox.h"
#include "Gun_Left.h"
#include "Gun_Right.h"
#include "Pod.h"
#include "Bullet.h"

#include "Wisp.h"
#include "Static_Object.h"

#include "Golem.h"
#include "Beetle.h"
#include "Titan.h"
#include "Vagrant.h"

#include "ShotUI.h"
#include "Select_Stage_Object.h"
#include "Mouse_Cursor.h"
#include "UI_BackGround.h"
#include "Player_Select_Dummy.h"
#include "Select_Button.h"

#include "Player_Hp.h"
#include "Boss_Hp.h"
#include "BossHp_BackGround.h"


CLoader::CLoader(LPDIRECT3DDEVICE9 pGrephic_Device)
	:m_pGraphic_Device(pGrephic_Device)
{
	Safe_AddRef(pGrephic_Device);
}

HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CS);
	m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr));

	if (0 == m_hThread)
	{
		return E_FAIL;
	}

	return S_OK;
}

unsigned CLoader::Thread_Main(void * pArg)
{
	CLoader* pLoader = reinterpret_cast<CLoader*>(pArg);

	EnterCriticalSection(&pLoader->m_CS);

	HRESULT hr = 0;

	switch (pLoader->m_eNextLevel)
	{
	case LEVEL_GAMEPLAY0:
		hr = pLoader->GamePlay_Stage0();

		break;
	case LEVEL_GAMEPLAY1:
		hr = pLoader->GamePlay_Stage1();
		break;
	case LEVEL_GAMEPLAY2:
		hr = pLoader->GamePlay_Stage2();
		break;
	case LEVEL_SELECT:
		hr = pLoader->GameSelect_Stage();
		break;
	default:
		break;
	}

	if (FAILED(hr))
	{
		return 0;
	}

	LeaveCriticalSection(&pLoader->m_CS);

	return 0;
}

HRESULT CLoader::GameSelect_Stage()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	m_fLoadingPer = 5.f;

	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_BackGound_UI"), CUI_BackGround::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_Camera_Free"), CCamera_Free::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_Select_Object"), CSelect_Stage_Object::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_Dummy_Player"), CPlayer_Select_Dummy::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_Select_Botton"), CSelect_Button::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_Shader_Cube"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Cube.hlsl")))))
	{
		goto failed;
	}

	m_fLoadingPer = 35.f;
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_Cursor"), CMouse_Cursor::Create(m_pGraphic_Device))))
	{
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_Player_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Player/"), TEXT("Player17.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_Texture_Cube"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_CUBE, TEXT("../bin/resources/skybox/SkyBox_%d.dds"), m_pGraphic_Device, 2))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_SELECT, TEXT("Prototype_SkyBox"), CSkyBox::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_Shader_SkyBox"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_SkyBox.hlsl")))))
	{
		goto failed;
	}


	m_fLoadingPer = 75.f;
	/*----------------------------  Mesh Buffer --------------------------*/
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_Select_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/PlayerSelect/"), TEXT("PlayerSelect.X")))))
	{
		goto failed;
	}
	/*-----------------------------  Mesh Shader -------------------------*/
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_SELECT, TEXT("Prototype_Shader_Mesh"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Mesh.hlsl")))))
	{
		goto failed;
	}


	m_isFinish = true;
	m_fLoadingPer = 100.f;

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
failed:
	RELEASE_INSTANCE(CGameInstacne);
	return E_FAIL;
}

HRESULT CLoader::GamePlay_Stage0()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_fLoadingPer = 0.f;

	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Camera_Free"), CCamera_Free::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Camera_Character"), CCharacter_Camera::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Player"), CPlayer::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Terrain"), CTerrain::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Left"), CGun_Left::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Right"), CGun_Right::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Pod"), CPod::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Bullet"), CBullet::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Wisp"), CWisp::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_ShotUI"), CShotUI::Create(m_pGraphic_Device))))
	{
		goto failed;
	}

	m_fLoadingPer = 15.f;
	/* Monster */

	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Golem"), CGolem::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Beetle"), CBeetle::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Titan"), CTitan::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Vagrant"), CVagrant::Create(m_pGraphic_Device))))
	{
		goto failed;
	}

	m_fLoadingPer = 30.f;

	/* Prob Prototype_Object */

	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_00"), CStatic_Object::Create(m_pGraphic_Device, CStatic_Object::PROB::RROB_TREE))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_01"), CStatic_Object::Create(m_pGraphic_Device, CStatic_Object::PROB::PROB_ROCK))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_02"), CStatic_Object::Create(m_pGraphic_Device, CStatic_Object::PROB::PROB_GRASS))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_03"), CStatic_Object::Create(m_pGraphic_Device, CStatic_Object::PROB::PROB_BROKEN_RING))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_04"), CStatic_Object::Create(m_pGraphic_Device, CStatic_Object::PROB::PROB_RING))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_Player_Hp"), CPlayer_Hp::Create(m_pGraphic_Device))))
	{
		goto failed;
	}

	m_fLoadingPer = 45.f;

	/* ---------------------------------------------------- */
	if (FAILED(pGameInstance->Add_Prototype_Object(LEVEL_GAMEPLAY0, TEXT("Prototype_SkyBox"), CSkyBox::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_GENERIC ,TEXT("../bin/resources/Textures/TerrainTile%d.tga"),m_pGraphic_Device,4))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device,TEXT("../bin/resources/Textures/HeightTest_0103_7.bmp"),1.f))))
	{
		goto failed;
	}

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Cube"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_CUBE, TEXT("../bin/resources/skybox/SkyBox_%d.dds"), m_pGraphic_Device, 2))))
	{
		goto failed;
	}

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Mesh.hlsl")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Terrain"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Terrain.hlsl")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_SkyBox"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_SkyBox.hlsl")))))
	{
		goto failed;
	}
	if(FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0,TEXT("Prototype_Shader_Cube"),CShader::Create(m_pGraphic_Device,TEXT("../Bin/ShaderFiles/Shader_Cube.hlsl")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Player_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Player/"), TEXT("Player17.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Pod_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Pod/"), TEXT("Pod3.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Left_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Gun/Right/"), TEXT("Gun_Right.x")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Right_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Gun/Left/"), TEXT("Gun_Left.x")))))
	{
		goto failed;
	}

	/*----------------------Monster_Mesh----------------------------*/
	m_fLoadingPer = 55.f;

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Wisp_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Monster/Wisp/"), TEXT("Wisp.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Beetle_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Monster/Beetle/"), TEXT("BeetleGuard.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Golem_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Monster/Golem/"), TEXT("Golem.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Vagrant_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Monster/Vagrant/"), TEXT("Vagrant.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Titan_Mesh"), CDynamic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Dynamic_Mesh/Monster/Titan/"), TEXT("Titan.X")))))
	{
		goto failed;
	}


	/*-------------------Prototype_Prob---------------------*/
	m_fLoadingPer = 90.f;

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_00_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Prob/"), TEXT("Prob00.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_01_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Prob/"), TEXT("Prob01.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_02_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Prob/"), TEXT("Prob02.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_03_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Prob/"), TEXT("Prob03.X")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Prob_04_Mesh"), CStatic_Mesh::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Models/Static_Mesh/Prob/"), TEXT("Prob04.X")))))
	{
		goto failed;
	}
	/*-----------------Prototype_Prob------------------------*/

	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Navigation"), CNavigation::Create(m_pGraphic_Device, TEXT("../../Data/Save/Cell/Navigation_35_Terrain_71Per.dat")))))
	{
		goto failed;
	}
	if (FAILED(pGameInstance->Add_Prototype_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Crosshair_Texture"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_GENERIC, TEXT("../Bin/Resources/Image/UI/Crosshair/texCrosshair_%d.tga"), m_pGraphic_Device, 2))))
	{
		goto failed;
	}

	m_isFinish = true;
	m_fLoadingPer = 100.f;

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
failed:
	RELEASE_INSTANCE(CGameInstacne);
	return E_FAIL;
}

HRESULT CLoader::GamePlay_Stage1()
{
	return S_OK;
}

HRESULT CLoader::GamePlay_Stage2()
{
	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating Loader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CS);
	Safe_Release(m_pGraphic_Device);
	CloseHandle(m_hThread);
}
