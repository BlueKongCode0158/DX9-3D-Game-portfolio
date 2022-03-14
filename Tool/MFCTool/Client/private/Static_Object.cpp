#include "stdafx.h"
#include "..\public\Static_Object.h"
#include "GameInstacne.h"

CStatic_Object::CStatic_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CStatic_Object::CStatic_Object(const CStatic_Object & rhs)
	:CGameObject(rhs)
	,m_pLayerTag(rhs.m_pLayerTag)
{
}

HRESULT CStatic_Object::NativeConstruct_Prototype(PROB eType)
{
	switch (eType)
	{
	case PROB::RROB_TREE:
		m_pLayerTag = TEXT("Prototype_Prob_00_Mesh");
		break;
	case PROB::PROB_ROCK:
		m_pLayerTag = TEXT("Prototype_Prob_01_Mesh");
		break;
	case PROB::PROB_GRASS:
		m_pLayerTag = TEXT("Prototype_Prob_02_Mesh");
		break;
	case PROB::PROB_BROKEN_RING:
		m_pLayerTag = TEXT("Prototype_Prob_03_Mesh");
		break;
	case PROB::PROB_RING:
		m_pLayerTag = TEXT("Prototype_Prob_04_Mesh");
		break;
	}
	return S_OK;
}

HRESULT CStatic_Object::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		_float3 vPosition = {};

		memcpy(vPosition, pArg, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition);
	}
	m_pTransformCom->Set_MatrixScale(0.005f, 0.005f, 0.005f);

	return S_OK;
}

_int CStatic_Object::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CStatic_Object::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CStatic_Object::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	_uint iShaderIndex = 2;

	m_pShaderCom->Begin_Shader(iShaderIndex);

	_uint iNumMaterial = m_pMeshCom->Get_NumMaterials();
	for (_uint i = 0; i < iNumMaterial; i++)
	{
		m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE::TYPE_DIFFUSE, i);

		if (nullptr == m_pMeshCom->Get_Material_Diffuse(MESHMATERIALTEXTURE::TYPE::TYPE_DIFFUSE, i))
		{
			m_pShaderCom->SetUp_ConstantTable("g_MtlTexture", _float4(0.2f, 0.2f, 0.2f, 1.f), sizeof(_float4));
		}

		if (SUCCEEDED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_NormalTexture", MESHMATERIALTEXTURE::TYPE_NORMAL, i)))
		{
			iShaderIndex = 3;
			m_pShaderCom->End_Shader();
			m_pShaderCom->Begin_Shader(iShaderIndex);
		}
		else
		{
			iShaderIndex = 2;
			m_pShaderCom->End_Shader();
			m_pShaderCom->Begin_Shader(iShaderIndex);
		}
		m_pShaderCom->Commit();
		if (FAILED(m_pMeshCom->Render(i)))
		{
			return E_FAIL;
		}
	}

	m_pShaderCom->End_Shader();
	m_pShaderCom->SetUp_ConstantTable("g_MtlTexture", _float4(0.f, 0.f, 0.f, 0.f), sizeof(_float4));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

HRESULT CStatic_Object::Add_Component()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, m_pLayerTag, TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Frustum"), TEXT("Com_Frustum"), reinterpret_cast<CComponent**>(&m_pFrustumCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CStatic_Object::SetUp_ConstantTable()
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

CStatic_Object * CStatic_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device, PROB eType)
{
	CStatic_Object* pInstance = new CStatic_Object(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CStatic_Object_Prototype");
	}
	return pInstance;
}

CGameObject * CStatic_Object::Clone(void * pArg)
{
	CStatic_Object* pInstance = new CStatic_Object(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CStatic_Object_Clone");
	}
	return pInstance;
}

void CStatic_Object::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
}
