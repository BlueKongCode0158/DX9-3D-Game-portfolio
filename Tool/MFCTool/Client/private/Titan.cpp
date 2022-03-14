#include "stdafx.h"
#include "..\public\Titan.h"
#include "GameInstacne.h"
#include "MonsterState.h"

CTitan::CTitan(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster(pGraphic_Device)
{
}

CTitan::CTitan(const CTitan & rhs)
	: CMonster(rhs)
{
}

HRESULT CTitan::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CTitan::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	m_pTransformCom->Set_MatrixScale(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, _float3(85.f, 0.f, 62.f));

	m_pMonsterState = CMonsterState::Create(m_pGraphicDevice, CMonsterState::MONSTERTYPE::TITAN, this);

	m_iHp = 2100;
	return S_OK;
}

_int CTitan::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CComponent*	pTerrainBuffer = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer_Terrain"), 0);
	CComponent* pTerrainTrasform = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);
	CCollider*	pColliderCom = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"),0));
	CCollider*	pOBBCollidercom = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_OBBCollider"), 0));
	m_pTransformCom->Stand_On_Plane(dynamic_cast<CTransform*>(pTerrainTrasform), dynamic_cast<CVIBuffer_Terrain*>(pTerrainBuffer));

	RELEASE_INSTANCE(CGameInstacne);

	m_iMonsterState = m_pMonsterState->HandleInput();
	m_pMonsterState->Update(TimeDelta);

	m_pColliderCom->Update_Collider();
	m_pColliderHand_RCom->Update_Collider();
	m_pColliderHand_LCom->Update_Collider();
	m_pColliderAttackCom->Update_Collider();
	m_pRecognition_ColliderCom->Update_Collider();

	if (nullptr != pColliderCom)
	{
		m_pColliderCom->Collision_AABB(pColliderCom);
	}
	if (nullptr != pOBBCollidercom)
	{
		m_pColliderHand_LCom->Collision_OBB(pOBBCollidercom);
		m_pColliderHand_RCom->Collision_OBB(pOBBCollidercom);
	}

	return _int();
}

_int CTitan::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);
	m_pMeshCom->Play_Animation(TimeDelta);

	m_pMonsterState->Late_Update(TimeDelta);

	return _int();
}

HRESULT CTitan::Render()
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
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderHand_LCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderHand_RCom);
	m_pRendererCom->Add_RenderDebug(m_pColliderAttackCom);
	m_pRendererCom->Add_RenderDebug(m_pRecognition_ColliderCom);
#endif // _DEBUG


	return S_OK;
}

HRESULT CTitan::Add_Component()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Titan_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("chest");
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(500.f, 2500.f, 500.f);
	ColliderDesc.vIniPos = _float3(0.f, 20.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = nullptr;
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	//ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Attack_Collider"), reinterpret_cast<CComponent**>(&m_pColliderAttackCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = nullptr;
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(60.f, 60.f, 60.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_Sphere"), TEXT("Com_Recognition_Collider"), reinterpret_cast<CComponent**>(&m_pRecognition_ColliderCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("hand_l");
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(600.f, 600.f, 600.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider_Attack_0"), reinterpret_cast<CComponent**>(&m_pColliderHand_LCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pBoneMatrix = m_pMeshCom->Get_BoneMatrixPointer("hand_r");
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(600.f, 600.f, 600.f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider_Attack_1"), reinterpret_cast<CComponent**>(&m_pColliderHand_RCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTitan::SetUp_ConstantTable()
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

CTitan * CTitan::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTitan* pInstance = new CTitan(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CTitan_Prototype");
	}
	return pInstance;
}

CGameObject * CTitan::Clone(void * pArg)
{
	CTitan* pInstance = new CTitan(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CTitan_Clone");
	}
	return pInstance;
}

void CTitan::Free()
{
	__super::Free();

	Safe_Release(m_pMonsterState);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pColliderAttackCom);
	Safe_Release(m_pColliderHand_LCom);
	Safe_Release(m_pColliderHand_RCom);
	Safe_Release(m_pRecognition_ColliderCom);
}
