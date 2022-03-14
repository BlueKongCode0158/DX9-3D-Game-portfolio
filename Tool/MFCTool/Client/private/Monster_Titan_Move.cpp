#include "stdafx.h"
#include "Monster_Titan_Move.h"
#include "GameInstacne.h"
#include "MonsterState.h"
#include "Titan.h"

CMonster_Titan_Move::CMonster_Titan_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Titan_Move::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Titan_Move::HandleInput()
{
	CGameInstacne*	pGameInstance	= GET_INSTANCE(CGameInstacne);
	CCollider*		pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (nullptr == pCollider)
	{
		goto Idle;
	}

	if (m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pCollider))
	{
		goto Attack;
	}

	if (true == m_pMonster->Get_isHurt())
	{
		goto Hurt;
	}

Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Hurt:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::HURT;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
}

void CMonster_Titan_Move::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CTitan::ANIMATION_KEY::WALKBACKWARD);
	m_fSpeed = 1.5f;
}

void CMonster_Titan_Move::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() <= 0.4f && m_pMonster->Get_MeshCom()->Play_Animation_Per() >= 0.f)
	{
		m_fSpeed = 1.5f;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per() <= 0.5f && m_pMonster->Get_MeshCom()->Play_Animation_Per() >= 0.4f)
	{
		m_fSpeed = 0.f;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() <= 0.9f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() >= 0.5f)
	{
		m_fSpeed = 1.5f;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per() <= 1.f && m_pMonster->Get_MeshCom()->Play_Animation_Per() >= 0.9f)
	{
		m_fSpeed = 0.f;
	}

	if (m_pMonster->Get_MeshCom()->Animation_State_End_Lower(CTitan::ANIMATION_KEY::WALKBACKWARD))
	{
		m_pMonster->Get_MeshCom()->Reset_FrameAnimation();
	}



	m_pMonster->Get_TransformCom()->TargetToMove(m_fSpeed*TimeDelta, pTransform);
	m_pMonster->Get_TransformCom()->Walk_Look(m_fSpeed * TimeDelta);

	RELEASE_INSTANCE(CGameInstacne);

}

void CMonster_Titan_Move::Late_Update(_float TimeDelta)
{
}

void CMonster_Titan_Move::Exit()
{
}

CMonster_Titan_Move * CMonster_Titan_Move::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Titan_Move*	pInstance = new CMonster_Titan_Move(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Titan_Move::Free()
{
	__super::Free();
}
