#include "stdafx.h"
#include "GameInstacne.h"
#include "Client_Defines.h"
#include "..\public\Select_Stage_Object.h"

CSelect_Stage_Object::CSelect_Stage_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

HRESULT CSelect_Stage_Object::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSelect_Stage_Object::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	if (nullptr != pArg)
	{

	}

	m_pTransformCom->Set_MatrixScale(0.005f,0.005f, 0.005f);
	return S_OK;
}

_int CSelect_Stage_Object::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CSelect_Stage_Object::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CSelect_Stage_Object::Render()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	_uint iShaderIndex = 3;

	m_pShaderCom->Begin_Shader(2);

	_uint iNumMaterial = m_pMeshCom->Get_NumMaterials();
	for (_uint i = 0; i < iNumMaterial; i++)
	{
		/* Diffuse Map Setting */
		if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i)))
		{
			return E_FAIL;
		}
		/* Normal Map Setting */
		if (SUCCEEDED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_NormalTexture", MESHMATERIALTEXTURE::TYPE_NORMAL, i)))
		{
			m_pShaderCom->End_Shader();
			m_pShaderCom->Begin_Shader(iShaderIndex);
		}
		m_pShaderCom->Commit();
		m_pMeshCom->Render(i);
	}
	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CSelect_Stage_Object::Add_Component()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Rederer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fTransformSpec = 5.f;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_SELECT, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_SELECT, TEXT("Prototype_Select_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSelect_Stage_Object::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne*		pGameInstance = GET_INSTANCE(CGameInstacne);
	/*	matrix  g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
	texture g_DiffuseTexture;*/
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CSelect_Stage_Object * CSelect_Stage_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSelect_Stage_Object* pInstance = new CSelect_Stage_Object(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CSelect_Stage_Object_Prototype");
	}
	return pInstance;
}

CGameObject * CSelect_Stage_Object::Clone(void * pArg)
{
	CSelect_Stage_Object* pInstance = new CSelect_Stage_Object(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CSelect_Stage_Object_Clone");
	}
	return pInstance;
}

void CSelect_Stage_Object::Free()
{
	__super::Free();

	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
