#include "stdafx.h"
#include "Client_Defines.h"
#include "..\public\Player_Hp.h"
#include "GameInstacne.h"
#include "Player.h"

CPlayer_Hp::CPlayer_Hp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CPlayer_Hp::CPlayer_Hp(const CPlayer_Hp & rhs)
	:CUI(rhs)
{
}

HRESULT CPlayer_Hp::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlayer_Hp::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int CPlayer_Hp::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0));

	if (nullptr == pPlayer && nullptr == m_pHpPer)
	{
		goto End;
	}
	if (nullptr == m_pHpPer)
	{
		m_pHpPer = pPlayer->Get_HpPer();
	}


End:
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

_int CPlayer_Hp::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);
	return _int();
}

HRESULT CPlayer_Hp::Render()
{
	m_iShaderIndex = 3;
	if (nullptr == m_pHpPer)
	{
		_float fPer = 1.f;
		if (FAILED(m_pShaderCom->SetUp_ConstantTable("g_fHpPer", &fPer, sizeof(_float))))
		{
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(m_pShaderCom->SetUp_ConstantTable("g_fHpPer", m_pHpPer, sizeof(_float))))
		{
			return E_FAIL;
		}
	}
	if (FAILED(m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, 0)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlayer_Hp::Add_Component()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_HpBar"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

CPlayer_Hp * CPlayer_Hp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_Hp* pInstance = new CPlayer_Hp(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Hp_Prototype");
	}
	return pInstance;
}

CGameObject * CPlayer_Hp::Clone(void * pArg)
{
	CPlayer_Hp* pInstance = new CPlayer_Hp(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Hp_Clone");
	}
	return pInstance;
}

void CPlayer_Hp::Free()
{
	__super::Free();

}
