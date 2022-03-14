#include "stdafx.h"
#include "Monster_Titan_Idle.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Titan.h"

CMonster_Titan_Idle::CMonster_Titan_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Titan_Idle::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Titan_Idle::HandleInput()
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
	if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pCollider))
	{
		goto Move;
	}
	if (true == m_pMonster->Get_isHurt())
	{
		goto Hurt;
	}
	
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Hurt:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::HURT;
Death:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::DEATH;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
}

void CMonster_Titan_Idle::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CTitan::ANIMATION_KEY::IDLE_SINGLE);
}

void CMonster_Titan_Idle::Update(_float TimeDelta)
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.95f)
	{
		m_pMonster->Get_MeshCom()->Set_FrameAnimation(0.03f);
	}
}

void CMonster_Titan_Idle::Late_Update(_float TimeDelta)
{
}

void CMonster_Titan_Idle::Exit()
{
}

CMonster_Titan_Idle * CMonster_Titan_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Titan_Idle*	pInstance = new CMonster_Titan_Idle(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Titan_Idle::Free()
{
	__super::Free();
}
