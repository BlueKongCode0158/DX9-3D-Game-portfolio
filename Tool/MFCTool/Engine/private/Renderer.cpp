#include "..\public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect_Viewport.h"
#include "Input_Device.h"
#include "Particle_System.h"
#include "Shader.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
{
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	if (nullptr == m_pTarget_Manager)
	{
		return E_FAIL;
	}

	D3DVIEWPORT9	Viewport;
	m_pGraphic_Device->GetViewport(&Viewport);

	/*Diffuse*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Diffuse"), Viewport.Width, Viewport.Height, D3DFMT_A8B8G8R8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
	{
		return E_FAIL;
	}

	/*Normal*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Normal"), Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f))))
	{
		return E_FAIL;
	}

	/*Shade*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Shade"), Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
	{
		return E_FAIL;
	}

	/*Specular*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Specular"), Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
	{
		return E_FAIL;
	}

	/*Depth*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Depth"), Viewport.Width, Viewport.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
	{
		return E_FAIL;
	}


	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
	{
		return E_FAIL;
	}

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
	{
		return E_FAIL;
	}

	m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
	if (nullptr == m_pVIBuffer)
	{
		return E_FAIL;
	}

	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}
#ifdef _DEBUG

	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Diffuse"), 0.f, 0.f, 200.f, 200.f)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Normal"), 0.f, 200.f, 200.f, 200.f)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Depth"), 0.f, 400.f, 200.f, 200.f)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Shade"), 200.f, 0.f, 200.f, 200.f)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Specular"), 200.f, 200.f, 200.f, 200.f)))
	{
		return E_FAIL;
	}

#endif // _DEBUG


	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject ||
		RENDER_END <= eGroup)
	{
		MSGBOX("Failed to Add_RenderGroup - Line 25");
		return E_FAIL;
	}

	m_RenderObjects[eGroup].push_back(pGameObject);
	Safe_AddRef(pGameObject);
	return S_OK;
}

HRESULT CRenderer::Add_RenderEffect(CParticle_System * pObject)
{
	if (nullptr == pObject)
	{
		return E_FAIL;
	}
	
	m_RenderEffects.push_back(pObject);
	Safe_AddRef(pObject);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_RenderDebug(CComponent * pComponent)
{
	m_DebugObject.push_back(pComponent);
	Safe_AddRef(pComponent);
	return S_OK;
}

HRESULT CRenderer::Render_Debug()
{
	CInput_Device* pInputDevice = GET_INSTANCE(CInput_Device);
	if (pInputDevice->Input_KeyBoard_Down(DIK_Q))
	{
		if (true == m_isDebugRender)
		{
			m_isDebugRender = false;
		}
		else
		{
			m_isDebugRender = true;
		}
	}
	RELEASE_INSTANCE(CInput_Device);

	if (true == m_isDebugRender)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Deferred"))))
		{
			return E_FAIL;
		}
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))
		{
			return E_FAIL;
		}
	}
	for (auto& pDebugObj : m_DebugObject)
	{
		if (true == m_isDebugRender)
		{
			pDebugObj->Render();
		}
		Safe_Release(pDebugObj);
	}
	m_DebugObject.clear();

	return S_OK;
}
#endif // _DEBUG


HRESULT CRenderer::Render_GameObject()
{
	Render_Priority();
	Render_NonAlpha();
	Render_LightAcc();
	Render_Blend();
	Render_Alpha();
	Render_UI();
#ifdef _DEBUG
	Render_Debug();
#endif // _DEBUG


	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(pGameObject->Render()))
		{
			MSGBOX("Failed to Render Object");
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();
	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Deferred"))))
	{
		return E_FAIL;
	}

	for (auto& pGameObject : m_RenderObjects[RENDER_NONALPHA])
	{
		if (FAILED(pGameObject->Render()))
		{
			MSGBOX("Failed to Render Object");
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONALPHA].clear();
	
	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_Deferred"))))
	{
		return E_FAIL;
	}
	return S_OK;
}

_bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_CamDistance() > pDest->Get_CamDistance();
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderObjects[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderObjects[RENDER_ALPHA])
	{
		if (FAILED(pGameObject->Render()))
		{
			MSGBOX("Failed to Render Object");
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_ALPHA].clear();
	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (FAILED(pGameObject->Render()))
		{
			MSGBOX("Failed to Render Object");
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_UI].clear();
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightAcc"))))
	{
		return E_FAIL;
	}

	CLight_Manager*		pLight_Manager = GET_INSTANCE(CLight_Manager);
	pLight_Manager->Render();
	RELEASE_INSTANCE(CLight_Manager);

	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_LightAcc"))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Diffuse")))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_ShadeTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Shade")))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_SpecularTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Specular")))))
	{
		return E_FAIL;
	}

	m_pShader->Begin_Shader(3);
	m_pVIBuffer->Render_VIBuffer();
	m_pShader->End_Shader();

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed tp Creating CRenderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTarget_Manager);
}
