#include "stdafx.h"
#include "Player.h"
#include "GameInstacne.h"
#include "Client_Defines.h"
#include "..\public\Player_Select_Dummy.h"

CPlayer_Select_Dummy::CPlayer_Select_Dummy(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPlayer_Select_Dummy::CPlayer_Select_Dummy(const CPlayer_Select_Dummy & rhs)
	:CGameObject(rhs)
{
}

HRESULT CPlayer_Select_Dummy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer_Select_Dummy::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_MatrixScale(0.04f, 0.04f, 0.04f);
	m_pMeshCom->Set_AnimationIndex(CPlayer::ANIMATION_KEY::POSE_LOOP, CPlayer::ANIMATION_KEY::POSE_LOOP);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, _float3(-12.6f, 0.0f, -8.7f));
	m_pTransformCom->SetUp_RotatinAxis(_float3(0.f, 1.f, 0.f), -D3DXToRadian(75.f));

	return S_OK;
}

_int CPlayer_Select_Dummy::Tick(double TimeDelta)
{

	return _int();
}

_int CPlayer_Select_Dummy::Late_Tick(double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);

	m_pMeshCom->Play_Animation(TimeDelta);
	return _int();
}

HRESULT CPlayer_Select_Dummy::Render()
{
	__super::Render();

	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(2);
	_uint iNumMeshContainers = m_pMeshCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; i++)
	{
		_uint iNumMaterial = m_pMeshCom->Get_NumMaterials(i);
		m_pMeshCom->Update_SkinnedMesh(i);

		for (_uint j = 0; j < iNumMaterial; j++)
		{
			if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i, j)))
			{
				return E_FAIL;
			}
			m_pShaderCom->Commit();
			m_pMeshCom->Render(i, j);
		}
	}
	m_pShaderCom->End_Shader();
}

HRESULT CPlayer_Select_Dummy::Add_Component()
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
	if (FAILED(__super::Add_Component(LEVEL_SELECT, TEXT("Prototype_Player_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer_Select_Dummy::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CPlayer_Select_Dummy * CPlayer_Select_Dummy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_Select_Dummy*	pInstance = new CPlayer_Select_Dummy(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Select_Dummy_Prototype");
	}
	return pInstance;
}

CGameObject * CPlayer_Select_Dummy::Clone(void * pArg)
{
	CPlayer_Select_Dummy*	pInstance = new CPlayer_Select_Dummy(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Select_Dummy_Clone");
	}
	return pInstance;
}

void CPlayer_Select_Dummy::Free()
{
	__super::Free();

	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
