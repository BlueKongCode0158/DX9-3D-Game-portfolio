#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstacne.h"
#include "..\public\Exit_Button.h"
#include "Mouse_Cursor.h"

CExit_Button::CExit_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{

}

CExit_Button::CExit_Button(const CExit_Button & rhs)
	:CUI(rhs)
{

}

HRESULT CExit_Button::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CExit_Button::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileExW(m_pGraphicDevice, TEXT("../bin/resources/Image/UI/Button/StartButton_1.tga"), 512, 128, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture_NonSelect)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CExit_Button::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	RECT rc;
	rc.left = m_tUIDesc.m_fX - m_tUIDesc.m_fSizeX * 0.5f;
	rc.top = m_tUIDesc.m_fY - m_tUIDesc.m_fSizeY*0.5f;
	rc.right = m_tUIDesc.m_fX + m_tUIDesc.m_fSizeX * 0.5f;
	rc.bottom = m_tUIDesc.m_fY + m_tUIDesc.m_fSizeY*0.5f;

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CMouse_Cursor* pGameMouse = dynamic_cast<CMouse_Cursor*>(pGameInstance->Find_GameObject(LEVEL_LOGO, TEXT("Layer_UI"), 2));

	if (true == PtInRect(&rc, pt))
	{
		m_isSelecting = true;
		if (nullptr != pGameMouse)
		{
			pGameMouse->Set_isActive(true);
		}
	}
	else if ( true != pGameMouse->Get_isActive())
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

_int CExit_Button::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	if (pGameInstance->Input_KeyMouse_Down(CInput_Device::DIM::DIM_LBUTTON) && true == m_isSelecting)
	{
		m_isSelect = true;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CExit_Button::Render()
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

HRESULT CExit_Button::SetUp_ConstantTable()
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

HRESULT CExit_Button::Add_Component()
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

CExit_Button * CExit_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExit_Button*		pInstance = new CExit_Button(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CExit_Button_Prorotype");
	}
	return pInstance;
}

CGameObject * CExit_Button::Clone(void * pArg)
{
	CExit_Button*		pInstance = new CExit_Button(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CExit_Button_Clone");
	}
	return pInstance;
}

void CExit_Button::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_NonSelect);
}
