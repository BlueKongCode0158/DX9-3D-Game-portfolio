#include "stdafx.h"
#include "..\public\Gun_Left.h"
#include "GameInstacne.h"


CGun_Left::CGun_Left(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CGun_Left::CGun_Left(const CGun_Left & rhs)
	:CGameObject(rhs)
{

}

HRESULT CGun_Left::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CGun_Left::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	_matrix ScaleMatrix, RotationX, RotationZ;
	D3DXMatrixRotationX(&RotationX, D3DXToRadian(90.f));
	D3DXMatrixRotationZ(&RotationZ, D3DXToRadian(180.f));
	D3DXMatrixScaling(&ScaleMatrix, 3.f, 3.f, 3.f);

	m_OriginMatrix = ScaleMatrix * RotationX * RotationZ;


	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CTransform* pPlayerTransformCom = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));
	if (nullptr == pPlayerTransformCom)
	{
		return E_FAIL;
	}
	m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	CDynamic_Mesh*	pPlayerMeshCom = reinterpret_cast<CDynamic_Mesh*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Mesh"), 0));
	if (nullptr == pPlayerMeshCom)
	{
		return E_FAIL;
	}
	m_pParentBoneMatrix = pPlayerMeshCom->Get_BoneMatrixPointer_Upper("gun_l");

	if (nullptr == m_pParentBoneMatrix)
	{
		MSGBOX(" Failed to Find Bone Matrix ");
		return E_FAIL;


	}

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

_int CGun_Left::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CGun_Left::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	_matrix WorldMatrix = m_OriginMatrix * ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));
	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
	{
		return -1;
	}
}

HRESULT CGun_Left::Render()
{
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	m_pShaderCom->Begin_Shader(0);

	_uint iNumMaterial = m_pMeshCom->Get_NumMaterials();
	for (_uint i = 0; i < iNumMaterial; i++)
	{
		if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE::TYPE_DIFFUSE, i)))
		{
			return E_FAIL;
		}
		m_pShaderCom->Commit();
		if (FAILED(m_pMeshCom->Render(i)))
		{
			return E_FAIL;
		}
	}
	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CGun_Left::Add_Component()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Gun_Left_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CGun_Left::SetUp_ConstantTable()
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

CGun_Left * CGun_Left::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGun_Left* pInstnace = new CGun_Left(pGraphic_Device);
	if (FAILED(pInstnace->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGun_Left_Prototype");
		Safe_Release(pInstnace);
	}
	return pInstnace;
}

CGameObject * CGun_Left::Clone(void * pArg)
{
	CGameObject* pInstance = new CGun_Left(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGun_Left_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGun_Left::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
}
