#include "stdafx.h"
#include "MainApp.h"
#include "GameInstacne.h"
#include "Renderer.h"

CMainApp::CMainApp()
{
}

HRESULT CMainApp::NativeConstruct()
{
	CGameInstacne*		pInstance = CGameInstacne::Get_Instance();
	if (nullptr == pInstance)
	{
		return E_FAIL;
	}
	Safe_AddRef(pInstance);

	Safe_Release(pInstance);


	//m_pGraphic_Device->ShowCursor(TRUE);
	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	CGameInstacne*		pInstance = CGameInstacne::Get_Instance();

	if (nullptr == pInstance)
	{
		return E_FAIL;
	}

	Safe_AddRef(pInstance);
	pInstance->Compute_MouseCursorPosInWorld(g_hWnd);
	pInstance->Compute_WindowCenterPosition();
	pInstance->Tick(TimeDelta);
	m_pFrustum->Update_Frustum();
	pInstance->Late_Tick(TimeDelta);

	Safe_Release(pInstance);

	return 0;
}

HRESULT CMainApp::Render()
{
	CGameInstacne* pInstance = CGameInstacne::Get_Instance();
	if (nullptr == pInstance)
	{
		return E_FAIL;
	}

	Safe_AddRef(pInstance);


	pInstance->Render_Begin();

	ShowCursor(FALSE);

	m_pRenderer->Render_GameObject();

	if (FAILED(pInstance->Render_Level()))
	{
		return E_FAIL;
	}

	pInstance->Render_End();
	Safe_Release(pInstance);
	return S_OK;
}

HRESULT CMainApp::Add_Prototype_GameObject()
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;

}

HRESULT CMainApp::Add_Prototype_Component()
{
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);


	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	Safe_AddRef(m_pRenderer);

	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Frustum"), m_pFrustum = CFrustum::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	Safe_AddRef(m_pFrustum);

	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), CTransform::Create(m_pGraphic_Device))))
	{
		goto failed;
	}

	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), CCollider::Create(m_pGraphic_Device, CCollider::COLLIDER::TYPE_AABB))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), CCollider::Create(m_pGraphic_Device, CCollider::COLLIDER::TYPE_OBB))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), CCollider::Create(m_pGraphic_Device, CCollider::COLLIDER::TYPE_SPHERE))))
	{
		goto failed;
	}

	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_2D"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_UI.hlsl")))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_3D"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_UI_3D.hlsl")))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Cursor"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_GENERIC, TEXT("../Bin/Resources/Image/UI/Cursor/texCursor_%d.tga"), m_pGraphic_Device, 2))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_Font"), CEngine_Font::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_STATIC, TEXT("Prototype_HpBar"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_GENERIC, TEXT("../Bin/Resources/Image/UI/Hp/texUICombatHealthbar.tga"), m_pGraphic_Device))))
	{
		goto failed;
	}

	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_LOGO, TEXT("Prototype_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_LOGO, TEXT("Prototype_Shader_SkyBox"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_SkyBox.hlsl")))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_LOGO, TEXT("Prototype_Texture_Cube"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_CUBE, TEXT("../bin/resources/skybox/SkyBox_%d.dds"), m_pGraphic_Device, 2))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_LOGO, TEXT("Prototype_VIBuffer_Logo_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../bin/resources/Textures/Logo_HeightMap_0.bmp"), 1.f))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_LOGO, TEXT("Prototype_Texture_Terrain"), CTexture::Create(CTexture::TEXTURETYPE::TEXTURE_GENERIC, TEXT("../bin/resources/Textures/TerrainTile%d.tga"), m_pGraphic_Device, 4))))
	{
		goto failed;
	}
	if (FAILED(pInstance->Add_Prototype_Component(LEVEL_LOGO, TEXT("Prototype_Shader_Terrain"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Terrain.hlsl")))))
	{
		goto failed;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
failed:
	RELEASE_INSTANCE(CGameInstacne);
	return E_FAIL;
}

HRESULT CMainApp::Add_Light_Desc()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	CGameInstacne *pGameInstance = GET_INSTANCE(CGameInstacne);


	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;

}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pGraphic_Device);
	CGameInstacne::Release_Engine();
}
