#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstacne.h"
#include "PlayerState.h"
#include "MonsterState.h"
#include "Monster.h"
#include "Bullet.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	:CGameObject(rhs)
{

}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();
	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	
	m_pPlayerState = CPlayerState::Create(m_pGraphicDevice,this);
	if (nullptr == m_pPlayerState)
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Pod"), TEXT("Com_Transform"), 0));

	if (nullptr == pTransform)
	{
		return E_FAIL;
	}

	const _matrix* pParentWorldMatrix = pTransform->Get_WorldMatrix();

	CDynamic_Mesh*	pPodMesh = reinterpret_cast<CDynamic_Mesh*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Pod"), TEXT("Com_Mesh"), 0));
	if (nullptr == pPodMesh)
	{
		return E_FAIL;
	}
	const _matrix* pBoneMatrix = pPodMesh->Get_BoneMatrixPointer("GameObject");

	RELEASE_INSTANCE(CGameInstacne);

	_matrix	StartMatrix = *pBoneMatrix * *pParentWorldMatrix;

	m_pTransformCom->Set_MatrixScale(0.04f, 0.04f, 0.04f);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, *reinterpret_cast<_float3*>(&StartMatrix.m[3][0]));
	m_pMeshCom->Set_AnimationIndex(IDLE_LOOP, IDLE_LOOP);


	m_iMaxHp = 210;

	m_iHp = m_iMaxHp;
	return S_OK;
}

_int CPlayer::Tick(double TimeDelta)
{
	__super::Tick(TimeDelta);
	if (m_iHp <= 0)
	{
		m_iHp = 0;
	}

	m_fBulletTime += TimeDelta;

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pPlayerState->HandleInput();
	m_pPlayerState->Update(TimeDelta);
	Character_MoveKey(TimeDelta);

	Update_HpPer();

	CComponent*	pTerrainBuffer = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer_Terrain"), 0);
	CComponent* pTerrainTrasform = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);
	m_pTransformCom->Stand_On_Plane(dynamic_cast<CTransform*>(pTerrainTrasform), dynamic_cast<CVIBuffer_Terrain*>(pTerrainBuffer));


	if ((m_pMeshCom->Play_Animation_Per_Upper() > 0.4f && m_pMeshCom->Play_Animation_Per_Upper() < 0.43f) &&
		CPlayer::ANIMATION_KEY::FIRE_PISTOL_LEFT == m_pMeshCom->Get_CurrentAnimationIndex_Upper())
	{
		CBullet::BULLETINFO BulletInfo;
		BulletInfo.eType = CBullet::GUN_BULLET::GUN_LEFT;
		BulletInfo.m_vTargetPos = m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_LOOK);

		if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Bullet"), TEXT("Layer_Bullet"), &BulletInfo)))
		{
			return E_FAIL;
		}

		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::PLAYER);
		CSound_Manager::Get_Instance()->PlaySound(L"char_commando_R_01.ogg", CSound_Manager::CHANNELID::PLAYER);
		m_fBulletTime = 0.f;
	}
	else if ((m_pMeshCom->Play_Animation_Per_Upper() > 0.4f && m_pMeshCom->Play_Animation_Per_Upper() < 0.43f) &&
		CPlayer::ANIMATION_KEY::FIRE_PISTOL_RIGHT == m_pMeshCom->Get_CurrentAnimationIndex_Upper())
	{
		CBullet::BULLETINFO BulletInfo;
		BulletInfo.eType = CBullet::GUN_BULLET::GUN_RIGHT;
		BulletInfo.m_vTargetPos = m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_LOOK);

		if (FAILED(pGameInstance->Add_GameObject_Clone(LEVEL_GAMEPLAY0, TEXT("Prototype_Bullet"), TEXT("Layer_Bullet"), &BulletInfo)))
		{
			return E_FAIL;
		}
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::PLAYER);
		CSound_Manager::Get_Instance()->PlaySound(L"char_commando_R_01.ogg", CSound_Manager::CHANNELID::PLAYER);
		m_fBulletTime = 0.f;
	}

	m_pColliderCom->Update_Collider();
	m_pOBBColliderCom->Update_Collider();

	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

_int CPlayer::Late_Tick(double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
	{
		return E_FAIL;
	}
	m_pMeshCom->Play_Animation(TimeDelta, m_fUpperSpeed, m_fLowerSpeer);;

	CGameInstacne*	pGameInstance	= GET_INSTANCE(CGameInstacne);
	_uint	iMonsterSize = pGameInstance->Get_GameObject_ListSize(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"));
	_bool	isCollision = true;

	for (_uint i = 0; i < iMonsterSize; i++)
	{
		CMonster*		pMonster = dynamic_cast<CMonster*>(pGameInstance->Find_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), i));

		if (nullptr == pMonster)
		{
			goto End;
		}

		if (CMonsterState::MOTION::ATTACK == pMonster->Get_State())
		{
			CCollider*		pColiderAttack0 = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0,TEXT("Layer_Monster"),TEXT("Com_Collider_Attack_0"),i));
			CCollider*		pColiderAttack1 = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), TEXT("Com_Collider_Attack_1"), i));
			CCollider*		pColiderAttack2 = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), TEXT("Com_Collider_Attack_2"), i));

			if (nullptr != pColiderAttack0)
			{
				isCollision = m_pOBBColliderCom->Collision_OBB(pColiderAttack0);
			}
			if (nullptr != pColiderAttack1)
			{
				isCollision = m_pOBBColliderCom->Collision_OBB(pColiderAttack1);
			}
			if (nullptr != pColiderAttack2)
			{
				isCollision = m_pOBBColliderCom->Collision_OBB(pColiderAttack2);
			}

			if (true == isCollision && false == m_isHurt)
			{
				m_isHurt = true;
				m_iHp -= 15 - 3 % rand();
			}
			else if (false == isCollision)
			{
				m_isHurt = false;
			}

			if (true == m_isHurt)
			{
				break;
			}
		}
		else if (CMonsterState::MOTION::AREA_ATTACK == pMonster->Get_State())
		{
			CCollider*		pColiderAreaAttack = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), TEXT("Com_Area_Attack"), i));

			if (nullptr == pColiderAreaAttack)
			{
				goto End;
			}

			isCollision = pColiderAreaAttack->Collision_SPHERE(m_pColliderCom);
			if (true == isCollision && false == m_isHurt)
			{
				m_isHurt = true;
				m_iHp -= 20 - 5 % rand();
			}
			else if (false == isCollision)
			{
				m_isHurt = false;
			}

			if (true == m_isHurt)
			{
				break;
			}
		}
	}

End:
	RELEASE_INSTANCE(CGameInstacne);
	if (m_iHp <= 0)
	{
		m_iHp = 0;
	}

	return _int();
}

HRESULT CPlayer::Render()
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

		for (_uint j = 0 ; j < iNumMaterial; j++)
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
	m_pRendererCom->Add_RenderDebug(m_pOBBColliderCom);
#endif // _DEBUG
#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pNavigationCom);
#endif // _DEBUG


	return S_OK;
}

HRESULT CPlayer::Render_Shadow()
{
	m_pShaderCom->Begin_Shader(5);
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
	m_pRendererCom->Add_RenderDebug(m_pOBBColliderCom);
#endif // _DEBUG
#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pNavigationCom);
#endif // _DEBUG

	return S_OK;
}

HRESULT CPlayer::Add_Component()
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

	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.25f, 2.5f, 1.25f);
	ColliderDesc.vIniPos = _float3(0.f, 2.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom),&ColliderDesc)))
	{
		return E_FAIL;
	}

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.5f, 2.7f, 1.5f);
	ColliderDesc.vIniPos = _float3(0.f, 2.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_OBBCollider"), reinterpret_cast<CComponent**>(&m_pOBBColliderCom), &ColliderDesc)))
	{
		return E_FAIL;
	}

	
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Player_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}

	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.iCurrentCellIndex = 6;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom),&NaviDesc)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable()
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

_float* CPlayer::Get_HpPer()
{
	m_fHpPer = static_cast<_float>(m_iHp) / static_cast<_float>(m_iMaxHp);

	return &m_fHpPer;
}

HRESULT CPlayer::Set_PlayerAnimation(_uint iIndex_Upper, _uint iIndex_Lower)
{
	if (nullptr == m_pMeshCom)
	{
		return E_FAIL;
	}
	m_pMeshCom->Set_AnimationIndex(iIndex_Upper, iIndex_Lower);
	return S_OK;
}

void CPlayer::Set_PlayerSpeed(_float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CPlayer::Set_PlayerDead(_bool isDead)
{
	m_isDead = isDead;
}

void CPlayer::Set_PlayerAnimationSpeed(_float fUpperSpeed, _float fLowerSpeer)
{
	m_fUpperSpeed = fUpperSpeed;
	m_fLowerSpeer = fLowerSpeer;
}

CDynamic_Mesh * CPlayer::Get_MeshCom()
{
	if (nullptr == m_pMeshCom)
	{
		return nullptr;
	}
	return m_pMeshCom;
}

CTransform * CPlayer::Get_TransformCom()
{
	return m_pTransformCom;
}

void CPlayer::Character_MoveKey(_double TimeDelta)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
	{
		m_pTransformCom->Walk_Look(TimeDelta * m_fSpeed, m_pNavigationCom);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
	{
		m_pTransformCom->Walk_Back(TimeDelta*m_fSpeed, m_pNavigationCom);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
	{
		m_pTransformCom->Walk_Left(TimeDelta*m_fSpeed, m_pNavigationCom);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
	{
		m_pTransformCom->Walk_Right(TimeDelta*m_fSpeed, m_pNavigationCom);
	}
	if (pGameInstance->Input_KeyBoard_Down(DIK_SPACE))
	{
		m_pTransformCom->Set_JumpState(true);
	}

	if ( true == m_pTransformCom->Get_JumpState())
	{
		m_pTransformCom->Jump(TimeDelta);
	}
	_long MouseMove = 0l;

	MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::DIMM::DIMM_X);
	if (0 != MouseMove)
	{
		m_pTransformCom->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta * MouseMove * 0.2f);
	}

	RELEASE_INSTANCE(CGameInstacne);

}

void CPlayer::Update_HpPer()
{
	m_fHpPer = static_cast<_float>(m_iHp) / static_cast<_float>(m_iMaxHp);
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPlayer_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{

	__super::Free();
	
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pPlayerState);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pOBBColliderCom);
	Safe_Release(m_pMeshCom);
}
