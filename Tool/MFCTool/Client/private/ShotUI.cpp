#include	"stdafx.h"
#include	"Client_Defines.h"
#include	"..\public\ShotUI.h"
#include	"GameInstacne.h"

CShotUI::CShotUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{

}

HRESULT CShotUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CShotUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int CShotUI::Tick(_double TimeDelta)
{
	
	__super::Tick(TimeDelta);

	return _int();
}

_int CShotUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*		pCollider = nullptr;
	CTransform*		pTransform = nullptr;

	_uint		iMonsterListSize = pGameInstance->Get_GameObject_ListSize(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"));
	for (_uint i = 0; i < iMonsterListSize; i++)
	{
		pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), TEXT("Com_Collider"), i));
		pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), TEXT("Com_Transform"), i));

		m_pPosition = pGameInstance->Compute_CenterPoint(pCollider, pCollider->Get_WorldMatrix());

		if (nullptr != m_pPosition)
		{
			break;
		}
	}

	if (nullptr == m_pPosition)
	{
		m_isActive = false;
	}
	else
	{
		m_isActive = true;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CShotUI::Render()
{
	m_iShaderIndex = 0;
	if (FAILED(m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, m_isActive)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CShotUI::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_2D"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Crosshair_Texture"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

CShotUI * CShotUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShotUI* pInstance = new CShotUI(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CShot_UI_Prototype");
	}
	return pInstance;
}

CGameObject * CShotUI::Clone(void * pArg)
{
	CShotUI* pInstance = new CShotUI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CShot_UI_Prototype");
	}
	return pInstance;
}

void CShotUI::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
