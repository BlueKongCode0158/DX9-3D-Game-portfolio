#include "stdafx.h"
#include "..\public\Vagrant.h"
#include "BossHp_BackGround.h"
#include "Boss_Hp.h"
#include "GameInstacne.h"
#include "MonsterState.h"

CVagrant::CVagrant(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster(pGraphic_Device)
{
}

CVagrant::CVagrant(const CVagrant & rhs)
	: CMonster(rhs)
	, m_pHpBackGround(dynamic_cast<CBossHp_BackGround*>(rhs.m_pHpBackGround->Clone()))
	, m_pHpBar(dynamic_cast<CBoss_Hp*>(rhs.m_pHpBar->Clone()))
{

}

HRESULT CVagrant::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	m_pHpBackGround = CBossHp_BackGround::Create(m_pGraphicDevice);
	m_pHpBar = CBoss_Hp::Create(m_pGraphicDevice);
	return S_OK;
}

HRESULT CVagrant::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_MatrixScale(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, _float3(80.f, 0.f, 60.f));
	m_pMeshCom->Set_AnimationIndex(CVagrant::ANIMATION_KEY::SPAWN);

	m_pMonsterState = CMonsterState::Create(m_pGraphicDevice, CMonsterState::MONSTERTYPE::VAGRANT, this);
	if (nullptr == m_pMonsterState)
	{
		return E_FAIL;
	}
	CBossHp_BackGround::tagUIDesc tUIDesc;

	tUIDesc.m_fWinSizeX = iWinCX;
	tUIDesc.m_fWinSizeY = iWinCY;
	tUIDesc.m_fSizeX = 1024.f;
	tUIDesc.m_fSizeY = 32.f;
	tUIDesc.m_fX = iWinCX * 0.5f;
	tUIDesc.m_fY = 200.f;

	m_pHpBackGround->Set_UIDesc(&tUIDesc);

	CBoss_Hp::tagUIDesc	tUIDesc0;

	tUIDesc0.m_fWinSizeX = iWinCX;
	tUIDesc0.m_fWinSizeY = iWinCY;
	tUIDesc0.m_fSizeX = 512.f;
	tUIDesc0.m_fSizeY = 48.f;
	tUIDesc0.m_fX = iWinCX * 0.5f;
	tUIDesc0.m_fY = 200.f;

	m_pHpBar->Set_UIDesc(&tUIDesc0);
	m_iMaxHp = 2100;
	m_iHp = m_iMaxHp;
	return S_OK;
}

_int CVagrant::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CComponent*	pTerrainBuffer = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer_Terrain"), 0);
	CComponent* pTerrainTrasform = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);
	m_pTransformCom->Stand_On_Plane(dynamic_cast<CTransform*>(pTerrainTrasform), dynamic_cast<CVIBuffer_Terrain*>(pTerrainBuffer));

	_float3	vPosition = m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition + _float3(0.f, 10.f, 0.f));

	RELEASE_INSTANCE(CGameInstacne);

	m_iMonsterState = m_pMonsterState->HandleInput();
	m_pMonsterState->Update(TimeDelta);

	m_pRecognition_ColliderCom->Update_Collider();
	m_pColliderAttackAreaCom->Update_Collider();
	m_pColliderAttackCom->Update_Collider();
	m_pColliderHand_LCom->Update_Collider();
	m_pColliderHand_RCom->Update_Collider();
	m_pColliderLegCom->Update_Collider();
	m_pColliderCom->Update_Collider();

	m_pHpBackGround->Tick(TimeDelta);
	m_pHpBar->Tick(TimeDelta);

	_float	fHp = static_cast<_float>(m_iHp) / static_cast<_float>(m_iMaxHp);
	m_pHpBar->Set_HpPer(fHp);

	return _int();
}

_int CVagrant::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pMonsterState->Late_Update(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*	pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (nullptr != pCollider)
	{
		if (m_pRecognition_ColliderCom->Collision_SPHERE(pCollider))
		{
			m_pHpBar->Late_Tick(TimeDelta);
			m_pHpBackGround->Late_Tick(TimeDelta);
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);
	m_pMeshCom->Play_Animation(TimeDelta);
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CVagrant::Render()
{
	__super::Render();

	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	_uint iShaderIndex = 2;
	m_pShaderCom->Begin_Shader(iShaderIndex);
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
			if (SUCCEEDED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_NormalTexture", MESHMATERIALTEXTURE::TYPE_NORMAL, i, j)))
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
			m_pMeshCom->Render(i, j);
		}
	}
	m_pShaderCom->End_Shader();

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pRecognition_ColliderCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderAttackAreaCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderAttackCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderHand_LCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderHand_RCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderLegCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
#endif // _DEBUG


	return S_OK;
}

HRESULT CVagrant::Add_Component()
{

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fTransformSpec = 5.f;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Vagrant_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("DetatchedHull");
	ColliderDesc.vScale = _float3(200.f, 100.f, 200.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = nullptr;
	ColliderDesc.vScale = _float3(100.f, 100.f, 100.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Recognition_Collider"), reinterpret_cast<CComponent**>(&m_pRecognition_ColliderCom),&ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = nullptr;
	ColliderDesc.vScale = _float3(60.f, 60.f, 60.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Attack_Collider"), reinterpret_cast<CComponent**>(&m_pColliderAttackCom), &ColliderDesc)))
	{
		return E_FAIL;
	}
	
	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("Tentacle1_007");
	ColliderDesc.vScale = _float3(100.f, 100.f, 300.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider_Attack_0"), reinterpret_cast<CComponent**>(&m_pColliderHand_LCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("Tentacle2_007");
	ColliderDesc.vScale = _float3(100.f, 100.f, 300.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider_Attack_1"), reinterpret_cast<CComponent**>(&m_pColliderHand_RCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("Tentacle3_007");
	ColliderDesc.vScale = _float3(100.f, 100.f, 300.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider_Attack_2"), reinterpret_cast<CComponent**>(&m_pColliderLegCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = nullptr;
	ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Area_Attack"), reinterpret_cast<CComponent**>(&m_pColliderAttackAreaCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CVagrant::SetUp_ConstantTable()
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

CVagrant * CVagrant::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVagrant* pInstance = new CVagrant(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CVagrant_Prototype");
	}
	return pInstance;
}

CGameObject * CVagrant::Clone(void * pArg)
{
	CVagrant* pInstance = new CVagrant(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CVagrant_Clone");
	}
	return pInstance;
}

void CVagrant::Free()
{
	__super::Free();

	Safe_Release(m_pHpBar);
	Safe_Release(m_pHpBackGround);

	Safe_Release(m_pMonsterState);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

	Safe_Release(m_pColliderLegCom);
	Safe_Release(m_pColliderAttackCom);
	Safe_Release(m_pColliderHand_LCom);
	Safe_Release(m_pColliderHand_RCom);
	Safe_Release(m_pColliderAttackAreaCom);
	Safe_Release(m_pRecognition_ColliderCom);
}
