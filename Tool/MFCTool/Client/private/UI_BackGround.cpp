#include "stdafx.h"
#include "GameInstacne.h"
#include "Client_Defines.h"
#include "..\public\UI_BackGround.h"

CUI_BackGround::CUI_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CUI_BackGround::CUI_BackGround(const CUI_BackGround & rhs)
	:CGameObject(rhs)
{
}

HRESULT CUI_BackGround::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_BackGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileExW(m_pGraphicDevice, TEXT("../bin/resources/Image/UI/BackGround/UI_Background_0.tga"), 1000, 928, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture)))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_MatrixScale(3.f, 5.f, 1.f);
	m_pTransformCom->SetUp_RotatinAxis(_float3(0.f, 1.f, 0.f), D3DXToRadian(80.f));
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, _float3(-13.0f, 2.5f, -4.3f));

	return S_OK;
}

_int CUI_BackGround::Tick(double TimeDelta)
{
	__super::Tick(TimeDelta);

	//CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	//if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
	//{
	//	m_pTransformCom->Walk_Look(TimeDelta);
	//}
	//if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
	//{
	//	m_pTransformCom->Rotation_Axis(_float3(0.f,1.f,0.f),-TimeDelta);
	//}
	//if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
	//{
	//	m_pTransformCom->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta);
	//}
	//if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
	//{
	//	m_pTransformCom->Walk_Back(TimeDelta);
	//}

	//if (pGameInstance->Input_KeyBoard_Pressing(DIK_V))
	//{
	//	m_pTransformCom->Rotation_Axis(_float3(0.f, 0.f, 1.f), -TimeDelta);
	//}
	//if (pGameInstance->Input_KeyBoard_Pressing(DIK_B))
	//{
	//	m_pTransformCom->Rotation_Axis(_float3(0.f, 0.f, 1.f), TimeDelta);
	//}


	//RELEASE_INSTANCE(CGameInstacne);

	return _int();
}

_int CUI_BackGround::Late_Tick(double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_ALPHA, this);

	//CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	//if (pGameInstance->Input_KeyBoard_Down(DIK_N))
	//{
	//	_float3 vPosition = m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	//	char str[100];

	//	sprintf(str, "x: %.1f y:%.1f z: %0.1f", vPosition.x, vPosition.y, vPosition.z);
	//	MessageBoxA(g_hWnd, str, NULL, 0);
	//}
	//RELEASE_INSTANCE(CGameInstacne);

	return _int();
}

HRESULT CUI_BackGround::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CUI_BackGround::Add_Component()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_3D"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 30.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CUI_BackGround::SetUp_ConstantTable()
{
	if (FAILED(m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTexture)))
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CUI_BackGround * CUI_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_BackGround* pInstance = new CUI_BackGround(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CUI_BackGround_Prototype");
	}
	return pInstance;
}

CGameObject * CUI_BackGround::Clone(void * pArg)
{
	CUI_BackGround* pInstance = new CUI_BackGround(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CUI_BackGround_Clone");
	}
	return pInstance;
}

void CUI_BackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}				