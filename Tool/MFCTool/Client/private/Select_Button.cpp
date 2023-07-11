#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstacne.h"
#include "..\public\Select_Button.h"
#include "Mouse_Cursor.h"

CSelect_Button::CSelect_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{

}

CSelect_Button::CSelect_Button(const CSelect_Button & rhs)
	:CUI(rhs)
{

}

HRESULT CSelect_Button::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSelect_Button::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileExW(m_pGraphicDevice, TEXT("../bin/resources/Image/UI/Button/Button_0.tga"), 256, 128, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture_NonSelect)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileExW(m_pGraphicDevice, TEXT("../bin/resources/Image/UI/Button/Button_1.tga"), 256, 128, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture_Select)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSelect_Button::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	RECT rc;
	rc.left		= m_tUIDesc.m_fX - m_tUIDesc.m_fSizeX * 0.5f;
	rc.top		= m_tUIDesc.m_fY - m_tUIDesc.m_fSizeY*0.5f;
	rc.right	= m_tUIDesc.m_fX + m_tUIDesc.m_fSizeX * 0.5f;
	rc.bottom	= m_tUIDesc.m_fY + m_tUIDesc.m_fSizeY*0.5f;

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CMouse_Cursor* pGameMouse = dynamic_cast<CMouse_Cursor*>(pGameInstance->Find_GameObject(LEVEL_SELECT, TEXT("Layer_UI"), 1));

	if (true == PtInRect(&rc, pt))
	{
		m_isSelecting = true;
		if (nullptr != pGameMouse)
		{
			pGameMouse->Set_isActive(true);
		}
	}
	else if (true == pGameMouse->Get_isActive())
	{
		m_isSelecting = false;
		if (nullptr != pGameMouse)
		{
			pGameMouse->Set_isActive(false);
		}
	}

	RELEASE_INSTANCE(CGameInstacne);

	return _int();
}

_int CSelect_Button::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup_Front(CRenderer::RENDERGROUP::RENDER_UI, this);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	if (pGameInstance->Input_KeyMouse_Down(CInput_Device::DIM::DIM_LBUTTON) && true == m_isSelecting)
	{
		m_isSelect = true;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CSelect_Button::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSelect_Button::SetUp_ConstantTable()
{
	if (FAILED(m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTexture_NonSelect)))
	{
		return E_FAIL;
	}

	if (false == m_isSelecting)
	{
		m_iShaderIndex = 0;
	}
	else
	{
		m_iShaderIndex = 2;
	}

	return S_OK;
}

HRESULT CSelect_Button::Add_Component()
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

	return S_OK;
}

CSelect_Button * CSelect_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSelect_Button*		pInstance = new CSelect_Button(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CSelect_Button_Prorotype");
	}
	return pInstance;
}

CGameObject * CSelect_Button::Clone(void * pArg)
{
	CSelect_Button*		pInstance = new CSelect_Button(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CSelect_Button_Clone");
	}
	return pInstance;
}

void CSelect_Button::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_NonSelect);
	Safe_Release(m_pTexture_Select);
}
