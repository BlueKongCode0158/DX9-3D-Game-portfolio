#include "stdafx.h"
#include "Monster_Beetle_Move.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Beetle.h"

CMonster_Beetle_Move::CMonster_Beetle_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Beetle_Move::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Beetle_Move::HandleInput()
{
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*		pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pCollider))
	{
		goto Attack;
	}
	else if ( m_fSkil0 <= 0.f )
	{
		goto  Buff;
	}
	
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Buff:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::BUFF;
}

void CMonster_Beetle_Move::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CBeetle::ANIMATION_KEY::RUNFORWARD);
	m_fSpeed = 2.5f;
}

void CMonster_Beetle_Move::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);
	CTransform*		pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	if (nullptr != pTransform)
	{
		m_pMonster->Get_TransformCom()->TargetToMove(m_fSpeed *TimeDelta,pTransform);
	}


	RELEASE_INSTANCE(CGameInstacne);
}

void CMonster_Beetle_Move::Late_Update(_float TimeDelta)
{
	__super::Late_Update(TimeDelta);
}

void CMonster_Beetle_Move::Exit()
{
	m_pMonster->Set_isHurt(false);
}

CMonster_Beetle_Move * CMonster_Beetle_Move::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Beetle_Move*	pInstance = new CMonster_Beetle_Move(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Beetle_Move::Free()
{
	__super::Free();
}
