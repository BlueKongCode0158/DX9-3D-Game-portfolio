#include "stdafx.h"
#include "..\public\Static_Prob_Object.h"
#include "GameInstacne.h"
#include "Mesh_Manager.h"

CStatic_Prob_Object::CStatic_Prob_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CStatic_Prob_Object::CStatic_Prob_Object(const CStatic_Prob_Object & rhs)
	:CGameObject(rhs)
	,m_eType(rhs.m_eType)
	,m_eCreateType(rhs.m_eCreateType)
	,m_pPrototypeTag(rhs.m_pPrototypeTag)
{
}

HRESULT CStatic_Prob_Object::NativeConstruct_Prototype(CStatic_Prob_Object::Crete_Object eType)
{
	__super::NativeConstruct_Prototype();

	switch (eType)
	{
	case MFC_TOOL::CStatic_Prob_Object::TREE:
		m_pPrototypeTag = TEXT("Prototype_Tree_Mesh");
		break;
	case MFC_TOOL::CStatic_Prob_Object::ROCK:
		m_pPrototypeTag = TEXT("Prototype_Rock_Mesh");
		break;
	case MFC_TOOL::CStatic_Prob_Object::GRASS:
		m_pPrototypeTag = TEXT("Prototype_Grass_Mesh");
		break;
	case MFC_TOOL::CStatic_Prob_Object::RING:
		m_pPrototypeTag = TEXT("Prototype_Ring_Mesh");
		break;
	case MFC_TOOL::CStatic_Prob_Object::RING_BREAK:
		m_pPrototypeTag = TEXT("Prototype_Ring_Break_Mesh");
		break;
	case MFC_TOOL::CStatic_Prob_Object::PLAYER_SELECT:
		m_pPrototypeTag = TEXT("Prototype_Player_Select_Mesh");
		break;
	case MFC_TOOL::CStatic_Prob_Object::CRETEOBJECT_END:
		break;
	default:
		break;
	}

	m_eType = eType;
	return S_OK;
}

HRESULT CStatic_Prob_Object::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	switch (m_eType)
	{
	case MFC_TOOL::CStatic_Prob_Object::TREE:
		m_pPrototypeTag = TEXT("Prototype_Tree_Mesh");
		CMesh_Manager::Get_Instance()->Add_StaticMeshList(TEXT("Layer_Tree"), this);
		break;
	case MFC_TOOL::CStatic_Prob_Object::ROCK:
		m_pPrototypeTag = TEXT("Prototype_Rock_Mesh");
		CMesh_Manager::Get_Instance()->Add_StaticMeshList(TEXT("Layer_Rock"), this);
		break;
	case MFC_TOOL::CStatic_Prob_Object::GRASS:
		m_pPrototypeTag = TEXT("Prototype_Grass_Mesh");
		CMesh_Manager::Get_Instance()->Add_StaticMeshList(TEXT("Layer_Grass"), this);
		break;
	case MFC_TOOL::CStatic_Prob_Object::RING:
		m_pPrototypeTag = TEXT("Prototype_Ring_Mesh");
		CMesh_Manager::Get_Instance()->Add_StaticMeshList(TEXT("Layer_Ring"), this);
		break;
	case MFC_TOOL::CStatic_Prob_Object::RING_BREAK:
		m_pPrototypeTag = TEXT("Prototype_Ring_Break_Mesh");
		CMesh_Manager::Get_Instance()->Add_StaticMeshList(TEXT("Layer_Ring_Broken"), this);
		break;
	case MFC_TOOL::CStatic_Prob_Object::PLAYER_SELECT:
		m_pPrototypeTag = TEXT("Prototype_Player_Select_Mesh");
		CMesh_Manager::Get_Instance()->Add_StaticMeshList(TEXT("Layer_Player_Select"), this);
		break;
	case MFC_TOOL::CStatic_Prob_Object::CRETEOBJECT_END:
		break;
	default:
		break;
	}

	if (nullptr != pArg)
	{
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, *reinterpret_cast<_float3*>(pArg));
	}

	m_pTransformCom->Set_MatrixScale(0.005f, 0.005f, 0.005f);
	return S_OK;
}

_int CStatic_Prob_Object::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CStatic_Prob_Object::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CStatic_Prob_Object::Render()
{
	if (__super::Render())
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

	return S_OK;
}

HRESULT CStatic_Prob_Object::Add_Component()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pPrototypeTag, TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Mesh_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CStatic_Prob_Object::SetUp_ConstantTable()
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
CStatic_Prob_Object * CStatic_Prob_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CStatic_Prob_Object::Crete_Object eType)
{
	CStatic_Prob_Object* pInstance = new CStatic_Prob_Object(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CStatic_Prob_Object_Prototype");
	}
	return pInstance;
}

CGameObject * CStatic_Prob_Object::Clone(void * pArg)
{
	CStatic_Prob_Object* pInstance = new CStatic_Prob_Object(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CStatic_Prob_Object_Prototype");
	}
	return pInstance;
}

void CStatic_Prob_Object::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
}
