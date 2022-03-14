#include "stdafx.h"
#include "..\public\Wisp.h"
#include "GameInstacne.h"
#include "MonsterState.h"
#include "Normal_Monster_Hp.h"

CWisp::CWisp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster(pGraphic_Device)
{
}

CWisp::CWisp(const CWisp & rhs)
	: CMonster(rhs)
	, m_pHpBar(reinterpret_cast<CNormal_Monster_Hp*>(rhs.m_pHpBar->Clone()))
{
}

HRESULT CWisp::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	m_pHpBar = CNormal_Monster_Hp::Create(m_pGraphicDevice);
	return S_OK;
}

HRESULT CWisp::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_MatrixScale(0.025f, 0.025f, 0.025f);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, _float3(75.f, 20.f, 35.f));

	_matrix tMatrix;
	D3DXMatrixIdentity(&tMatrix);
	m_pMeshCom->Set_PivotMatrix(tMatrix);

	m_pMonsterState = CMonsterState::Create(m_pGraphicDevice, CMonsterState::MONSTERTYPE::WISP, this);
	if (nullptr == m_pMonsterState)
	{
		return E_FAIL;
	}

	CNormal_Monster_Hp::HPDESC	tHpDesc;
	tHpDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	tHpDesc.vIniPos = _float3(0.f, 2.f, 0.f);

	m_pHpBar->Set_tagHpDesc(&tHpDesc);
	m_iMaxHp = 35;
	m_iHp = m_iMaxHp;

	return S_OK;
}

_int CWisp::Tick(double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_iMonsterState =  m_pMonsterState->HandleInput();
	m_pMonsterState->Update(TimeDelta);

	if (CMonsterState::MOTION::DEATH == m_iMonsterState)
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CGameObject::OBJ_DEAD;
	}

	CComponent*	pTerrainBuffer = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer_Terrain"), 0);
	CComponent* pTerrainTrasform = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);

	m_pMeshCom->Update_Pivot();
	m_pHpBar->Tick(TimeDelta);

	RELEASE_INSTANCE(CGameInstacne);

	m_pColliderCom->Update_Collider();
	m_pColliderAttackCom->Update_Collider();
	m_pRecognition_ColliderCom->Update_Collider();

	_float	fHp = static_cast<_float>(m_iHp) / static_cast<_float>(m_iMaxHp);
	m_pHpBar->Set_HpPer(fHp);


	return _int();
}

_int CWisp::Late_Tick(double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pMonsterState->Late_Update(TimeDelta);
	m_pHpBar->Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
	{
		return E_FAIL;
	}

	return _int();
}

HRESULT CWisp::Render()
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

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderAttackCom);
	m_pRendererCom->Add_RenderDebug(m_pRecognition_ColliderCom);
#endif // _DEBUG

	return S_OK;
}

HRESULT CWisp::Add_Component()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Rederer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fTransformSpec = 4.f;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Wisp_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.vScale = _float3(55.f, 55.f, 55.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Recognition_Collider"), reinterpret_cast<CComponent**>(&m_pRecognition_ColliderCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Attack_Collider"), reinterpret_cast<CComponent**>(&m_pColliderAttackCom), &ColliderDesc)))
	{
		return E_FAIL;
	}
	return S_OK;

}

HRESULT CWisp::SetUp_ConstantTable()
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

CWisp * CWisp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWisp* pInstance = new CWisp(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CWisp_Prototype");
	}
	return pInstance;
}

CGameObject * CWisp::Clone(void * pArg)
{
	CWisp* pInstance = new CWisp(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CWisp_Clone");
	}
	return pInstance;
}

void CWisp::Free()
{
	__super::Free();

	Safe_Release(m_pHpBar);
	Safe_Release(m_pMonsterState);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pColliderAttackCom);
	Safe_Release(m_pRecognition_ColliderCom);
}
