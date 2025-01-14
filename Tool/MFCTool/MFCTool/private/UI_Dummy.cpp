#include "stdafx.h"
#include "..\public\UI_Dummy.h"
#include "GameInstacne.h"
#include "UICreate_Manager.h"
#include "tinyxml2.h"

#ifdef DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CUI_Dummy::CUI_Dummy(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CUI_Dummy::CUI_Dummy(CUI_Dummy & rhs)
	:CGameObject(rhs)
{

}

HRESULT CUI_Dummy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Dummy::NativeConstruct(void * pArg)
{
	m_pLayerTag = *static_cast<CString*>(pArg);

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	m_pParentTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_STATIC, TEXT("Layer_3DUI_Parent"), TEXT("Com_Transform")));
	RELEASE_INSTANCE(CGameInstacne);
	CUICreate_Manager::Get_Instance()->Add_UI(m_pLayerTag,this);
	return S_OK;
}

_int CUI_Dummy::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_isDead == true)
	{
		return OBJ_DEAD;
	}

	if (m_isSelect == true)
	{
		CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
		if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
		{
			m_pTransformCom->Walk_Up(TimeDelta * 0.1f);
		}
		if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
		{
			m_pTransformCom->Walk_Left(TimeDelta * 0.1f);
		}
		if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
		{
			m_pTransformCom->Walk_Down(TimeDelta * 0.1f);
		}
		if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
		{
			m_pTransformCom->Walk_Right(TimeDelta * 0.1f);
		}
		RELEASE_INSTANCE(CGameInstacne);
	}
	return _int();
}

_int CUI_Dummy::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);

	cout << m_isSelect << '\n';

	return _int();
}

HRESULT CUI_Dummy::Render()
{
	Update_Matrix();
	if (__super::Render())
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();
	return S_OK;
}

HRESULT CUI_Dummy::Set_Position(_float3 vecPosition)
{
	if (nullptr == m_pTransformCom)
	{
		return E_FAIL;
	}
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vecPosition);
	return S_OK;
}

void CUI_Dummy::Set_Select(_bool isSelect)
{
	m_isSelect = isSelect;
}

void CUI_Dummy::Set_Dead(_bool isDead)
{
	m_isDead = isDead;
}

void CUI_Dummy::Update_Matrix()
{
	if (nullptr == m_pParentTransform)
	{
		return;
	}
	_matrix _OriginalMatrix = *m_pTransformCom->Get_WorldMatrix();
	m_ParentMatrix = *m_pParentTransform->Get_WorldMatrix();
	m_WorldMatrix = _OriginalMatrix * m_ParentMatrix;
}

void CUI_Dummy::Get_Information(UIINFO & rInfo)
{
	rInfo.vPosition = m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	rInfo.vScale = m_pTransformCom->Get_Scale();
}

CTransform * CUI_Dummy::Get_Transform()
{
	if (nullptr == m_pTransformCom)
	{
		return nullptr;
	}
	return m_pTransformCom;
}

HRESULT CUI_Dummy::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;

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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_3D"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Dummy::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);
	/*	matrix  g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
	texture g_DiffuseTexture;*/
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_WorldMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CUI_Dummy * CUI_Dummy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Dummy* pInstance = new CUI_Dummy(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CUI_Dummy_Prototype");
	}
	return pInstance;
}

CGameObject * CUI_Dummy::Clone(void * pArg)
{
	CUI_Dummy* pInstance = new CUI_Dummy(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CUI_Dummy");
	}
	return pInstance;
}

void CUI_Dummy::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
